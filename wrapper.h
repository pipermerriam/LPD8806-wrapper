#include "LPD8806.h"
#include <Arduino.h>

//#define COLUMN_SIZE 26
//#define ROW_SIZE 6 
//#define STRIP_COUNT 1

class StripWrapper {
  public:
    StripWrapper(uint8_t r_size, uint8_t c_size, LPD8806 * s);
    int 
      cartesian_to_pixel(const uint16_t x, const uint16_t y) const,
      num_columns() const,
      num_rows() const,
      num_pixels() const;
    void
      begin(),
      show(),
      setRowColor(const uint8_t x, const uint8_t r, const uint8_t g, const uint8_t b),
      setRowColor(const uint8_t x, const uint32_t c),
      setColumnColor(const uint16_t y, const uint8_t r, const uint8_t g, const uint8_t b),
      setColumnColor(const uint16_t y, const uint32_t c),
      setPixelColor(const uint16_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b),
      setPixelColor(const uint16_t x, const uint8_t y, const uint32_t color),
      clearallpixels();
    uint32_t
      Color(const byte r, const byte g, const byte b) const;
  private:
    LPD8806 * strip;
    int 
      row_size, 
      column_size;
};
