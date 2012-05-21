#include "SPI.h"
#include "panel.h"

#define DATA_PIN 8
#define CLOCK_PIN 9

#define ROW_SIZE 6
#define COLUMN_SIZE 26

#define FADE_FRAMES 64

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
}


void loop() {
  //pixel_test(RED);
  //pixel_test(ALL_OFF);
  //pixel_test(ORANGE);
  //pixel_test(YELLOW);
  //pixel_test(GREEN);
  //pixel_test(CYAN);
  //pixel_test(BLUE);
  //pixel_test(VIOLET);

  color_gradient(1000);
  int x, y;
  for(int i=0; i < 100; i++)
  {
    x = random(panel.columns());
    y = random(panel.rows());
    color_blossom(x, y, panel.random_color());
  }
}

# define BLOSSOM_RADIUS 4


/*
 *   Blossoms a color out of an x, y coordinate using color fading to slowly fade it in.
 */
void color_blossom(int x, int y, uint32_t color){
  uint32_t current;
  for(int r=0; r<BLOSSOM_RADIUS + 2; r++)
  {
    for(int k=0; k<FADE_FRAMES/BLOSSOM_RADIUS; k++)
    {
      for(int xx=x-r; xx<=x+r; xx++)
      {
        for(int yy=y-r; yy<=y+r; yy++)
        {
          if ( xx < 0 or xx >= panel.columns() or yy < 0 or yy >= panel.rows() )
            continue;
          int radius = r - max(abs(xx-x), abs(yy-y));
          int step = min((radius * FADE_FRAMES) / BLOSSOM_RADIUS + k, FADE_FRAMES - 1);
          current = panel.getPixelColor(xx, yy);
          panel.setPixelColor(xx, yy, panel.color_average(current, color, step, FADE_FRAMES));
        }
      }
      panel.show();
    }
  }
}

#define BLOCK_WIDTH 5
#define BLOCK_HEIGHT 5


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
  int x, y = 0;
  uint32_t intermediate, current, target;

  for (y=0; y < panel.rows(); y += BLOCK_HEIGHT)
  {
    for (x=0; x < panel.columns(); x += BLOCK_WIDTH)
    {
      panel.setPixelColor(x, y, panel.random_color());

      if (x - BLOCK_WIDTH >= 0)
      {
        target = panel.getPixelColor(x-BLOCK_WIDTH, y);
        current = panel.getPixelColor(x, y);
        for(int i=1; i < BLOCK_WIDTH; i++)
        {
          intermediate = panel.color_average(current, target, i, BLOCK_WIDTH);
          panel.setPixelColor(x-i, y, intermediate);
        }
      }
    }
    if (y - BLOCK_HEIGHT >= 0)
    {
      for(int xx=0; xx<panel.columns(); xx++)
      {
        target = panel.getPixelColor(xx, y-BLOCK_WIDTH);
        current = panel.getPixelColor(xx, y);
        for(int i=1; i < BLOCK_HEIGHT; i++)
        {
          intermediate = panel.color_average(current, target, i, BLOCK_HEIGHT);
          panel.setPixelColor(xx, y-i, intermediate);
        }
      }
    }
    panel.show();
  }
  delay(1000);
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
