#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define SENSOR_PIN 2
#define LED_PIN 3
#define LED_COUNT 24

bool isTouched = false;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(9600);
  strip.begin();
  strip.show();
  strip.setBrightness(50);
  pinMode(SENSOR_PIN, INPUT);
}

void loop()
{
  isTouched = digitalRead(SENSOR_PIN);

  if (isTouched == true)
  {
    Serial.println("Is touched");
    strip.setPixelColor(0, 255, 0, 0);
    strip.show();
  }
  else
  {
    strip.clear();
    strip.show();
  }
}