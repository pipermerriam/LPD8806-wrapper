#include "panel.h"
#include "SPI.h"

#define DATA_PIN 0
#define CLOCK_PIN 1

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
#define TEAL 0x7f007f
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
  pixel_test(RED);
  pixel_test(ORANGE);
  pixel_test(YELLOW);
  pixel_test(GREEN);
  pixel_test(TEAL);
  pixel_test(BLUE);
  pixel_test(VIOLET);

  pixel_test(ALL_ON);
  pixel_test(ALL_OFF);

  fade_to_color(RED);
  fade_to_color(ORANGE);
  fade_to_color(YELLOW);
  fade_to_color(GREEN);
  fade_to_color(TEAL);
  fade_to_color(BLUE);
  fade_to_color(VIOLET);

  fade_to_color(ALL_OFF);

  fill_rainbow();
  hard_rainbow();
  multi_rainbow();
  rainbow();
  knight_rider();
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

/*
 *  Traces two red pixels up and down the columns of the strip.
 */
void knight_rider()
{
  panel.setColumnColor(0, ALL_OFF);
  panel.setColumnColor(1, ALL_OFF);

  bool reverse;
  for(uint8_t i=0; i < 30; i++)
  {
    reverse = (i%2);
    for(uint8_t j=0; j < panel.rows() / 2; j++)
    {
      uint8_t p;
      if (reverse)
      {
        p = panel.rows() - (2 * j);
      }
      else
      {
        p = 2 * j;
      }
      panel.setRowColor(p, RED);
      panel.setRowColor(p + 1, RED);
      panel.show();
      delay(40);
      panel.setRowColor(p, ALL_OFF);
      panel.setRowColor(p + 1, ALL_OFF);
    }
  }
}

/*
 *  Fills in a hard-transition rainbow one pixel at a time.
 */
void fill_rainbow()
{
  panel.setColumnColor(0, ALL_OFF);
  panel.setColumnColor(1, ALL_OFF);

  for(uint8_t i=panel.rows(); i > 0; i--)
  {
    uint32_t color = RAINBOW[i / 13];
    for(uint8_t j=0; j < i; j++)
    {
      panel.setRowColor(j, color);
      panel.show();
      panel.setRowColor(j, ALL_OFF);
    }
    panel.setRowColor(i - 1, color);
    panel.show();
  }
}

/*
 *  Fills a rainbow vertically across the panel with 2 pixels per color.
 */
void multi_rainbow()
{
  for(uint8_t r=0; r < panel.rows() / 2; r++)
  {
    panel.setRowColor(r * 2, RAINBOW[r%6]);
    panel.setRowColor(r * 2 + 1, RAINBOW[r%6]);
  }
  panel.show();
  delay(60000);
}


/*
 *  Sets a hard-transition rainbow across the pixels on the panel.
 */
void hard_rainbow()
{
  for(uint8_t i=0; i < 6; i++)
  {
    for(uint8_t j=0; j < 13; j++)
    {
      panel.setRowColor(i*13 + j, RAINBOW[i]);
    }
  }
  panel.show();
  delay(60000);
}


/*
 *  Sets the fully saturated colors of an RGB color wheel equally spaces across
 *  the panel.  Largely copied from the original LPD8806 library.
 */
void rainbow()
{
  uint16_t i, j;
  for(j=0; j < 384 * 15; j++)
  {
    for(i=0; i < panel.columns(); i++)
    {
      panel.setRowColor(i, Wheel( (i * 384 / panel.columns() + j) % 384) );
    }
    panel.show();
  }
}

/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r
// (From the original LPD8806 library)

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(panel.Color(r,g,b));
}
