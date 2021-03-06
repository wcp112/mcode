/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Alexander Chumakov
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "hw-sound.h"

#include "mtick.h"
#include "hw-uart.h"
#include "mstring.h"
#include "mcode-config.h"

#include <stdbool.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

/* 2*Fclk/Fnote: 4 times (OCRn plus one), when N (prescaler) is one; */
static const uint16_t TheBase[] PROGMEM = {
  56361u, /*< C  : 261.63 Hz */
  53199u, /*< C# : 277.18 Hz */
  50213u, /*< D  : 293.66 Hz */
  47394u, /*< D# : 311.13 Hz */
  44734u, /*< E  : 329.63 Hz */
  42223u, /*< F  : 349.23 Hz */
  39854u, /*< F# : 369.99 Hz */
  37616u, /*< G  : 392.00 Hz */
  35506u, /*< G# : 415.30 Hz */
  33513u, /*< A  : 440.00 Hz */
  31632u, /*< B  : 466.16 Hz */
  29857u  /*< H  : 493.88 Hz */
};

static const uint16_t ThePreScalers[] PROGMEM = {
  1u, 8u, 32u, 64u, 128u, 256u, 1024u
};

static void sound_play_note_ex(uint16_t noteInfo);

void sound_init(void)
{
  TCCR2 = 0;
  DDRD |= _BV(DDD7);
}

void sound_deinit(void)
{
}

void sound_play_note(uint8_t note, uint16_t length)
{
  do {
    /* Parse the tone of the note */
    const uint8_t tone = (note & 0x0fu);
    if (tone > 0x0bu) {
      break;
    }

    /* Parse the octave of the note */
    uint8_t octave = ((note>>4) & 0x0fu) - 4;
    if (octave > 4) {
      break;
    }

    uint16_t base = pgm_read_word(&TheBase[tone]);
    while (octave--) {
      base >>= 1;
    }

    uint8_t i;
    uint16_t prescaler;
    uint16_t countPlusOne;
    for (i = 0; i < sizeof (ThePreScalers)/sizeof (*ThePreScalers); ++i) {
      prescaler = pgm_read_word(&ThePreScalers[i]);
      countPlusOne = (base/prescaler + 2)>>2;
      if (countPlusOne > 0u && countPlusOne < 0x100u) {
        break;
      }
    }
    if (countPlusOne < 1u || countPlusOne > 0xffu) {
      break;
    }

    /* Configure the timer2 HW */
    TCCR2 =
      (1<<WGM21)|(0<<WGM20)| /*< Counter mode: CTC */
      (0<<COM21)|(1<<COM20); /*< Output mode: Toggle output */
    TCNT2 = 0;
    OCR2 = (uint8_t)(countPlusOne - 1);
    switch (prescaler) {
    case 1:
      TCCR2 |= (0<<CS22)|(0<<CS21)|(1<<CS20);
      break;
    case 8:
      TCCR2 |= (0<<CS22)|(1<<CS21)|(0<<CS20);
      break;
    case 32:
      TCCR2 |= (0<<CS22)|(1<<CS21)|(1<<CS20);
      break;
    case 64:
      TCCR2 |= (1<<CS22)|(0<<CS21)|(0<<CS20);
      break;
    case 128:
      TCCR2 |= (1<<CS22)|(0<<CS21)|(1<<CS20);
      break;
    case 256:
      TCCR2 |= (1<<CS22)|(1<<CS21)|(0<<CS20);
      break;
    case 1024:
      TCCR2 |= (1<<CS22)|(1<<CS21)|(1<<CS20);
      break;
    default:
      /* should not appear here, disable output (on default) */
      merror(MStringInternalError);
      break;
    }
  } while (false);

  mtick_sleep(length);

  /* Turn off the sound */
  TCCR2 = 0;
}

void sound_play_tune(const uint16_t *notes)
{
  uint16_t noteInfo;
  while (0 != (noteInfo = *notes++)) {
    sound_play_note_ex(noteInfo);
  }
}

void sound_play_tune_P(const uint16_t *notes)
{
  uint16_t noteInfo;
  while (0 != (noteInfo = pgm_read_word(notes++))) {
    sound_play_note_ex(noteInfo);
  }
}

void sound_play_tune_E(const uint16_t *notes)
{
  uint16_t noteInfo;
  while (0 != (noteInfo = eeprom_read_word(notes++))) {
    sound_play_note_ex(noteInfo);
  }
}

void sound_play_note_ex(uint16_t noteInfo)
{
  const uint8_t note = (uint8_t)noteInfo;
  const uint16_t length = 20u*((uint16_t)((uint8_t)(noteInfo>>8)));

#ifdef MCODE_TUNE_TRACK
  mprintstr(PSTR("Note: 0x"));
  mprint_uint8(note, false);
  mprintstr(PSTR(", length: "));
  mprint_uintd(length, 0);
  mprint(MStringNewLine);
#endif /* MCODE_TUNE_TRACK */

  sound_play_note(note, length);
}
