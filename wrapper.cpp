#include "wrapper.h"

/*****************************************************************************/

/*
 *  CONSTRUCTOR
 */
StripWrapper::StripWrapper(uint8_t r_size, uint8_t c_size, uint8_t dpin, uint8_t cpin)
  : LPD8806(r_size * c_size, dpin, cpin)
  { 
    column_size = c_size;
    row_size = r_size;
  } 

/*
 *  UTILITY METHODS
 */
int StripWrapper::cartesian_to_pixel(const uint16_t x, const uint16_t y) const {
    bool reverse = x % 2;

    if ( reverse ) {
      return (x * column_size + (column_size - y - 1));
    }
    else {
      return (x * column_size + y);
    }
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
void StripWrapper::setColumnColor(const uint16_t x, const uint8_t r, const uint8_t g, const uint8_t b) {
  setColumnColor(x, LPD8806::Color(r, g, b));
}

void StripWrapper::setColumnColor(const uint16_t x, const uint32_t c) {
  for (int y=0; y < column_size; y++)
  {
    setPixelColor(x, y, c);
  }
}

void StripWrapper::setRowColor(const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b) {
  setRowColor(y, LPD8806::Color(r, g, b));
}

void StripWrapper::setRowColor(const uint8_t y, const uint32_t c) {
  for (int x=0; x < row_size; x++)
  {
    setPixelColor(x, y, c);
  }
}

/*
 *  PIXEL SETTERS
 */
void StripWrapper::setPixelColor(const uint16_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b) {
  setPixelColor(x, y, LPD8806::Color(r, g, b));
}

void StripWrapper::setPixelColor(const uint16_t x, const uint8_t y, const uint32_t color) {
  LPD8806::setPixelColor(cartesian_to_pixel(x, y), color);
}

void StripWrapper::setPixelAverage(const uint16_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b) {
  setPixelAverage(x, y, LPD8806::Color(r, g, b));
}

void StripWrapper::setPixelAverage(const uint16_t x, const uint8_t y, const uint32_t color) {
  int index = cartesian_to_pixel(x, y); 

  uint32_t current = LPD8806::getPixelColor(index);

  uint8_t g = ((color >> 16 & 0x7f) + (current >> 16 & 0x7f)) / 2;
  uint8_t r = ((color >> 8 & 0x7f) + (current >> 8 & 0x7f)) / 2;
  uint8_t b = ((color & 0x7f) + (current & 0x7f)) / 2;

  LPD8806::setPixelColor(index, r, g, b);
}


void StripWrapper::clearallpixels() {
  for (int i=0; i < LPD8806::numPixels() ; i++)
      LPD8806::setPixelColor(i, 0x000000);
}
