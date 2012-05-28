#include <p32xxxx.h>
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
      RGB(uint32_t color, uint8_t * r, uint8_t * g, uint8_t * b),
      shiftLeft(void),
      shiftRight(void),
      shiftDown(void),
      shiftUp(void),
      darken(void),
      darken(double step),
      setRowColor(uint8_t y, uint8_t r, uint8_t g, uint8_t b),
      setRowColor(uint8_t y, uint32_t color),
      setColumnColor(uint16_t x, uint8_t r, uint8_t g, uint8_t b),
      setColumnColor(uint16_t x, uint32_t color),
      setPixelColor(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b),
      setPixelColor(uint16_t x, uint8_t y, uint32_t color),
      setPixelAverage(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b),
      setPixelAverage(uint16_t x, uint8_t y, uint32_t color),
      setPixelAverage(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, uint16_t step),
      setPixelAverage(uint16_t x, uint8_t y, uint32_t color, uint16_t step),
      setPixelAverage(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, uint16_t step, uint16_t total),
      setPixelAverage(uint16_t x, uint8_t y, uint32_t color, uint16_t step, uint16_t total),
      setPixelAverage(uint16_t x, uint8_t y, uint32_t color, uint16_t step, uint16_t total, bool direction),
      color_to_hsl(uint32_t color, double * h, double * s, double * l),
      rgb_to_hsl(uint8_t r, uint8_t g, uint8_t b, double * h, double * s, double * l),
      hsl_to_rgb(double h, double s, double l, uint8_t * red, uint8_t * green, uint8_t * blue);
    double
      hue2rgb(double p, double q, double t);
    uint8_t
      column_to_wrapper(uint16_t x),
      get_wrapper_column(uint8_t index, uint16_t x);
    uint32_t
      color_average(uint32_t current, uint32_t target),
      color_average(uint32_t current, uint32_t target, int step),
      color_average(uint32_t current, uint32_t target, int step, int total),
      color_average(uint32_t current, uint32_t target, int step, int total, bool direction),
      random_color(void),
      random_primary(void),
      hsl_to_color(double h, double s, double l),
      getPixelColor(uint16_t x, uint8_t y),
      Color(double h, double s, double l),
      Color(byte, byte, byte),
      SmoothColor(int x, int y),
      SmoothColor(int x, int y, long x_offset, long y_offset, double z_offset);
  private:
    StripWrapper * wrappers;
    int
      wrapper_count,
      x_size,
      y_size;
};
