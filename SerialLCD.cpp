/*
 *  SerialLCD.cpp
 *  Driver for 128x64 LCD by Sparkfun with custom firmware.
 *  LCD https://www.sparkfun.com/products/710
 *  Serial Backpack https://www.sparkfun.com/products/9352
 *  Custom Firmware http://serialglcd.sourceforge.net/
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
#include "HardwareSerial.h"
#include "SerialLCD.h"

#define BITBLT_MAX_BUFF 64
#define BITBLT_DELAY 1
#define DEFAULT_DRAW_MODE 7

SerialLCD::SerialLCD(HardwareSerial &s) : serialPort(s) {}

void SerialLCD::begin(long baudrate, int ms) {
  delay(ms);
  serialPort.begin(115200);
}

void SerialLCD::toggleSplash() {
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::SPLASH_TOGGLE);
}

void SerialLCD::clearDisplay() {
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::CLEAR_DISPLAY);
}

void SerialLCD::debugLevel(char level) {
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::DEBUG_LEVEL);
  serialPort.write(level);
}

void SerialLCD::backLight(char val) {
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::BACKLIGHT_DUTY_CICLE);
  serialPort.write(val);
}

void SerialLCD::eraseBlock(char x1, char y1, char x2, char y2) {
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::ERASE_BLOCK);
  serialPort.write(x1);
  serialPort.write(y1);
  serialPort.write(x2);
  serialPort.write(y2);
}

void SerialLCD::reset() {
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::RESET);
}

void SerialLCD::baudRate(char val) {
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::BAUD_RATE);
  serialPort.write(val);
}

void SerialLCD::setPixel(char x, char y, char status) {
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::SET_PIXEL);
  serialPort.write(x);
  serialPort.write(y);
  serialPort.write(status);
}

void SerialLCD::filledBox(char x1, char y1, char x2, char y2, char fill) {
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::FILLED_BOX);
  serialPort.write(x1);
  serialPort.write(y1);
  serialPort.write(x2);
  serialPort.write(y2);
  serialPort.write(fill);
}

void SerialLCD::reverseMode() {
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::REVERSE_MODE);
}

void SerialLCD::bitblt(char x, char y, char data[]) {
  bitblt(x, y , DEFAULT_DRAW_MODE, data);
}

void SerialLCD::bitblt(char x, char y, char mode, char data[]) {
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::BITBLT);
  serialPort.write(x);
  serialPort.write(y);
  serialPort.write(mode);

  int len = data[0] * data[1] / 8 + 2;

  for (int i = 0; i < len; i++) {
    serialPort.write(data[i]);
    if (!(i % BITBLT_MAX_BUFF)) delay(BITBLT_DELAY);
  }
}

void SerialLCD::placeCursor(char x, char y){
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::SET_X);
  serialPort.write(x);
  serialPort.write(SerialLCD::START_COMMAND);
  serialPort.write(SerialLCD::SET_Y);
  serialPort.write(y);
}

// CUSTOM

void SerialLCD::writeString(String str) {
  serialPort.println(str);
}

#undef BITBLT_MAX_BUFF 64
#undef BITBLT_DELAY 1
#undef DEFAULT_DRAW_MODE 7