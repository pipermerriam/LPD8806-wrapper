#ifndef STRIP_WRAPPER
#define STRIP_WRAPPER

#include "LPD8806.h"
#include <Arduino.h>

//#define COLUMN_SIZE 26
//#define ROW_SIZE 6 
//#define STRIP_COUNT 1

class StripWrapper : public LPD8806 {
  public:
    StripWrapper(uint8_t r_size, uint8_t c_size, uint8_t dpin, uint8_t cpin);
    int 
      cartesian_to_pixel(const uint16_t x, const uint16_t y) const,
      num_columns() const,
      num_rows() const,
      num_pixels() const;
    void
      setRowColor(const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b),
      setRowColor(const uint8_t y, const uint32_t c),
      setColumnColor(const uint16_t x, const uint8_t r, const uint8_t g, const uint8_t b),
      setColumnColor(const uint16_t x, const uint32_t c),
      setPixelColor(const uint16_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b),
      setPixelColor(const uint16_t x, const uint8_t y, const uint32_t color),
      setPixelAverage(const uint16_t x, const uint8_t y, const uint32_t color),
      setPixelAverage(const uint16_t x, const uint8_t y, const uint8_t r, const uint8_t g, const uint8_t b),
      clearallpixels();
  private:
    int 
      row_size, 
      column_size,
};
#endif
