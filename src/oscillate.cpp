/*
 *  oscillate.cpp
 *  Oscillates digital output n times at f frequency.
 *
 *  https://github.com/kilianc/arduino-lib
 *
 *  Created by Kilian Ciuffolo on 11/09/13.
 *  This software is released under the MIT license cited below.
 *
 *  Copyright (c) 2010 Kilian Ciuffolo, me@nailik.org. All Rights Reserved.
 *
 *  Permission is hereby granted, free of charge, to any person
 *  obtaining a copy of this software and associated documentation
 *  files (the 'Software'), to deal in the Software without
 *  restriction, including without limitation the rights to use,
 *  copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following
 *  conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *  OTHER DEALINGS IN THE SOFTWARE.
 */

#include "oscillate.h"
#include "Arduino.h"
#include <simple_timer.h>

 typedef struct {
   unsigned char value;
   unsigned int timer_id;
 } pin_t;

static pin_t pin_slots[14] = { [0 ... 13] { 0, -1 } };

int oscillate(int pin, unsigned long interval, int start_value, char times) {
  pin_slots[pin].pin = pin;
  pin_slots[pin].value = start_value;

  clear_timer(pin_slots[pin].timer_id);
  pin_slots[pin].timer_id = set_repeat(interval, toggle_pin, times, pin);

  digitalWrite(pin, start_value);

  return pin_slots[pin].timer_id;
}

void toggle_pin(int tick_count, int pin) {
  pin_t target = pin_slots[pin];
  target.value ^= 1;
  digitalWrite(pin, target.value);
}