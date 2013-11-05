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

static timer_t timers_list[___SIMPLE_TIMER_SLOTS___];

int set_repeat(unsigned int interval, void (*callback)(va_list args), int repeat, ...) {
  // read variable list
  va_list args;
  va_start(args, repeat);
  va_end(args);

  int id = get_free_id();

  if (id == FULL)
    return FULL;

  timer_t timer = timers_list[id];

  timer.status = RUNNING;
  timer.interval = interval;
  timer.last_call = 0;
  timer.repeat = repeat;
  timer.tick_count = 0;
  timer.callback = callback;
  timer.args = args;

  return id;
}

int set_interval(unsigned int interval, void (*callback)(va_list args)) {
  return set_repeat(interval, callback, 0);
}

int set_timeout(unsigned int interval, void (*callback)(va_list args)) {
  return set_repeat(interval, callback, 1);
}

void update_timers() {
  unsigned int i;
  unsigned long ms = millis();
  timer_t timer;

  for (i = 0; i < ___SIMPLE_TIMER_SLOTS___; ++i) {
    timer = timers_list[i];
    if (timer.status == RUNNING && (ms - timer.last_call) >= timer.interval) {
      timer.last_call = ms;
      timer.tick_count++;
      timer.callback(timer.args);
      if (timer.repeat == timer.tick_count) {
        timer.status = COMPLETE;
      }
    }
  }
}

void clear_timer(int id) {
  if (id < ___SIMPLE_TIMER_SLOTS___)
    timers_list[id].status = COMPLETE;
}

int get_free_id() {
  unsigned int i;
  for (i = 0; i < ___SIMPLE_TIMER_SLOTS___; ++i) {
    if (timers_list[i].status == COMPLETE)
      return i;
  }
  return FULL;
}