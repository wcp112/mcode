/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Alexander Chumakov
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

#include "mtick.h"
#include "hw-lcd.h"
#include "console.h"
#include "hw-leds.h"
#include "hw-uart.h"
#include "mstring.h"
#include "scheduler.h"
#include "cmd-engine.h"
#include "mcode-config.h"

#include <stm32f10x.h>

#ifdef MCODE_DEBUG_BLINKING
static void main_tick(void);
#endif /* MCODE_DEBUG_BLINKING */
static void main_mtick(void);

int main(void)
{
  /* first, init the scheduler */
  mcode_scheduler_init();
  mtick_init();
  leds_init();
  hw_uart_init();
  lcd_init(240, 320);
  mtick_add(main_mtick);
  mprintstrln(PSTR("ARM variant started."));
#ifdef MCODE_DEBUG_BLINKING
  mcode_scheduler_add(main_tick);
#endif /* MCODE_DEBUG_BLINKING */
  console_init();
  cmd_engine_init();

  cmd_engine_start();
  mcode_scheduler_start();

  /* Clean-up */
  cmd_engine_deinit();
  lcd_deinit();
  leds_deinit();
  mtick_deinit();
  mcode_scheduler_deinit();
  return 0;
}

#ifdef MCODE_DEBUG_BLINKING
static int TheCase = 0;
static int TheDelay = 0;
void main_tick(void)
{
  if (TheDelay++ != 0xFFFFU) {
    return;
  }
  TheDelay = 0;

  switch (TheCase) {
  case 0:
    leds_set(1, 1);
    leds_set(2, 0);
    leds_set(3, 0);
    break;
  case 1:
    leds_set(1, 0);
    leds_set(2, 1);
    leds_set(3, 0);
    break;
  case 2:
    leds_set(1, 0);
    leds_set(2, 0);
    leds_set(3, 1);
    break;
  default:
    break;
  }

  if (++TheCase > 2) TheCase = 0;
}
#endif /* MCODE_DEBUG_BLINKING */

void main_mtick(void)
{
  static int n = 0;
  if (++n != 1000) {
    return;
  } else {
    n = 0;
  }

  static bool on = false;
  on = !on;
}
