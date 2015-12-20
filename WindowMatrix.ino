#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

static const uint8_t  kMatrixPin    = 4;
static const int      kMatrixRows   = 11;
static const int      kMatrixCols   = 12;
static const int      kMatrixPixels = kMatrixRows * kMatrixCols;

static Adafruit_NeoMatrix matrix(
  kMatrixCols, kMatrixRows, kMatrixPin,
  NEO_MATRIX_TOP | NEO_MATRIX_LEFT | NEO_MATRIX_ZIGZAG,
  NEO_RGB | NEO_KHZ400
);

// simple color definition
struct RGB {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

// pull in the code generated images
// generates:
//
//  static const RGB    kImages[][kMatrixPixels] = {...};
//  static const size_t kImageCount = ...;
//
#include "Images.h"

// draws an RGB array assuming row-ordered, progressive flattened array
void draw_rgb_array(RGB const *pixels) {
  for (int x = 0; x < kMatrixCols; x++) {
    for (int y = 0; y < kMatrixRows ; y++) {
      uint16_t color = matrix.Color(pixels->r, pixels->g, pixels->b);
      matrix.drawPixel(x,y, color);
      pixels++;
    }
  }
}

void setup() {
  matrix.begin();
  matrix.show();
}

void loop() {
  for (int i = 0; i < kImageCount; i++) {
    draw_rgb_array(kImages[i]);
    matrix.show();
    delay(5000);
  }
}
