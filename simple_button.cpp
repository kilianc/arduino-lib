/*
 *  simple_button.cpp
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

#include "simple_button.h"
#include "Arduino.h"

void simple_button_set(unsigned int pin, simple_button_t *button, unsigned int mode, unsigned long held_treshold, unsigned long held_frequency) {
  (*button).pin = pin;
  pinMode(pin, INPUT);
  if (mode) {
    digitalWrite((*button).pin, HIGH);
  }
}

int simple_button_read(simple_button_t *button) {
  unsigned long ms = millis();

  (*button).last_read = ms;
  (*button).previus_read = (*button).read;
  (*button).read = digitalRead((*button).pin);
  (*button).down = !(*button).read;
  (*button).up = (*button).read;

  if ((*button).previus_read != (*button).read) {
    (*button).last_change = ms;
    (*button).last_held = ms;
    (*button).held = 0;
    (*button).held_count = 0;
    (*button).click = !(*button).down;
    (*button).rising_edge = (*button).down;
    (*button).falling_edge = !(*button).down;
  } else {
    // nothing changed then clear event flags
    (*button).rising_edge = 0;
    (*button).falling_edge = 0;
    (*button).click = 0;
    // check for held event
    if ((*button).down && (ms - (*button).last_change) >= (*button).held_treshold && (ms - (*button).last_held) >= (*button).held_frequency) {
      (*button).held = 1;
      (*button).held_count++;
      (*button).last_held = ms;
      // accelleration y = x / (30 / (x + 25))
      (*button).value = (*button).held_count / (30 / (*button).held_count + 25);
    } else {
      (*button).held = 0;
      (*button).value = 0;
    }
  }
  // trigger callback
  if ((*button).click && (*button).click_cb != NULL) (*button).click_cb(button);
  if ((*button).rising_edge && (*button).rising_edge_cb != NULL) (*button).rising_edge_cb(button);
  if ((*button).falling_edge && (*button).falling_edge_cb != NULL) (*button).falling_edge_cb(button);
  if ((*button).held && (*button).held_cb != NULL) (*button).held_cb(button);
}