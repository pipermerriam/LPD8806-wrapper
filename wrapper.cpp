#include "wrapper.h"

/*****************************************************************************/

StripWrapper::StripWrapper(uint16_t c, uint16_t r, LPD8806 * s) {
  column_size = c;
  row_count = r;
  strip = s;

}

void StripWrapper::begin(void) {
    (*strip).begin();
}

void StripWrapper::show(void) {
    (*strip).show();
}

int StripWrapper::cartesian_to_pixel(uint16_t x, uint16_t y) {
    bool reverse = x % 2;

    if ( reverse ) {
      return (x * column_size + (column_size - y - 1));
    }
    else {
      return (x * column_size + y);
    }
}

void StripWrapper::setColumnColor(uint16_t x, uint8_t r, uint8_t g, uint8_t b) {
  setColumnColor(x, (*strip).Color(r, g, b));
}

void StripWrapper::setColumnColor(uint16_t x, uint32_t c) {
  for (int y=0; y < row_count; x++)
  {
    setPixelColor(x, y, c);
  }
}

void StripWrapper::setRowColor(uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setRowColor(y, (*strip).Color(r, g, b));
}

void StripWrapper::setRowColor(uint8_t y, uint32_t c) {
  for (int x=0; x < column_size; x++)
  {
    setPixelColor(x, y, c);
  }
}

void StripWrapper::setPixelColor(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setPixelColor(x, y, (*strip).Color(r, g, b));
}

void StripWrapper::setPixelColor(uint16_t x, uint8_t y, uint32_t c) {
  (*strip).setPixelColor(cartesian_to_pixel(x, y), c);
}
