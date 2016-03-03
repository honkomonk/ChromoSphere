/* StableWS2811 FlickerTest.ino - RGB LED Test
   Copyright (c) 2014 Jim Paris
   Copyright (c) 2013 Paul Stoffregen, PJRC.COM, LLC

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

   Required Connections
   --------------------
   pin 7:  LED strip data  (OctoWS2811 adapter output #3, green wire)

   This test is useful for checking if your LED strips work, and to
   verify that there is no flickering.  All LEDs will be lit at
   minimum brightness, with brighter colored pixels moving along the
   strip, and the strip will be updated as quickly as possible.  */

#include <StableWS2811.h>

const int stripLen = 49;
const int config = WS2811_GRB | WS2811_800kHz;

static DMAMEM uint32_t spiBuf[stripLen * 6];
static uint8_t pixelBuf[stripLen * 3];

StableWS2811 leds(stripLen, spiBuf, pixelBuf, config);

int red, green, blue;

void setup() {
  leds.begin();
  leds.show();
}

void loop() {
  int wait = 50000; // wait in microseconds
  
  for (int c=0; c<767; c++) { 
    red   = 255 - min(255, abs(255-c));
    green = 255 - min(255, abs(511-c));
    blue  = 255 - min(min(255, abs(767-c)), c);
    
    leds.setPixel(0 , red, green, blue);
    leds.setPixel(1 , blue, red, green);
    leds.setPixel(2 , green, blue, red);
    leds.setPixel(3 , red, green, blue);
    leds.setPixel(4 , blue, red, green);
    leds.setPixel(5 , green, blue, red);
    leds.setPixel(6 , red, green, blue);
    leds.setPixel(7 , blue, red, green);
    leds.setPixel(8 , green, blue, red);
    leds.setPixel(9 , red, green, blue);
    leds.setPixel(10, blue, red, green);
    leds.setPixel(11, green, blue, red);
    leds.setPixel(12, red, green, blue);
    leds.setPixel(13, blue, red, green);
    leds.setPixel(14, green, blue, red);
    leds.setPixel(15, red, green, blue);
    leds.setPixel(16, blue, red, green);
    leds.setPixel(17, green, blue, red);    
    leds.setPixel(18, red, green, blue);
    leds.setPixel(19, blue, red, green);
    leds.setPixel(20, green, blue, red); 
    leds.setPixel(21, red, green, blue);
    leds.setPixel(22, blue, red, green);
    leds.setPixel(23, green, blue, red); 
    leds.setPixel(24, red, green, blue);
    leds.setPixel(25, red, green, blue);
    leds.setPixel(26, blue, red, green);
    leds.setPixel(27, green, blue, red);
    leds.setPixel(28, red, green, blue);
    leds.setPixel(29, blue, red, green);
    leds.setPixel(30, green, blue, red);
    leds.setPixel(31, red, green, blue);
    leds.setPixel(32, blue, red, green);
    leds.setPixel(33, green, blue, red);
    leds.setPixel(34, red, green, blue);
    leds.setPixel(35, blue, red, green);
    leds.setPixel(36, green, blue, red);
    leds.setPixel(37, red, green, blue);
    leds.setPixel(38, blue, red, green);
    leds.setPixel(39, green, blue, red);
    leds.setPixel(40, red, green, blue);
    leds.setPixel(41, blue, red, green);
    leds.setPixel(42, green, blue, red);    
    leds.setPixel(43, red, green, blue);
    leds.setPixel(44, blue, red, green);
    leds.setPixel(45, green, blue, red); 
    leds.setPixel(46, red, green, blue);
    leds.setPixel(47, blue, red, green);
    leds.setPixel(48, green, blue, red); 
    leds.show();
    delayMicroseconds(wait);
  }
  
}

