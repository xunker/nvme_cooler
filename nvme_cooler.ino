#include <FAB_LED.h>

/*
  Trinket 85: 0 => B,0
  Bluefruit LE Micro: 5 => C,6
*/

// Declare the LED protocol and the port
// sk6812<C,6>  strip;
// sk6812b<C,6>  strip;
// ws2812<C,6>  strip;
ws2812b<C,6>  strip;

// How many pixels to control
const uint8_t numPixels = 4;

// How bright the LEDs will be (max 255)
const uint8_t maxBrightness = 127;

// The pixel array to display
grb  pixels[numPixels] = {};

void updateColors(char r, char g, char b) {
  for(int i = 0; i < numPixels; i++) {
    pixels[i].r = r;
    pixels[i].g = g;
    pixels[i].b = b;
  }
}

void setup() {
  // Serial.begin(57600);
  // Turn off the LEDs
  strip.clear(2 * numPixels);
}

uint8_t currentPixel = -1;

byte currentBrightness = 1;
#define BRIGHT_INC 5
#define BRIGHT_DEC -5

int8_t brightnessDirection = BRIGHT_INC;

void chase() {
  pixels[currentPixel].r = currentBrightness;
  pixels[currentPixel].g = 0;
  pixels[currentPixel].b = 0;

  strip.sendPixels(numPixels, pixels);

  delay(100/4);

  pixels[currentPixel].r = 0;
  pixels[currentPixel].g = 0;
  pixels[currentPixel].b = 0;

  currentPixel++;
  if (currentPixel > numPixels)
    currentPixel = 0;

  currentBrightness = currentBrightness + brightnessDirection;
  // Serial.println(currentBrightness);
  if (currentBrightness > maxBrightness) {
    // currentBrightness = 1;
    brightnessDirection = -1;
  } else if (currentBrightness < 1) {
    brightnessDirection = 1;
  }
}

void pulseAll() {
  updateColors(currentBrightness, 0, 0);

  strip.sendPixels(numPixels, pixels);

  delay(100/20);

  currentBrightness = currentBrightness + brightnessDirection;
  // Serial.println(currentBrightness);
  if (currentBrightness > maxBrightness) {
    // currentBrightness = 1;
    brightnessDirection = BRIGHT_DEC;
  } else if (currentBrightness < 1) {
    brightnessDirection = BRIGHT_INC;
  }
}

void loop() {
  chase();
  // pulseAll();
}
