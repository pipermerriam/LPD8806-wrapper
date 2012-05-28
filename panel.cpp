#include <SPI.h>
#include "panel.h"

#define DEFAULT_STEPS 128
#define SMOOTH_COLOR_SCALE 120.0

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

// Wrapper around confusingly named hsl_to_color
uint32_t Panel::Color(double h, double s, double l) {
  return hsl_to_color(h, s, l);
}

// Turns a 32 bit color value into its RGB components.
void Panel::RGB(uint32_t color, uint8_t * r, uint8_t * g, uint8_t * b) {
  *g = color >> 16 & 0x7f;
  *r = color >> 8 & 0x7f;
  *b = color & 0x7f;
}

// Wrapper around the hsl_to_rgb
void Panel::RGB(double h, double s, double l, uint8_t * red, uint8_t * green, uint8_t * blue) {
  hsl_to_rgb(h, s, l, red, green, blue);
}

uint32_t Panel::SmoothColor(int x, int y){
  return SmoothColor(x, y, 0, 0, 0);
}

uint32_t Panel::SmoothColor(int x, int y, long x_offset, long y_offset, double z_offset){
  double z = (sin(sqrt(2) * (x + x_offset) / SMOOTH_COLOR_SCALE) - sin(M_PI * (x + x_offset) / SMOOTH_COLOR_SCALE) + sin(sqrt(3) * (y + y_offset) / SMOOTH_COLOR_SCALE) - sin(M_E * (y + y_offset) / SMOOTH_COLOR_SCALE)) + z_offset;
  double h = z - (int) z;
  return hsl_to_color(h, 1.0, 0.5);
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

// Returns a hash signature of the current pixel colors on the board.
uint32_t Panel::image_hash(void) {
  uint32_t hash = 0x000000;
  for(int x=0; x<columns(); x++)
  {
    for(int y=0; y<rows(); y++)
    {
      hash ^= getPixelColor(x, y);
    }
  }
  return hash;
}

/*
 *  Image filters
 */

// Darkens the entire board by a fixed amount (1/200)
void Panel::darken() {
  darken(0.005);
}

// Darkens the entire board by whatever step value you provide.
void Panel::darken(double step) {
  for(int x=0; x < columns(); x++)
  {
    for(int y=0; y < rows(); y++)
    {
      double h,s,l;
      long current_color = getPixelColor(x,y); 
      color_to_hsl(current_color, &h, &s, &l);
                  
      l -= step;
      constrain(l,0,1.0);
      setPixelColor(x, y, hsl_to_color(h,s,l));
    }
  }
}

// Lighten the entire board by a fixed amount (1/200)
void Panel::lighten() {
  lighten(0.005);
}

// Lighten the entire board by whatever step value you provide.
void Panel::lighten(double step) {
  for(int x=0; x < columns(); x++)
  {
    for(int y=0; y < rows(); y++)
    {
      double h,s,l;
      long current_color = getPixelColor(x,y); 
      color_to_hsl(current_color, &h, &s, &l);
                  
      l += step;
      constrain(l,0,1.0);
      setPixelColor(x, y, hsl_to_color(h,s,l));
    }
  }
}

/*
 * Shifters
 */

// Shifts all pixels on the board left one pixel.
void Panel::shiftLeft() {
  for(int x=0; x < columns() - 1; x++)
  {
    for(int y=0; y < rows(); y++)
    {
      setPixelColor(x, y, getPixelColor(x + 1, y));
    }
  }
}

// Shifts all pixels on the board right one pixel.
void Panel::shiftRight() {
  for(int x=columns() - 1; x > 0; x--)
  {
    for(int y=0; y < columns() - 1; y++)
    {
      setPixelColor(x, y, getPixelColor(x - 1, y));
    }
  }
}

// Shifts all pixels on the board down one pixel.
void Panel::shiftDown() {
  for(int y=0; y < rows() - 1; y++)
  {
    for(int x=0; x < columns(); x++)
    {
      setPixelColor(x, y, getPixelColor(x, y + 1));
    }
  }
}

// Shifts all pixels on the board up one pixel.
void Panel::shiftUp() {
  for(int y=rows() - 1; y > 0; y--)
  {
    for(int x=0; x < columns(); x++)
    {
      setPixelColor(x, y, getPixelColor(x, y - 1));
    }
  }
}

/*
 *   COLUMN AND ROW SETTERS
 */
void Panel::setColumnColor(uint16_t x, uint8_t r, uint8_t g, uint8_t b) {
  setColumnColor(x, Color(r, g, b));
}

void Panel::setColumnColor(uint16_t x, uint32_t color) {
  uint8_t index = column_to_wrapper(x);            // First, figure out which wrapper 
  uint8_t strip_x = get_wrapper_column(index, x);  // Next, figure out index into that wrapper
  wrappers[index].setColumnColor(strip_x, color);
}

void Panel::setRowColor(uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setRowColor(y, Color(r, g, b));
}

void Panel::setRowColor(uint8_t y, uint32_t color) {
  for(uint8_t i=0; i < wrapper_count; i++)
  {
    wrappers[i].setRowColor(y, color);
  }
}
/*
 *   PIXEL GETTER
 */
uint32_t Panel::getPixelColor(uint16_t x, uint8_t y) {
  uint8_t index = column_to_wrapper(x);
  if ( index >= wrapper_count )
    return 0x000000;
  uint8_t strip_x = get_wrapper_column(index, x);
  wrappers[index].getPixelColor(strip_x, y);
}

/*
 *   PIXEL SETTERS
 */
void Panel::setPixelColor(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setPixelColor(x, y, Color(r, g, b));
}

void Panel::setPixelColor(uint16_t x, uint8_t y, uint32_t color) {
  uint8_t index = column_to_wrapper(x);
  if ( index >= wrapper_count )
    return;
  uint8_t strip_x = get_wrapper_column(index, x);
  wrappers[index].setPixelColor(strip_x, y , color);
}

void Panel::setPixelAverage(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setPixelAverage(x, y, Color(r, g, b));
}

/*
 *  Pixel Average Setters.
 *
 *  Params:
 *    x - The x coordinate of the pixel to set
 *    y - The y coordinate of the pixel to set
 *    color - The color we are 'averaging' towards.
 *    step - The step number we are on in our averaging.
 *    total - The total number of steps we are using to get to our new color.
 *    direction - The hard coded direction around the color wheel we should take.
 *      `true` -> Travel up, or clockwise around the HSL color wheel
 *      `false` -> Travel down, or counter-clockwise around the HSL color wheel.
 *
 *    These methods allow you to fade smoothly from the current color on the
 *    board to whatever new color you'd like across a fixed number of steps.
 *    Called without the `direction` parameter it will take the shortest path
 *    around the color wheel.
 */
void Panel::setPixelAverage(uint16_t x, uint8_t y, uint32_t color) {
  uint32_t current = getPixelColor(x, y);
  setPixelColor(x, y , color_average(current, color, 1, 2));
}

void Panel::setPixelAverage(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, uint16_t step) {
  setPixelAverage(x, y, Color(r, g, b), step);
}

void Panel::setPixelAverage(uint16_t x, uint8_t y, uint32_t color, uint16_t step) {
  setPixelAverage(x, y, color, step, DEFAULT_STEPS);
}

void Panel::setPixelAverage(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b, uint16_t step, uint16_t total) {
  setPixelAverage(x, y, Color(r, g, b), step, total);
}

void Panel::setPixelAverage(uint16_t x, uint8_t y, uint32_t color, uint16_t step, uint16_t total) {
  setPixelColor(x, y, color_average(getPixelColor(x, y), color, step, total));
}

// Allows you to enforce which direction you will travel around the color wheel.
void Panel::setPixelAverage(uint16_t x, uint8_t y, uint32_t color, uint16_t step, uint16_t total, bool direction) {
  setPixelColor(x, y, color_average(getPixelColor(x, y), color, step, total, direction));
}


/*
 *  Returns a random color.
 */

//  Large range of random colors using HSL, constrained to being above 90%
//  brightness and almost full saturation.
uint32_t Panel::random_color(void) {
  double h = random(0, 1000) / 1000.0;
  double s = random(900, 1000) / 1000.0;
  double l = random(450, 550) / 1000.0;

  return hsl_to_color(h, s, l);
}

// Returns a random Primary color
uint32_t Panel::random_primary(void) {
  int i = random(0, 6);

  switch (i)
  {
    case 0:
      return 0x00f700; // Red
    case 1:
      return 0x207f00; // Orange
    case 2:
      return 0x7f7f00; // Yellow
    case 3:
      return 0x7f0000; // Green
    case 4:
      return 0x00007f; // Blue
    case 5:
      return 0x10407f; // Purple
  }
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

uint32_t Panel::color_average(uint32_t current, uint32_t target, int step) {
  color_average(current, target, step, DEFAULT_STEPS);
}

// This method merely determines the shortest direction to fade the color and instructs the fade function to go that direction.
uint32_t Panel::color_average(uint32_t current, uint32_t target, int step, int total) {
  double current_h, current_s, current_l;
  double target_h, target_s, target_l;

  color_to_hsl(current, &current_h, &current_s, &current_l);
  color_to_hsl(target, &target_h, &target_s, &target_l);

  double diff = current - target;

  bool direction = ((diff > 0 and diff < 0.5) or (diff < 0 and diff > 0.5));
  return color_average(current, target, step, total, direction);
}

uint32_t Panel::color_average(uint32_t current, uint32_t target, int step, int total, bool direction) {
  double current_h, current_s, current_l;
  double target_h, target_s, target_l;

  // Ensure that we are within our step values
  step = constrain(step, 0, total);

  color_to_hsl(current, &current_h, &current_s, &current_l);
  color_to_hsl(target, &target_h, &target_s, &target_l);

  double h_step = abs(target_h - current_h);

  if ( !direction )
    h_step *= -1;

  // Ensure that we arn't on the last step, at which point we should just go to
  // the target
  if (step == total)
  {
    current_h = target_h;
    current_s = target_s;
    current_l = target_l;
  }
  else if (step == 0)
  {
    current_h;
  }
  else
  {
    current_h += h_step / (total - step);
    current_s += (target_s - current_s)/(total-step);
    current_l += (target_l - current_l)/(total-step);
  }

  constrain(current_s, 0.9, 1.0);
  constrain(current_l, 0.45, 0.55);

  // Ensure that our h-value is within bounds
  if (current_h > 1.0)
    current_h -= 1;
  else if (current_h < 0)
    current_h += 1;

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
void Panel::rgb_to_hsl(uint8_t r, uint8_t g, uint8_t b, double * h, double * s, double * l) {
  double red = r / 127.0;
  double green = g / 127.0;
  double blue = b / 127.0;
  
  double maximum = max(max(red, green), blue);
  double minimum = min(min(red, green), blue);
  double hue, saturation, lightness = (maximum + minimum) / 2.0;
  // TODO: Clean this function up to use all pointers for the hsl values.
  // *h, *s, *l = (maximum + minimum) / 2.0;

  if ( maximum == minimum )
    hue = saturation = 0;
  else
  {
    double d = maximum - minimum;
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
 *  Wrapper for rgb_to_hsl to allow color inputs.
 */
void Panel::color_to_hsl(uint32_t color, double * h, double * s, double * l) {
  // Don't forget GRB color order
  uint8_t g = color >> 16 & 0x7f;
  uint8_t r = color >> 8 & 0x7f;
  uint8_t b = color & 0x7f;

  rgb_to_hsl(r, g, b, h, s, l);
}

/*
 *  Wrapper for hsl_to_rgb to return a full color;
 */
uint32_t Panel::hsl_to_color(double h, double s, double l) {
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
void Panel::hsl_to_rgb(double h, double s, double l, uint8_t * red, uint8_t * green, uint8_t * blue) {
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

  *red = constrain(r * 127, 0, 127);
  *green = constrain(g * 127, 0, 127);
  *blue = constrain(b * 127, 0, 127);
}

// Helper method for HSL <-> RGB conversion
double Panel::hue2rgb(double p, double q, double t) {
  if(t < 0) t += 1;
  if(t > 1) t -= 1;
  if(t < 1/6.0) return p + (q - p) * 6 * t;
  if(t < 1/2.0) return q;
  if(t < 2/3.0) return p + (q - p) * (2/3.0 - t) * 6;
  return p;
}
