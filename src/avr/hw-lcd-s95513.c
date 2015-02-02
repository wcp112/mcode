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

#include "hw-lcd.h"

#include "hw-i80.h"

#include <stdint.h>
#include <stdlib.h>

void lcd_turn(bool on)
{
  if (on) {
    /* exit_sleep_mode */
    hw_i80_write (UINT8_C(0x11), 0, NULL);
    /* set_display_on */
    hw_i80_write (UINT8_C(0x29), 0, NULL);
    /* set_pixel_format: 18bpp */
    uint8_t byte = 0x55;
    hw_i80_write (UINT8_C(0x3A), 1, &byte);
    byte = UINT8_C(0x02);
    hw_i80_write (UINT8_C(0x36), 1, &byte);
    lcd_set_scroll_start(UINT16_C(0));
  } else {
    /* set_display_off */
    hw_i80_write (UINT8_C(0x28), 0, NULL);
    /* enter_sleep_mode */
    hw_i80_write (UINT8_C(0x10), 0, NULL);
  }
}