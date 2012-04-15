#include "LPD8806.h"
#include <Arduino.h>

#define COLUMN_SIZE 26
#define ROW_COUNT 6
#define STRIP_COUNT 1

class StripWrapper {
    
    public:
      StripWrapper(uint8_t r, uint8_t c, LPD8806 * s);
      int 
        cartesian_to_pixel(uint16_t x, uint16_t y),
        columns(void),
        pixels(void);
      void
        begin(void),
        show(void),
        setRowColor(uint8_t y, uint8_t r, uint8_t g, uint8_t b),
        setRowColor(uint8_t y, uint32_t c),
        setColumnColor(uint16_t x, uint8_t r, uint8_t g, uint8_t b),
        setColumnColor(uint16_t x, uint32_t c),
        setPixelColor(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b),
        setPixelColor(uint16_t x, uint8_t y, uint32_t c);
      uint32_t
        Color(byte, byte, byte);
    private:
      LPD8806 * strip;
      int 
        row_count, 
        column_count;
};
