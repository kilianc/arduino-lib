/*
 *  simple_button.h
 *  Stupid simple button library for Arduino.
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

typedef struct simple_button_struct simple_button_t;
struct simple_button_struct {
  // digital pin state
  unsigned int pin;
  bool previus_read;
  bool read;
  // button state
  bool down;
  bool up;
  // event flags
  bool click;
  bool rising_edge;
  bool falling_edge;
  bool held;
  // callbacks
  void (*click_cb)(simple_button_t *button);
  void (*rising_edge_cb)(simple_button_t *button);
  void (*falling_edge_cb)(simple_button_t *button);
  void (*held_cb)(simple_button_t *button);
  // hold
  unsigned long held_treshold;    // time before the first held event (ms)
  unsigned long held_frequency;   // frequency after held_treshold of held event (ms)
  unsigned int held_count;
  unsigned long value;            // value of button that accellerates proportionally to held_count
  // private
  unsigned long last_read;
  unsigned long last_change;
  unsigned long last_held;
};

void simple_button_set(unsigned int pin, simple_button_t *button, unsigned int mode, unsigned long held_treshold, unsigned long held_frequency);
int simple_button_read(simple_button_t *button);