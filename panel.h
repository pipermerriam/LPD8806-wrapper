#include "wrapper.h"

class Panel {
  public:
    Panel(uint8_t strip_count, StripWrapper * w);
    int
      columns(void),
      rows(void);
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
      setPixelColor(uint16_t x, uint8_t y, uint32_t c),
      setPixelAverage(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b),
      setPixelAverage(uint16_t x, uint8_t y, uint32_t c),
      color_to_hsl(uint32_t color, float * h, float * s, float * l),
      rgb_to_hsl(uint8_t r, uint8_t g, uint8_t b, float * h, float * s, float * l),
      hsl_to_rgb(float h, float s, float l, uint8_t * red, uint8_t * green, uint8_t * blue);
    double
      hue2rgb(double p, double q, float t);
    uint8_t
      column_to_wrapper(uint16_t x),
      get_wrapper_column(uint8_t index, uint16_t x);
    uint32_t
      hsl_to_color(float h, float s, float l),
      getPixelColor(uint16_t x, uint8_t y),
      Color(byte, byte, byte);
  private:
    StripWrapper * wrappers;
    int
      wrapper_count,
      x_size,
      y_size;
};
