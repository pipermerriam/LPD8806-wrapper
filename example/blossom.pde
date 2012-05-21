#include "SPI.h"
#include "panel.h"

#define DATA_PIN 8
#define CLOCK_PIN 9

#define ROW_SIZE 6
#define COLUMN_SIZE 26

#define FADE_FRAMES 64

#define BLOSSOM_RADIUS 4

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
  int x = random(panel.columns());
  int y = random(panel.rows());
  color_blossom(x, y, panel.random_color());
}

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
