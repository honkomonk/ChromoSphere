/*---------------------------------------------------------------------------------------------

  Open Sound Control (OSC) library for the ESP8266

  Example for receiving open sound control (OSC) bundles on the ESP8266
  Send integers '0' or '1' to the address "/led" to turn on/off the built-in LED of the esp8266.

  This example code is in the public domain.

--------------------------------------------------------------------------------------------- */
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>

//char ssid[] = "PropagandaFunk";          // your network SSID (name)
//char pass[] = "****";      // your network password
char ssid[] = "FabLab Karlsruhe 2";   // your network SSID (name)
char pass[] = "foobar42";             // your network password


// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
const IPAddress outIp(192,168,0,25);        // remote IP (not needed for receive)
const unsigned int outPort = 9999;          // remote port (not needed for receive)
const unsigned int localPort = 1234;        // local port to listen for UDP packets (here's where we send the packets)

OSCErrorCode error;
unsigned int ledState = LOW;              // LOW means led is *on*

#include <NeoPixelBus.h>

const uint16_t PixelCount = 4; // this assumes 4 
// ignored for Esp8266! It's GPIO03 by default (RX pin on the NodeMCU)
const uint8_t PixelPin = 0;  // make sure to set this to the correct pin

NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(PixelCount, PixelPin);

#define colorSaturation 128
RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);    // turn *off* led

  Serial.begin(115200);

  Serial.println("Initializing NeoPixelBus...");
  // this resets all the neopixels to an off state
  strip.Begin();
  strip.Show();
  
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(BUILTIN_LED, LOW);    // turn *on* led
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());

}


RgbColor ledColor(0,0,0);

void led(OSCMessage &msg, int adrOffset) {
  ledColor.R = msg.getInt(0);
  ledColor.G = msg.getInt(1); 
  ledColor.B = msg.getInt(2);
  Serial.print("[led] R:");
  Serial.print(ledColor.R);
  Serial.print(",G: ");
  Serial.print(ledColor.G);
  Serial.print(",B: ");
  Serial.println(ledColor.B);
  strip.SetPixelColor(0, ledColor);
  strip.Show();
}


void updatePixmap(OSCMessage &msg) {
  
  strip.Show();
}


void loop() {
  OSCMessage message;
  int size = Udp.parsePacket();

  if (size > 0) {
    //Serial.print("! ");
    while (size--) {
      message.fill(Udp.read());
    }
    if (!message.hasError()) {
      if (message.route("/led", led)){
        // Serial.println("led matched!");
      }
    } else {
      error = message.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}



