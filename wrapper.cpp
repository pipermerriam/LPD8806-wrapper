#include "wrapper.h"

/*****************************************************************************/

/*
 *  CONSTRUCTOR
 */
StripWrapper::StripWrapper(uint8_t r_size, uint8_t c_size, LPD8806 * s)
: row_size (r_size),
  column_size (c_size),
  strip (s) 
  { } 
/*
 *  UTILITY METHODS
 */
uint32_t StripWrapper::Color(const byte r, const byte g, const byte b) const {
  // Take the lowest 7 bits of each value and append them end to end
  // We have the top bit set high (its a 'parity-like' bit in the protocol
  // and must be set!)
  return 0x808080 | ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
}

int StripWrapper::cartesian_to_pixel(const uint16_t x, const uint16_t y) const {
    //bool reverse = x % 2;
    bool reverse = y % 2;

    if ( reverse ) {
      //return (x * row_size + (row_size - y - 1));
      return (y * column_size + (column_size - x - 1));
    }
    else {
      //return (x * row_size + y);
      return (y * column_size + x);
    }
}

/*
 *  BEGIN AND SHOW
 */
void StripWrapper::begin() {
    (*strip).begin();
}

void StripWrapper::show() {
    (*strip).show();
}

/*
 *  INFO METHODS
 */
int StripWrapper::num_columns() const {
  return row_size;
}

int StripWrapper::num_rows() const {
  return column_size;
}

int StripWrapper::num_pixels() const {
  return column_size * row_size;
}

/*
 *  COLUMN AND ROW SETTERS
 */
void StripWrapper::setColumnColor(const uint16_t y, const uint8_t r, const uint8_t g, const uint8_t b) {
  setColumnColor(y, Color(r, g, b));
}

void StripWrapper::setColumnColor(const uint16_t y, const uint32_t c) {
  for (int x=0; x < column_size; x++)
  {
    setPixelColor(x, y, c);
  }
}

void StripWrapper::setRowColor(const uint8_t x, const uint8_t r, const uint8_t g, const uint8_t b) {
  setRowColor(x, Color(r, g, b));
}

void StripWrapper::setRowColor(const uint8_t x, const uint32_t c) {
  for (int y=0; y < row_size; y++)
  {
    setPixelColor(x, y, c);
  }
}

/*
 *  PIXEL SETTERS
 */
void StripWrapper::setPixelColor(const uint16_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b) {
  setPixelColor(x, y, Color(r, g, b));
}

void StripWrapper::setPixelColor(const uint16_t x, const uint8_t y, const uint32_t color) {
  (*strip).setPixelColor(cartesian_to_pixel(x, y), color);
}

/* eas: find a more efficient way to do this */
void StripWrapper::clearallpixels() {
  for (int i=0; i < row_size; i++)
      setColumnColor(i, 0, 0, 0);

}
