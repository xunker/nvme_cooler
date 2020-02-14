#include <FAB_LED.h>

/*
  Trinket 85: 0 => B,0
  Bluefruit LE Micro: 5 => C,6
*/

// Declare the LED protocol and the port
// sk6812<C,6>  strip;
// sk6812b<C,6>  strip;
// ws2812<C,6>  strip;
// ws2812b<C,6>  strip;
// apa106<C,6>  strip;
apa104<C,6>  strip;

// How many pixels to control
const uint8_t numPixels = 4;

// How bright the LEDs will be (max 255)
const uint8_t maxBrightness = 63;

// The pixel array to display
grb  pixels[numPixels] = {};

void updateColors(char r, char g, char b) {
  for(int i = 0; i < numPixels; i++) {
    pixels[i].r = r;
    pixels[i].g = g;
    pixels[i].b = b;
  }
}

#define FIRST_PIXEL 0
uint8_t currentPixel = FIRST_PIXEL;

byte currentBrightness = 1;
#define BRIGHT_INC 2
#define BRIGHT_DEC -1

int8_t brightnessDirection = BRIGHT_INC;

int16_t pixelBrightness[numPixels] = {}; // 16-bit int to account for states above maxBrightness or below 0
int8_t pixelDirection[numPixels] = {};

#define FADE_EVERY 25 // ms
unsigned long nextFade = FADE_EVERY;

#define MOVE_EVERY 1000 // ms
unsigned long nextMove = MOVE_EVERY;

void setup() {
  // Serial.begin(57600);
  // Turn off the LEDs
  strip.clear(2 * numPixels);

  for (uint8_t i = 0; i < numPixels; i++)
  {
    pixelBrightness[i] = 0;
    pixelDirection[i] = 0;
  }
}

void chaseFade() {
  unsigned long currentMillis = millis();

  if (nextFade < currentMillis) {
    nextFade = currentMillis + FADE_EVERY;

    for(uint8_t i = 0; i < numPixels; i++) {
      if (pixelDirection[i] == 0)
        continue;

      pixelBrightness[i] = pixelBrightness[i] + pixelDirection[i];

      pixels[i].g = pixelBrightness[i];

      strip.sendPixels(numPixels, pixels);

      if (pixelBrightness[i] > maxBrightness) {
        pixelDirection[i] = BRIGHT_DEC;
      } else if (pixelBrightness[i] < 1) {
        pixelDirection[i] = 0;
      }
    }
  }

  if (nextMove < currentMillis) {
    nextMove = currentMillis + MOVE_EVERY;

    currentPixel++;
    if (currentPixel >= numPixels)
      currentPixel = FIRST_PIXEL;

    pixelDirection[currentPixel] = BRIGHT_INC;
  }
}

void simple() {
  if (currentPixel < 0)
    currentPixel = 0;

  pixels[currentPixel].r = maxBrightness;

  strip.sendPixels(numPixels, pixels);

  pixels[currentPixel].r = 0;

  delay(1000);
  currentPixel++;
  if (currentPixel > numPixels-1)
    currentPixel = 0;

}

void loop() {
  chaseFade();
  // simple();
}
