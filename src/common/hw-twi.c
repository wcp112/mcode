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

#include "hw-twi.h"

#include "scheduler.h"

#include <string.h>

static bool TheSuccess;
static uint8_t *TheBuffer;

static void twi_write_done(bool success);
static void twi_read_ready(bool success, uint8_t length, const uint8_t *data);

bool twi_recv_sync(uint8_t addr, uint8_t length, uint8_t *data)
{
  TheSuccess = false;
  TheBuffer = data;
  twi_recv(addr, length, twi_read_ready);
  mcode_scheduler_start();

  return TheSuccess;
}

bool twi_send_sync(uint8_t addr, uint8_t length, const uint8_t *data)
{
  TheSuccess = false;
  twi_send(addr, length, data, twi_write_done);
  mcode_scheduler_start();

  return TheSuccess;
}

void twi_write_done(bool success)
{
  TheSuccess = success;
  mcode_scheduler_stop();
}

void twi_read_ready(bool success, uint8_t length, const uint8_t *data)
{
  TheSuccess = success;
  memcpy(TheBuffer, data, length);
  TheBuffer = 0;
  mcode_scheduler_stop();
}