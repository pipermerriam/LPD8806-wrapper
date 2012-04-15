#include "wrapper.h"

class Panel {
  public:
    Panel(uint8_t c, StripWrapper * w);
    void
      begin(void),
      show(void),
      setRowColor(uint8_t y, uint8_t r, uint8_t g, uint8_t b),
      setRowColor(uint8_t y, uint32_t c),
      setColumnColor(uint16_t x, uint8_t r, uint8_t g, uint8_t b),
      setColumnColor(uint16_t x, uint32_t c),
      setPixelColor(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b),
      setPixelColor(uint16_t x, uint8_t y, uint32_t c);
    uint16_t 
      column_to_strip(uint16_t x);
    uint32_t
      Color(byte, byte, byte);
  private:
    StripWrapper * wrappers;
    int
      strip_count;
    int * columns;
};
