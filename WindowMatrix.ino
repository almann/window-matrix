#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

namespace {

  const uint8_t  kMatrixPin    = 4;
  const int      kMatrixRows   = 11;
  const int      kMatrixCols   = 12;
  const int      kMatrixPixels = kMatrixRows * kMatrixCols;
  const int      kMatrixTiles  = 4;

  static Adafruit_NeoMatrix matrix(
    kMatrixCols, kMatrixRows, kMatrixTiles, 1, kMatrixPin,
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
  void draw_rgb_array(int const tile, RGB const *pixels) {
    int xOffset = tile * kMatrixCols;
    for (int x = 0; x < kMatrixCols; x++) {
      for (int y = 0; y < kMatrixRows ; y++) {
        uint16_t color = matrix.Color(pixels->r, pixels->g, pixels->b);
        matrix.drawPixel(x + xOffset, y, color);
        pixels++;
      }
    }
  }

  inline RGB const *image_at(size_t index) {
    index = index % kImageCount;
    return kImages[index];
  }
}

void setup() {
  matrix.begin();
  matrix.show();
}

void loop() {
  for (int i = 0; i < kImageCount; i++) {
    for (int j = 0; j < kMatrixTiles; j++) {
      draw_rgb_array(j, image_at(i + j));  
    }
    matrix.show();
    delay(3000);

    // transition to the next image
  }
}
