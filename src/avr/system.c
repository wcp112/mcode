/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Alexander Chumakov
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

#include "system.h"

#include "mtick.h"
#include "hw-uart.h"

#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

void reboot(void)
{
  hw_uart_write_string_P(PSTR("\r\nSystem reboot has been requested.\r\n"));
/*  mtick_sleep(1);*/
  /* Disable all interrupts */
  cli();
  /* Enable the Watchdog timer with the smallest timeout */
  wdt_enable(WDTO_15MS);
  /* Peacefully wait for the system reset */
  for(;;);
}
