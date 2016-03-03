#include <Adafruit_NeoPixel.h>


#define D0   16
#define D1   5
#define D2   4
#define D3   0
#define D4   2
#define D5   14
#define D6   12
#define D7   13
#define D8   15
#define D9   3
#define D10  1


const int stripLen = 49;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel leds = Adafruit_NeoPixel(stripLen, D3, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.



int loopMax = 767;
int perPixelShift = 25;
int red, green, blue;

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.println("Init leds ... ");
  
  leds.begin();
  leds.show();
}

void loop() {
  
  for (int c=0; c<loopMax; c++) { 
    for (int ledNr=0; ledNr<stripLen; ledNr++) {
      int cMod = (c + ledNr*perPixelShift) % loopMax;
      red   = 255 - min(255, abs(255-cMod));
      green = 255 - min(255, abs(511-cMod));
      blue  = 255 - min(min(255, abs(767-cMod)), cMod); 
      leds.setPixelColor(ledNr, leds.Color(red, green, blue));
    }

    Serial.println(String(c));
    
    leds.show();
    delay(100);
  }
  
}

