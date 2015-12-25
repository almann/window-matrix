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

  inline uint16_t rgb_to_color(RGB const *pixel) {
    return matrix.Color(pixel->r, pixel->g, pixel->b);
  }

  // draws an RGB array assuming column-ordered, progressive flattened array
  void draw_rgb_array(int const tile, RGB const *pixels) {
    int xOffset = tile * kMatrixCols;
    for (int x = 0; x < kMatrixCols; x++) {
      for (int y = 0; y < kMatrixRows; y++) {
        matrix.drawPixel(x + xOffset, y, rgb_to_color(pixels));
        pixels++;
      }
    }
  }

  // draws a the old image shifted offset left with a new image in the shifted position 
  void draw_left_marquee_rgb_array(int const tile, int offset, RGB const *left, RGB const *right) {
    int xOffset = tile * kMatrixCols;
    // left image
    left += offset * kMatrixRows;
    for (int x = 0; x < kMatrixCols - offset; x++) {
      for (int y = 0; y < kMatrixRows; y++) {
        matrix.drawPixel(x + xOffset, y, rgb_to_color(left));
        left++;
      }
    }
    // right image
    for (int x = kMatrixCols - offset; x < kMatrixCols; x++) {
      for (int y = 0; y < kMatrixRows; y++) {
        matrix.drawPixel(x + xOffset, y, rgb_to_color(right));
        right++;
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
    delay(2000);

    // transition to the next image
    for (int offset = 0; offset < kMatrixCols; offset++) {
      for (int j = 0; j < kMatrixTiles; j++) {
        draw_left_marquee_rgb_array(j, offset, image_at(i + j), image_at(i + j + 1));
      }
      matrix.show();
      delay(100);
    }
  }
}
