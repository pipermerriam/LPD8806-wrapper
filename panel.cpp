#include <SPI.h>
#include "panel.h"

#define DEFAULT_STEPS 64

/*****************************************************************************/
Panel::Panel(uint8_t c, StripWrapper * w) {
  wrapper_count = c;
  wrappers = w;
  y_size = w[0].num_rows();
  x_size = 0;
  for (uint8_t i=0; i < wrapper_count; i++)
  {
    x_size += wrappers[i].num_columns();
  }
}


/*
 *  UTILITY METHODS
 */
uint32_t Panel::Color(byte r, byte g, byte b) {
  // Take the lowest 7 bits of each value and append them end to end
  // We have the top bit set high (its a 'parity-like' bit in the protocol
  // and must be set!)
  return 0x808080 | ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
}

uint8_t Panel::column_to_wrapper(uint16_t x) {
  // Takes a cartesian x, y coordinate (eas:???) and returns the strip wrapper which
  // contains that corrdinate
  uint16_t i, column = 0;
  for (i=0; i < wrapper_count; i++)
  {
    column += wrappers[i].num_columns();
    if ( x < column )
    {
      return i;
    }
  }
  // TODO bounds checking
  return i;
}

uint8_t Panel::get_wrapper_column(uint8_t index, uint16_t x) {
  for (uint8_t i=0; i < index; i++)
  {
   x -= wrappers[i].num_columns();
  }
  return x;
}


/*
 *   BEGIN AND SHOW
 */
void Panel::begin(void) {
  for(int i=0; i < wrapper_count; i++)
  {
    wrappers[i].begin();
  }
}

void Panel::show(void) {
  for(int i=0; i < wrapper_count; i++)
  {
    wrappers[i].show();
  }
}

/*
 *  INFO METHODS
 */
int Panel::columns(void) {
  return x_size;
}

int Panel::rows(void) {
  return y_size;
}

/*
 *  BOX SETTER
 */
void Panel::setBoxColor(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom, uint8_t r, uint8_t g, uint8_t b) {
  setBoxColor(left, right, top, bottom, Color(r, g, b));
}

void Panel::setBoxColor(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom, uint32_t c) {
  ///Colors a box bounded by coordinates left, right, top, bottom (inclusive)
  for (uint16_t x=left; x <= right; x++)
  {
    for (uint8_t y=bottom; y <= top; y++)
    {
      setPixelColor(x, y, c);
    }
  }
}

/*
 *   COLUMN AND ROW SETTERS
 */
void Panel::setColumnColor(uint16_t x, uint8_t r, uint8_t g, uint8_t b) {
  setColumnColor(x, Color(r, g, b));
}

void Panel::setColumnColor(uint16_t x, uint32_t c) {
  uint8_t index = column_to_wrapper(x);            // First, figure out which wrapper 
  uint8_t strip_x = get_wrapper_column(index, x);  // Next, figure out index into that wrapper
  wrappers[index].setColumnColor(strip_x, c);
}

void Panel::setRowColor(uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setRowColor(y, Color(r, g, b));
}

void Panel::setRowColor(uint8_t y, uint32_t c) {
  for(uint8_t i=0; i < wrapper_count; i++)
  {
    wrappers[i].setRowColor(y, c);
  }
}
/*
 *   PIXEL GETTER
 */
uint32_t Panel::getPixelColor(uint16_t x, uint8_t y) {
  uint8_t index = column_to_wrapper(x);
  uint8_t strip_x = get_wrapper_column(index, x);
  wrappers[index].getPixelColor(strip_x, y);
}

/*
 *   PIXEL SETTERS
 */
void Panel::setPixelColor(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setPixelColor(x, y, Color(r, g, b));
}

void Panel::setPixelColor(uint16_t x, uint8_t y, uint32_t c) {
  uint8_t index = column_to_wrapper(x);
  uint8_t strip_x = get_wrapper_column(index, x);
  wrappers[index].setPixelColor(strip_x, y , c);
}

void Panel::setPixelAverage(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setPixelAverage(x, y, Color(r, g, b));
}

// TODO:  Make these use HSL color averages instead.
void Panel::setPixelAverage(uint16_t x, uint8_t y, uint32_t c) {
  uint8_t index = column_to_wrapper(x);
  uint8_t strip_x = get_wrapper_column(index, x);
  wrappers[index].setPixelAverage(strip_x, y , c);
}

/*
 *  Wrapper for rgb_to_hsl to allow color inputs.
 */
void Panel::color_to_hsl(uint32_t color, float * h, float * s, float * l) {
  // Don't forget GRB color order
  uint8_t g = color >> 16 & 0x7f;
  uint8_t r = color >> 8 & 0x7f;
  uint8_t b = color & 0x7f;

  rgb_to_hsl(r, g, b, h, s, l);
}

/*
 * Color Averaging
 *
 * Uses the HSL color to compute the appropriate average of two colors.  Called
 * with only two colors it will compute the true average of the two.  Given a
 * step value and a total value, the function will compute a weighted average.
 *
 * # Example color_average(0x7f0000, 0x000000, 2, 8)
 *
 * This would compute the average between full green and all off, giving the
 * green value a weight of 6, and the off value a weight of 2.
 *
 * The weighted average allows you to step through color fades in a very
 * controlled manner.
 *
 */
uint32_t Panel::color_average(uint32_t current, uint32_t target) {
  color_average(current, target, 1, 2);
}

uint32_t Panel::color_average(uint32_t current, uint32_t target, uint32_t step) {
  color_average(current, target, step, DEFAULT_STEPS);
}

uint32_t Panel::color_average(uint32_t current, uint32_t target, uint16_t step, uint16_t total) {
  float current_h, current_s, current_l;
  float target_h, target_s, target_l;

  color_to_hsl(current, &current_h, &current_s, &current_l);
  color_to_hsl(target, &target_h, &target_s, &target_l);

  current_h += (target_h - current_h)/(total-step);
  current_s += (target_s - current_s)/(total-step);
  current_l += (target_l - current_l)/(total-step);

  return hsl_to_color(current_h, current_s, current_l);
}

//
//  Color Converters
//

/*
 * From http://axonflux.com/handy-rgb-to-hsl-and-rgb-to-hsv-color-model-c
 *
 * Converts rgb color values to hsl color values.
 * http://en.wikipedia.org/wiki/HSL_and_HSV
 */
//void Panel::rgb_to_hsl(float red, float green, float blue, float * h, float * s, float * l) {
void Panel::rgb_to_hsl(uint8_t r, uint8_t g, uint8_t b, float * h, float * s, float * l) {
  float red = r / 127.0;
  float green = g / 127.0;
  float blue = b / 127.0;
  
  float maximum = max(max(red, green), blue);
  float minimum = min(min(red, green), blue);
  float hue, saturation, lightness = (maximum + minimum) / 2.0;
  // TODO: Clean this function up to use all pointers for the hsl values.
  // *h, *s, *l = (maximum + minimum) / 2.0;

  if ( maximum == minimum )
    hue = saturation = 0;
  else
  {
    float d = maximum - minimum;
    if ( lightness > 0.5 )
      saturation = d / (2 - maximum - minimum);
    else
      saturation = d / (maximum + minimum);

    if (maximum == red)
    {
      hue = (green - blue) / d;
      if ( green < blue )
        hue += 6;
    }
    else if (maximum == green)
      hue = (blue - red) / d + 2;
    else if (maximum == blue)
      hue = (red - green) / d + 4;

    hue /= 6.0;
  }
  *h = hue;
  *s = saturation;
  *l = lightness;
}


/*
 *  Wrapper for hsl_to_rgb to return a full color;
 */
uint32_t Panel::hsl_to_color(float h, float s, float l) {
  uint8_t r, g, b;

  hsl_to_rgb(h, s, l, &r, &g, &b);

  return Color(r, g, b);
}

/*
 * from https://github.com/ratkins/RGBConverter/
 *
 * Converts a color in HSL to RGB;
 *
 * TODO:  Check if we need doubles here.
 */
void Panel::hsl_to_rgb(float h, float s, float l, uint8_t * red, uint8_t * green, uint8_t * blue) {
  double r, g, b;

  if (s == 0) {
      r = g = b = l; // achromatic
  } else {
      double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
      double p = 2 * l - q;
      r = Panel::hue2rgb(p, q, h + 1/3.0);
      g = Panel::hue2rgb(p, q, h);
      b = Panel::hue2rgb(p, q, h - 1/3.0);
  }

  *red = r * 127;
  *green = g * 127;
  *blue = b * 127;
}

// TODO:  Do we need doubles here? save some memory....
double Panel::hue2rgb(double p, double q, float t) {
  if(t < 0) t += 1;
  if(t > 1) t -= 1;
  if(t < 1/6.0) return p + (q - p) * 6 * t;
  if(t < 1/2.0) return q;
  if(t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6;
  return p;
}
