#include <OctoWS2811.h>

const int ledsPerStrip = 1;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

int red, green, blue;

void setup() {
  leds.begin();
  leds.show();
}

void loop() {
  int wait = 100000; // wait in microseconds
  
  for (int c=0; c<767; c++) { 
    red   = 255 - min(255, abs(255-c));
    green = 255 - min(255, abs(511-c));
    blue  = 255 - min(min(255, abs(767-c)), c);
    
    leds.setPixel(0, red, green, blue);
    leds.setPixel(1, blue, red, green);
    leds.setPixel(2, green, blue, red);
    leds.show();
    delayMicroseconds(wait);
  }
  
   /* leds.setPixel(0, 255, 0, 0);
    leds.show();
    delayMicroseconds(1000000);
    leds.setPixel(0, 0, 255, 0);
    leds.show();
    delayMicroseconds(1000000);
    leds.setPixel(0, 0, 0, 255);
    leds.show();
    delayMicroseconds(1000000);
  */

}



