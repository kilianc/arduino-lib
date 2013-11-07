/*
 *  simple_timer.h
 *  Tiny timer library for Arduino.
 *  Created by Kilian Ciuffolo on 11/09/13.
 *
 *  https://github.com/kilianc/arduino-lib
 *
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

#ifndef ___SIMPLE_TIMER_SLOTS___
#define ___SIMPLE_TIMER_SLOTS___ 5
#endif

#ifndef ___SIMPLE_TIMER___
#define ___SIMPLE_TIMER___

int set_repeat(unsigned int interval, void (*callback)(int arg), int repeat, ...);
int set_interval(unsigned int interval, void (*callback)(int arg));
int set_timeout(unsigned int interval, void (*callback)(int arg));
void update_timers();
void clear_timer(int id);

static int get_free_slot_index();
static unsigned int uuid_gen();

#endif