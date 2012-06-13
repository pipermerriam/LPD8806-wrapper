#include "SPI.h"
#include "panel.h"

/*  
 *  This example code smoothly fades the board between a random field of
 *  smoothly faded colors.
 */

// The row and column size for the strip we'll be using.
#define ROW_SIZE 6
#define COLUMN_SIZE 26

// Defines how many frames will be used to transition between colors.  Larger
// numbers will produce slower and smoother transitions
#define FADE_FRAMES 64

// Define a strip wrapper, this one a has 6 columns of 26 pixels.  These would
// be wired in a zig-zag pattern which the wrapper and panel classes abstract
// into a 2d coordinate plane.
StripWrapper wrapper1(ROW_SIZE, COLUMN_SIZE, 13, 83);
StripWrapper wrapper2(ROW_SIZE, COLUMN_SIZE, 12, 82);
StripWrapper wrapper3(ROW_SIZE, COLUMN_SIZE, 10, 80);
StripWrapper wrapper4(ROW_SIZE+1, COLUMN_SIZE, 8, 78);
StripWrapper wrapper5(ROW_SIZE, COLUMN_SIZE, 22, 23);
StripWrapper wrapper6(ROW_SIZE, COLUMN_SIZE, 30, 31);
StripWrapper wrapper7(ROW_SIZE, COLUMN_SIZE, 34, 35);
StripWrapper wrapper8(ROW_SIZE+1, COLUMN_SIZE, 38, 39);
StripWrapper wrapper9(ROW_SIZE, COLUMN_SIZE, 42, 43);
StripWrapper wrapper10(ROW_SIZE, COLUMN_SIZE, 46, 47);
StripWrapper wrapper11(ROW_SIZE, COLUMN_SIZE, 50, 51);
StripWrapper wrapper12(ROW_SIZE+1, COLUMN_SIZE, 52, 53);

// Initialize an array of wrappers (singular in this example) for use in
// initializing our panel class.
StripWrapper wrappers[] = {wrapper1, wrapper2, wrapper3, wrapper4, wrapper5, wrapper6, wrapper7, wrapper8, wrapper9, wrapper10, wrapper11, wrapper12};

// Initialize our panel class which is capable of abstracting multiple wrapper
// classes in such a way that you can write to them as a single coordinate
// plane.
Panel panel(12, wrappers);

// ON AND OFF CONSTANTS
#define ALL_OFF 0x808080
#define ALL_ON 0x7f7f7f

// PRIMARY COLORS
// color order is GRB, not RGB
#define RED 0x80f780
#define ORANGE 0x207f80
#define YELLOW 0x7f7f80
#define GREEN 0x7f8080
#define BLUE 0x80807f
#define CYAN 0x7f807f
#define VIOLET 0x807f7f
#define PURPLE 0x10407f

// PIMARY COLORED RAINBOW
uint8_t STRIPES[6] = {5, 4, 4, 4, 4, 5};
prog_uint32_t RAINBOW[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};

void setup() {
  Serial.begin(9600);
  panel.begin();
  panel.show();
  // This initializes the color field with a suedo random field of color.
  // Otherwise there is some strangeness as it fades into the first frame.
}


// Each loop generates a new suedo random frame of color and then fades the
// panel to that frame.
void loop() {
  // seed, step
  static long x = 0;
  static long y = 0;

  long target_x = random(x - 500, x + 500);
  long target_y = random(y - 500, y + 500);

  while (true)
  {
    long diff_x = target_x - x;
    long diff_y = target_y - y;

    if( diff_x and diff_y )
    {
      bool move_x = random(0, 2);
      if( x%2 and y%2 )
        x += diff_x / abs(diff_x);
      else
        y += diff_y / abs(diff_y);
    }
    else if( diff_x )
      x += diff_x / abs(diff_x);
    else if( diff_y )
      y += diff_y / abs(diff_y);
    else
      break;
    color_gradient(x, y, FADE_FRAMES);
    delay(25);
  }
}

/*
 *  Outputs a random spread of color onto the entire panel based off of a
 *  random seed.  Given the same random seed, the exact same field of color
 *  will be generated.  The step indicates which step in the fade-transition we
 *  are in.  Set this value to FADE_FRAMES - 1 to go strait to the new color
 *  field.
 */
void color_gradient(long x_offset, long y_offset, int step) {
  for(int x = 0; x < panel.columns(); x++)
  {
    for(int y = 0; y < panel.rows(); y++)
    {
      panel.setPixelAverage(x, y, panel.SmoothColor(x, y, x_offset, y_offset, 0), step, FADE_FRAMES);
    }
  }
  panel.show();
}

/*
 *  Steps through all pixels on the panel, setting them individually to a color
 */
void pixel_test(uint32_t color)
{
  for (uint8_t y=0; y < panel.rows(); y++)
  {
    for (uint8_t x=0; x < panel.columns() ; x++)
    {
      panel.setPixelColor(x, y, color);
      panel.show();
    }
  }
}


//  Generates suedo-random seeds for the random number generator by xor-ing all
//  of the pixels in the panel.
uint32_t hash_image() {
  uint32_t hash = 0;
  for(int x=0; x < panel.columns(); x++)
  {
    for(int y=0; y < panel.rows(); y++)
      hash ^= panel.getPixelColor(x, y);
  }
  return hash;
}

void column_gradient(int column, uint32_t start_color, uint32_t end_color, bool direction) {
  uint32_t current = start_color;

  panel.setPixelColor(column, 0, start_color);
  panel.setPixelColor(column, panel.rows() - 1, end_color);
  
  for(int y=1; y < panel.rows() - 1; y++)
  {
    current = panel.color_average(current, end_color, y, panel.rows() - 1, direction);
    panel.setPixelColor(column, y, current);
  }
  panel.show();
}

