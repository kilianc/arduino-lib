/*
 *  simple_timer.cpp
 *  Tiny timer library for Arduino.
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

#include "Arduino.h"
#include "simple_timer.h"
#include <stdarg.h>

static unsigned int uid;
static timer_t timers_slots[___SIMPLE_TIMER_SLOTS___];

int set_repeat(unsigned int interval, void (*callback)(int arg), int repeat, ...) {
  int i = get_free_slot_index();

  if (i == FULL)
    return FULL;

  timers_slots[i].id = uuid_gen();
  timers_slots[i].status = RUNNING;
  timers_slots[i].interval = interval;
  timers_slots[i].last_call = millis();
  timers_slots[i].repeat = repeat;
  timers_slots[i].tick_count = 0;
  timers_slots[i].callback = callback;

  // read variable list
  va_list args;
  va_start(args, repeat);
  timers_slots[i].arg = va_arg(args, int);
  va_end(args);

  return timers_slots[i].id;
}

int set_interval(unsigned int interval, void (*callback)(int arg)) {
  return set_repeat(interval, callback, 0);
}

int set_timeout(unsigned int interval, void (*callback)(int arg)) {
  return set_repeat(interval, callback, 1);
}

void update_timers() {
  unsigned int i;
  unsigned long ms = millis();

  for (i = 0; i < ___SIMPLE_TIMER_SLOTS___; ++i) {
    if (timers_slots[i].status == RUNNING && (ms - timers_slots[i].last_call) >= timers_slots[i].interval) {
      timers_slots[i].last_call = ms;
      timers_slots[i].tick_count++;
      timers_slots[i].callback(timers_slots[i].arg);
      if (timers_slots[i].repeat == timers_slots[i].tick_count) {
        timers_slots[i].status = COMPLETE;
      }
    }
  }
}

void clear_timer(int id) {
  unsigned int i;
  for (i = 0; i < ___SIMPLE_TIMER_SLOTS___; ++i) {
    if (timers_slots[i].id == id)
      timers_slots[i].status = COMPLETE;
  }
}

int get_free_slot_index() {
  unsigned int i;
  for (i = 0; i < ___SIMPLE_TIMER_SLOTS___; ++i) {
    if (timers_slots[i].status == COMPLETE)
      return i;
  }
  return FULL;
}

unsigned int uuid_gen() {
  return uid = (uid + 1) % 65536;
}