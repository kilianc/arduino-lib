/*
 *  SerialLCD.cpp
 *  Driver for 128x64 LCD by Sparkfun with custom firmware.
 *   - LCD https://www.sparkfun.com/products/710
 *   - Serial Backpack https://www.sparkfun.com/products/9352
 *   - Custom Firmware http://serialglcd.sourceforge.net/
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

class HardwareSerial;
class String;

class SerialLCD {
  private:
    static const char START_COMMAND = 0x7c;
    static const char CLEAR_DISPLAY = 0x00;
    static const char DEBUG_LEVEL = 0x01;
    static const char BACKLIGHT_DUTY_CICLE = 0x02;
    static const char DRAW_CIRCLE = 0x03;
    static const char CR_LF_TOGGLE = 0x04;
    static const char ERASE_BLOCK = 0x05;
    static const char RESET = 0x06;
    static const char BAUD_RATE = 0x07;
    static const char AUX_FONT_TOGGLE = 0x08;
    static const char FONT_MODE = 0x0A;
    static const char DRAW_SPRITE = 0x0B;
    static const char DRAW_LINE = 0x0C;
    static const char UPLOAD_SPRITE = 0x0D;
    static const char DRAW_BOX = 0x0F;
    static const char SET_PIXEL = 0x10;
    static const char FILLED_BOX = 0x12;
    static const char REVERSE_MODE = 0x14;
    static const char SPLASH_TOGGLE = 0x15;
    static const char BITBLT = 0x16;
    static const char SET_X = 0x18;
    static const char SET_Y = 0x19;
    HardwareSerial &serialPort;
  public:
    SerialLCD(HardwareSerial &);
    void begin(long, int);
    void toggleSplash();
    void clearDisplay();
    void debugLevel(char);
    void backLight(char);
    void eraseBlock(char, char, char, char);
    void reset();
    void baudRate(char);
    void setPixel(char, char, char);
    void filledBox(char, char, char, char, char);
    void reverseMode();
    void bitblt(char, char, char[]);
    void bitblt(char, char, char, char[]);
    void placeCursor(char, char);
    void writeString(String);
};