#include "wrapper.h"

/*****************************************************************************/

/*
 *  CONSTRUCTOR
 */
StripWrapper::StripWrapper(uint8_t r, uint8_t c, LPD8806 * s) {
  row_count = r;
  column_count = c;
  strip = s;
}

/*
 *  UTILITY METHODS
 */
uint32_t StripWrapper::Color(byte r, byte g, byte b) {
  // Take the lowest 7 bits of each value and append them end to end
  // We have the top bit set high (its a 'parity-like' bit in the protocol
  // and must be set!)
  return 0x808080 | ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
}

int StripWrapper::cartesian_to_pixel(uint16_t x, uint16_t y) {
    bool reverse = x % 2;

    if ( reverse ) {
      return (x * row_count + (row_count - y - 1));
    }
    else {
      return (x * row_count + y);
    }
}

/*
 *  BEGIN AND SHOW
 */
void StripWrapper::begin(void) {
    (*strip).begin();
}

void StripWrapper::show(void) {
    (*strip).show();
}

/*
 *  INFO METHODS
 */
int StripWrapper::columns(void) {
  return column_count;
}

int StripWrapper::rows(void) {
  return row_count;
}

int StripWrapper::pixels(void) {
  return column_count * row_count;
}

/*
 *  COLUMN AND ROW SETTERS
 */
void StripWrapper::setColumnColor(uint16_t x, uint8_t r, uint8_t g, uint8_t b) {
  setColumnColor(x, Color(r, g, b));
}

void StripWrapper::setColumnColor(uint16_t x, uint32_t c) {
  for (int y=0; y < row_count; y++)
  {
    setPixelColor(x, y, c);
  }
}

void StripWrapper::setRowColor(uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setRowColor(y, Color(r, g, b));
}

void StripWrapper::setRowColor(uint8_t y, uint32_t c) {
  for (int x=0; x < column_count; x++)
  {
    setPixelColor(x, y, c);
  }
}

/*
 *  PIXEL SETTERS
 */
void StripWrapper::setPixelColor(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setPixelColor(x, y, Color(r, g, b));
}

void StripWrapper::setPixelColor(uint16_t x, uint8_t y, uint32_t c) {
  (*strip).setPixelColor(cartesian_to_pixel(x, y), c);
}
