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

#include "scheduler.h"

#include <string.h>

#define MCODE_TICKS_COUNT (8)

static int NoExitRequest;
static int ClientsNumber;
static mcode_tick TheApplicationTicks[MCODE_TICKS_COUNT];

void mcode_scheduler_init(void)
{
  ClientsNumber = 0;
  NoExitRequest = 1;
  memset(TheApplicationTicks, 0, sizeof (TheApplicationTicks));
}

void mcode_scheduler_deinit(void)
{
  memset(TheApplicationTicks, 0, sizeof (TheApplicationTicks));
}

void mcode_scheduler_start(void)
{
  int i;
  while (NoExitRequest) {
    for (i = 0; i < ClientsNumber; i++) {
      mcode_tick tick = TheApplicationTicks[i];
      if (tick) {
        (*tick)();
      }
    }
  }
}

void mcode_scheduler_stop(void)
{
  NoExitRequest = 0;
}

void mcode_scheduler_add(mcode_tick tick)
{
  if (ClientsNumber < MCODE_TICKS_COUNT) {
    TheApplicationTicks[ClientsNumber++] = tick;
  } else {
    /*! @todo add assert(false) here */
  }
}