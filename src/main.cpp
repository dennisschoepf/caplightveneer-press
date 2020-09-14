#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define SENSOR_PIN 8
#define LED_PIN 3
#define LED_COUNT 24
#define LED_LINE_COUNT 3
#define LED_PER_LINE_COUNT 8

void wave();
void lightLine(int lineIndex, uint32_t color);
bool isTouched = false;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
int stripIndexMatrix[3][8] = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {15, 14, 13, 12, 11, 10, 9, 8},
    {16, 17, 18, 19, 20, 21, 22, 23}};

void setup()
{
  Serial.begin(9600);
  strip.begin();
  strip.show();
  strip.setBrightness(150);
  pinMode(SENSOR_PIN, INPUT);
}

void loop()
{
  isTouched = digitalRead(SENSOR_PIN);

  if (isTouched == true)
  {
    Serial.println("Is touched");
    wave();
    strip.clear();
    strip.show();
    isTouched = false;
  }
  else
  {
    strip.clear();
    strip.show();
  }
}

void wave()
{
  for (int i = 0; i < LED_PER_LINE_COUNT; i++)
  {
    if (i <= LED_PER_LINE_COUNT / 2)
    {
      /* Turn on the first half of LED lines rather fast */
      lightLine(i, strip.Color(255, 0, 0));
      delay(15 + i * 10);
    }
    else if (i == LED_PER_LINE_COUNT / 2 + 1)
    {
      lightLine(0, strip.Color(0, 0, 0));
      lightLine(5, strip.Color(255, 0, 0));
      delay(80);

      for (int j = 0; j < LED_PER_LINE_COUNT / 2 + 1; j++)
      {
        lightLine(j, strip.Color(0, 0, 0));
        delay(50 + j * 17.5);
      }
    }
    else if (i <= LED_PER_LINE_COUNT * 3 / 4)
    {
      lightLine(i, strip.Color(255, 0, 0));
      lightLine(i - 1, strip.Color(0, 0, 0));
      delay(100 + i * 7.5);
    }
    else if (i == LED_PER_LINE_COUNT - 1)
    {
      lightLine(i, strip.Color(255, 0, 0));
      delay(125 + i * 20);
      lightLine(i - 1, strip.Color(0, 0, 0));
      delay(125 + i * 20);
      lightLine(i, strip.Color(0, 0, 0));
    }
  }
}

void lightLine(int lineIndex, uint32_t color)
{
  for (int i = 0; i < LED_LINE_COUNT; i++)
  {
    strip.setPixelColor(stripIndexMatrix[i][lineIndex], color);
  }
  strip.show();
}