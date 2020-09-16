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

/* Set up neopixel led strip as described in their documentation */
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

/* Indices of the light strip LEDs, first element in array marks the bottommost LED of each strip to be able to illuminate them in a line */
int stripIndexMatrix[3][8] = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {15, 14, 13, 12, 11, 10, 9, 8},
    {16, 17, 18, 19, 20, 21, 22, 23}};

void setup()
{
  Serial.begin(9600);

  /* Basic strip setup, adjust brightness as needed */
  strip.begin();
  strip.show();
  strip.setBrightness(200);

  pinMode(SENSOR_PIN, INPUT);
}

void loop()
{
  /* Digital Read from capacitative sensor, 1 == isTouched */
  isTouched = digitalRead(SENSOR_PIN);

  if (isTouched == true)
  {
    Serial.println("Is touched");
    wave();
    strip.clear();
    strip.show();
    /* Set isTouched to false after illuminating once in order to not immediately re-trigger the light sequence */
    isTouched = false;
  }
  else
  {
    strip.clear();
    strip.show();
  }
}

void outwards()
{
}

void wave()
{
  /* Loop through all LED lines (1 LED per strip depending on the matrix setup) to create wave/ripple effect */
  for (int i = 0; i < LED_PER_LINE_COUNT; i++)
  {
    if (i <= LED_PER_LINE_COUNT / 2)
    {
      /* Turn on the first half of LED lines rather fast to support the animation */
      lightLine(i, strip.Color(255, 0, 0));
      delay(15 + i * 10);
    }
    else if (i == LED_PER_LINE_COUNT / 2 + 1)
    {
      /* Begin to shut off the first LEDs as soon as the animation went through half of the LED lines */
      lightLine(0, strip.Color(0, 0, 0));
      lightLine(5, strip.Color(255, 0, 0));
      delay(80);

      /* Loop through the first half of LED lines to turn them off again */
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
      /* Increase delay for the last LED lines */
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
  /* Illuminates all LEDs in a line depending on how the stripIndexMatrix is set up */
  for (int i = 0; i < LED_LINE_COUNT; i++)
  {
    strip.setPixelColor(stripIndexMatrix[i][lineIndex], color);
  }
  strip.show();
}