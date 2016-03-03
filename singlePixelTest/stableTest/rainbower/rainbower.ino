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

int loopMax = 767;
int perPixelShift = 25;
int red, green, blue;

void setup() {
  leds.begin();
  leds.show();
}

void loop() {
  int wait = 100000; // wait in microseconds
  
  for (int c=0; c<loopMax; c++) { 
    for (int ledNr=0; ledNr<stripLen; ledNr++) {
      int cMod = (c + ledNr*perPixelShift) % loopMax;
      red   = 255 - min(255, abs(255-cMod));
      green = 255 - min(255, abs(511-cMod));
      blue  = 255 - min(min(255, abs(767-cMod)), cMod); 
      leds.setPixel(ledNr, red, green, blue);
    }
 
    leds.show();
    delayMicroseconds(wait);
  }
  
}

