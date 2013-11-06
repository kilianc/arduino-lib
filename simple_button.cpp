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

void simple_button_set(unsigned int pin, simple_button_t *button, unsigned int mode, unsigned long hold_treshold, unsigned long hold_frequency) {
  (*button).pin = pin;
  (*button).mode = mode;
  (*button).hold_treshold = hold_treshold;
  (*button).hold_frequency = hold_frequency;
  (*button).last_read = (*button).read = mode;

  pinMode(pin, INPUT);
  if (mode) digitalWrite(pin, HIGH);
}

int simple_button_read(simple_button_t *button) {
  unsigned long ms = millis();

  (*button).last_read = ms;
  (*button).previus_read = (*button).read;
  (*button).read = digitalRead((*button).pin);
  (*button).down = (*button).mode ? !(*button).read : (*button).read;
  (*button).up = !(*button).down;

  if ((*button).previus_read != (*button).read) {
    (*button).last_change = ms;
    (*button).last_hold = ms;
    (*button).hold = 0;
    (*button).hold_count = 0;
    (*button).click = !(*button).down;
    (*button).rising_edge = (*button).down;
    (*button).falling_edge = !(*button).down;
  } else {
    // nothing changed then clear event flags
    (*button).rising_edge = 0;
    (*button).falling_edge = 0;
    (*button).click = 0;
    // check for hold event
    if ((*button).down && (ms - (*button).last_change) >= (*button).hold_treshold && (ms - (*button).last_hold) >= (*button).hold_frequency) {
      (*button).hold = 1;
      (*button).hold_count++;
      (*button).last_hold = ms;
    } else {
      (*button).hold = 0;
    }
  }
  // trigger callback
  if ((*button).rising_edge && (*button).rising_edge_cb != NULL) (*button).rising_edge_cb(button);
  if ((*button).falling_edge && (*button).falling_edge_cb != NULL) (*button).falling_edge_cb(button);
  if ((*button).hold && (*button).hold_cb != NULL) (*button).hold_cb(button);
  if ((*button).click && (*button).click_cb != NULL) (*button).click_cb(button);
}