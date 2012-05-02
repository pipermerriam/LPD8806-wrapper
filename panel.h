#include "wrapper.h"

class Panel {
  public:
    Panel(uint8_t c, StripWrapper * w);
    int
      columns(void);
    void
      begin(void),
      show(void),
      setBoxColor(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom, uint8_t r, uint8_t g, uint8_t b),
      setBoxColor(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom, uint32_t c),
      setRowColor(uint8_t y, uint8_t r, uint8_t g, uint8_t b),
      setRowColor(uint8_t y, uint32_t c),
      setColumnColor(uint16_t x, uint8_t r, uint8_t g, uint8_t b),
      setColumnColor(uint16_t x, uint32_t c),
      setPixelColor(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b),
      setPixelColor(uint16_t x, uint8_t y, uint32_t c);
    uint16_t 
      column_to_wrapper(uint16_t x);
    uint32_t
      Color(byte, byte, byte);
  private:
    StripWrapper * wrappers;
    int
      wrapper_count,
      column_count;
    //int * column_starts; // eas: not at all sure this will work
     int column_starts[6];
};
