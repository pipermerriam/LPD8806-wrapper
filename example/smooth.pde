#include "SPI.h"
#include "panel.h"

#define DATA_PIN 8
#define CLOCK_PIN 9

#define ROW_SIZE 6
#define COLUMN_SIZE 26

#define FADE_FRAMES 256

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
  Serial.begin(9600);
  panel.begin();
  panel.show();
  color_gradient(12345678, FADE_FRAMES - 1);
}


void loop() {
  uint32_t seed = hash_image();
  if (seed == 0)
    color_gradient(19738, FADE_FRAMES - 1);
  Serial.println(seed);
  for(int k=0; k < FADE_FRAMES; k++)
    color_gradient(seed, k);
}


uint32_t hash_image() {
  uint32_t hash = 0;
  for(int x=0; x < panel.columns(); x++)
  {
    for(int y=0; y < panel.rows(); y++)
      hash ^= panel.getPixelColor(x, y);
  }
  return hash;
}

#define BLOCK_WIDTH 5
#define BLOCK_HEIGHT 13


/*
 *  Outputs a random spread of color onto the entire panel based off of a
 *  random seed
 */
void color_gradient(long seed, int step) {
  // Give a specific random seed to ensure we can re-create this color gradient
  // for fading
  randomSeed(seed);

  // Initialize the grid with a random color at the origin.

  // Walk the grid
  int x, y;

  int block_width = BLOCK_WIDTH;
  int block_height = BLOCK_HEIGHT;

  uint32_t intermediate, current, target;

  uint32_t current_column[panel.rows()];
  uint32_t previous_column[panel.rows()];

  //for (x=0; x < panel.columns(); x += BLOCK_WIDTH)
  x = 0;
  while(x < panel.columns())
  {
    Serial.println(x);
    //for (y=0; y < panel.rows(); y += BLOCK_HEIGHT)
    y = 0;
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
        target = current_column[y - block_height];
        for(int i=1; i < block_height; i++)
        {
          current = current_column[y-i] = panel.color_average(current, target, i, block_height);
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
        target = previous_column[yy];
        for(int i=1; i < block_width; i++)
        {
          current = intermediate = panel.color_average(current, target, i, block_width);
          panel.setPixelAverage(x-i, yy, intermediate, step, FADE_FRAMES);
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

/*
 * Fades the entire strip to a new color across 64 calls to show.
 */
void fade_to_color(uint32_t color) {
  for(int k=0; k < FADE_FRAMES; k++)
  {
    for (int y=0; y < panel.rows(); y++)
    {
      for (int x=0; x < panel.columns(); x++)
      {
        uint32_t current = panel.getPixelColor(x, y);

        panel.setPixelColor(x, y, panel.color_average(current, color, k, FADE_FRAMES));
      }
    }
    panel.show();
  }
}
