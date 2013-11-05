#include <stdint.h>
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