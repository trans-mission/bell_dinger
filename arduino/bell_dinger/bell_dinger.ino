/**
  Bell Dinger
  
  This sketch uses a microphone to detect blowing.
  If sound amplitude is higher than a set amount,
  a neopixel LED strip begins to light up.
  If the lights reach the end of the LED strip,
  a solenoid is activated, which dings a bell.

  OK! TRANSMIT - 2020
  by mmansion
*/


#include <Adafruit_NeoPixel.h>

#define PIX_SIG_PIN 6
#define SOL_PIN 2
#define MIC_SIG_PIN A0
#define MIC_PWR_PIN 12
#define SMOOTHING 20
#define VOL 10

int avg[SMOOTHING];
int mic = 0;
int inc = 0;
int sum = 0;
int numPx = 0;
int totPx = 144;
long lastDing = 0;
int  dingDelay = 1000;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(totPx, PIX_SIG_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  
  Serial.begin(9600);

  // pin setup
  pinMode(MIC_PWR_PIN, OUTPUT);
  pinMode(MIC_SIG_PIN, INPUT);
  pinMode(SOL_PIN, OUTPUT);

  // pin states at start
  digitalWrite(MIC_PWR_PIN, HIGH);
  digitalWrite(SOL_PIN, LOW);
  
  //zero the rolling average array
  memset(avg,0,sizeof(avg));

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {

  int v = analogRead(MIC_SIG_PIN);
  if(v > 0) {
   avg[inc] = map(v, 0, 1024, 0, 100);
  }
  
  sum = 0;
  for(int i = 0; i < SMOOTHING; i++) {
    sum+=avg[i];
    mic = sum/SMOOTHING;  
  }
  
  inc++;
  inc %= SMOOTHING;
  //Serial.println(mic);

  if(mic> VOL) {
    numPx++;
    if(numPx >= totPx) {
      numPx = 144;
      ding();
    }
  } else {
    numPx--;
    if(numPx < 0) numPx = 0;
  }

  Serial.println(numPx);
  setLeds();
}

void ding() {
  numPx = 0;
  digitalWrite(SOL_PIN, HIGH);
  delay(100);
  digitalWrite(SOL_PIN,LOW);
  strip.clear();
  strip.show();
  delay(3000);
}

// Fill the dots one after the other with a color
void setLeds() {
  strip.clear();
  strip.show();
  // turn on amount
  for(uint16_t i=0; i< numPx; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  strip.show();
}
