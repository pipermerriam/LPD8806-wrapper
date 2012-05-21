#include "SPI.h"
#include "panel.h"

/*  
 *  This example code smoothly fades the board between a random field of
 *  smoothly faded colors.
 */

// Data and Clock pins for the single LPD8806 strip.
#define DATA_PIN 8
#define CLOCK_PIN 9

// The row and column size for the strip we'll be using.
#define ROW_SIZE 6
#define COLUMN_SIZE 26

// Defines how many frames will be used to transition between colors.  Larger
// numbers will produce slower and smoother transitions
#define FADE_FRAMES 256

// Defines the number of pixels both horizontally and vertically that will be
// used to transition from one color to another.  Larger values will produce
// smoother fields of color.
#define BLOCK_WIDTH 5
#define BLOCK_HEIGHT 13

// Define a strip wrapper, this one a has 6 columns of 26 pixels.  These would
// be wired in a zig-zag pattern which the wrapper and panel classes abstract
// into a 2d coordinate plane.
StripWrapper wrapper(ROW_SIZE, COLUMN_SIZE, DATA_PIN, CLOCK_PIN);

// Initialize an array of wrappers (singular in this example) for use in
// initializing our panel class.
StripWrapper wrappers[] = {wrapper};

// Initialize our panel class which is capable of abstracting multiple wrapper
// classes in such a way that you can write to them as a single coordinate
// plane.
Panel panel(1, wrappers);

// ON AND OFF CONSTANTS
#define ALL_OFF 0x000000
#define ALL_ON 0x7f7f7f

// PRIMARY COLORS
// color order is GRB, not RGB
#define RED 0x00f700
#define ORANGE 0x207f00
#define YELLOW 0x7f7f00
#define GREEN 0x7f0000
#define BLUE 0x00007f
#define CYAN 0x7f007f
#define VIOLET 0x007f7f
#define PURPLE 0x10407f

// PIMARY COLORED RAINBOW
uint8_t STRIPES[6] = {5, 4, 4, 4, 4, 5};
prog_uint32_t RAINBOW[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};

void setup() {
  panel.begin();
  panel.show();
  // This initializes the color field with a suedo random field of color.
  // Otherwise there is some strangeness as it fades into the first frame.
  color_gradient(12345678, FADE_FRAMES - 1);
}


// Each loop generates a new suedo random frame of color and then fades the
// panel to that frame.
void loop() {
  uint32_t seed = hash_image();
  if (seed == 0)
    color_gradient(19738, FADE_FRAMES - 1);
  for(int k=0; k < FADE_FRAMES; k++)
    color_gradient(seed, k);
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


/*
 *  Outputs a random spread of color onto the entire panel based off of a
 *  random seed.  Given the same random seed, the exact same field of color
 *  will be generated.  The step indicates which step in the fade-transition we
 *  are in.  Set this value to FADE_FRAMES - 1 to go strait to the new color
 *  field.
 */
void color_gradient(long seed, int step) {
  // Give a specific random seed to ensure we can re-create this color gradient
  // for fading
  randomSeed(seed);

  // Temporary storage to hold the current block size.  This can vary on the
  // last block if the size of the panel is not evenly divisible by the block
  // size.
  int block_width = BLOCK_WIDTH;
  int block_height = BLOCK_HEIGHT;

  // Used as temporary storage for a color value as we walk across color
  // averages.
  uint32_t current;

  // We need to store the current anchor column of values along with the
  // previous column so that we can fade between frames.  Otherwise
  // getPixelColor methods would suffice.
  uint32_t current_column[panel.rows()];
  uint32_t previous_column[panel.rows()];

  int x = 0;
  while(x < panel.columns())
  {
    int y = 0;
    while(y < panel.rows())
    {
      // Record the new anchor color
      current_column[y] = panel.random_color();
      //current_column[y] = panel.random_primary();

      // Set the anchor pixel on the strip
      panel.setPixelAverage(x, y, current_column[y], step, FADE_FRAMES);

      // If we are not at an origin row, fill in the intermediate column values
      if (y - block_height >= 0)
      {
        current = current_column[y];
        for(int i=1; i < block_height; i++)
        {
          current = current_column[y-i] = panel.color_average(current, current_column[y - block_height], i, block_height);
          panel.setPixelAverage(x, y-i, current_column[y-i], step, FADE_FRAMES);
        }
      }

      // Increment y
      if ( y == panel.rows() - 1 )
        break;
      else if ( y + 2 * BLOCK_HEIGHT > panel.rows() - 1 )
      {
        block_height = panel.rows() - y - 1;
        y = panel.rows() - 1;
      }
      else
      {
        block_height = BLOCK_HEIGHT;
        y += BLOCK_HEIGHT;
      }
    }

    // If we are not at the origin column, fill in the intermediate column
    // values
    if (x - block_width >= 0)
    {
      for(int yy=0; yy<panel.rows(); yy++)
      {
        current = current_column[yy];
        for(int i=1; i < block_width; i++)
        {
          current = panel.color_average(current, previous_column[yy], i, block_width);
          panel.setPixelAverage(x-i, yy, current, step, FADE_FRAMES);
        }
      }
    }
    // Copy the current row into previous row.
    for(int i=0; i<panel.rows(); i++)
      previous_column[i] = current_column[i];
    panel.show();

    // Increment x
    if ( x == panel.columns() - 1 )
      break;
    else if ( x + 2 * BLOCK_WIDTH > panel.columns() - 1 )
    {
      block_width = panel.columns() - x - 1;
      x = panel.columns() - 1;
    }
    else
    {
      block_width = BLOCK_WIDTH;
      x += BLOCK_WIDTH;
    }
  }
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
