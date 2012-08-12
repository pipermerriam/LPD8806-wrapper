#include "SPI.h"
#include "panel.h"

/*  
 *  This example code smoothly fades the board between a random field of
 *  smoothly faded colors.
 */

// The row and column size for the strip we'll be using.
#define ROW_SIZE 6
#define COLUMN_SIZE 26

// Define a strip wrapper, this one a has 6 columns of 26 pixels.  These would
// be wired in a zig-zag pattern which the wrapper and panel classes abstract
// into a 2d coordinate plane.
StripWrapper wrapper1(ROW_SIZE, COLUMN_SIZE, 52, 53);
StripWrapper wrapper2(ROW_SIZE, COLUMN_SIZE, 50, 51);
StripWrapper wrapper3(ROW_SIZE, COLUMN_SIZE, 48, 49);
StripWrapper wrapper4(ROW_SIZE+1, COLUMN_SIZE, 46, 47);
StripWrapper wrapper5(ROW_SIZE, COLUMN_SIZE, 44, 45);
StripWrapper wrapper6(ROW_SIZE, COLUMN_SIZE, 42, 43);
StripWrapper wrapper7(ROW_SIZE, COLUMN_SIZE, 40, 41);
StripWrapper wrapper8(ROW_SIZE+1, COLUMN_SIZE, 38, 39);
StripWrapper wrapper9(ROW_SIZE, COLUMN_SIZE, 36, 37);
StripWrapper wrapper10(ROW_SIZE, COLUMN_SIZE, 34, 35);
StripWrapper wrapper11(ROW_SIZE, COLUMN_SIZE, 32, 33);
StripWrapper wrapper12(ROW_SIZE+1, COLUMN_SIZE, 30, 31);
StripWrapper wrapper13(ROW_SIZE, COLUMN_SIZE, 28, 29);
StripWrapper wrapper14(ROW_SIZE, COLUMN_SIZE, 72, 2);
StripWrapper wrapper15(ROW_SIZE, COLUMN_SIZE, 73, 3);
StripWrapper wrapper16(ROW_SIZE+1, COLUMN_SIZE, 22, 23);

// Initialize an array of wrappers (singular in this example) for use in
// initializing our panel class.
StripWrapper wrappers[] = {wrapper1, wrapper2, wrapper3, wrapper4, wrapper5,
  wrapper6, wrapper7, wrapper8, wrapper9, wrapper10, wrapper11, wrapper12,
  wrapper13, wrapper14, wrapper15, wrapper16};

// Initialize our panel class which is capable of abstracting multiple wrapper
// classes in such a way that you can write to them as a single coordinate
// plane.
Panel panel(16, wrappers);

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
uint32_t RAINBOW[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};

int program = 0;
int time = 0;

void setup() {
  //Serial.begin(9600);
  panel.begin();
  panel.show();
  // This initializes the color field with a suedo random field of color.
  // Otherwise there is some strangeness as it fades into the first frame.
}

#define MINUTES_1 60000
#define MINUTES_2 120000
#define MINUTES_5 300000
#define MINUTES_10 720000

// Each loop generates a new suedo random frame of color and then fades the
// panel to that frame.
void loop() {
  wrapper_test(RED);
  wrapper_test(PURPLE);
  return;
  //panel_test();

  //program = millis() % 6;
  program = random(6);
  switch (program)
  {
    case 0:
      hard_rainbow();
      delay(MINUTES_10);
      break;
    case 1:
      slider(MINUTES_5);
      break;
    case 2:
      //color_wipe(MINUTES_1);
      break;
    case 3:
      bounce(MINUTES_2);
      break;
    case 4:
      rainbow(MINUTES_5);
      break;
    case 5:
      color_rain(MINUTES_5);
      break;
    case 6:
      color_blossom(MINUTES_2);
      break;
  }
}

void wrapper_test(uint32_t color)
{
  for (uint16_t y=0; y < panel.rows(); y++)
  {
    panel.setRowColor(y, color);
    panel.show();
  }
}


/*
 *  Outputs a random spread of color onto the entire panel based off of a
 *  random seed.  Given the same random seed, the exact same field of color
 *  will be generated.  The step indicates which step in the fade-transition we
 *  are in.  Set this value to FADE_FRAMES - 1 to go strait to the new color
 *  field.
 */

/*
 *  Steps through all pixels on the panel, setting them individually to a color
 */
void pixel_test(uint32_t color)
{
  for (uint16_t y=0; y < panel.rows(); y++)
  {
    for (uint16_t x=0; x < panel.columns() ; x++)
    {
      panel.setPixelColor(x, y, color);
      panel.show();
    }
  }
}

void panel_test()
{
  pixel_test(RED);
  pixel_test(GREEN);
  pixel_test(BLUE);
  pixel_test(ALL_OFF);
  pixel_test(ALL_ON);
}

/*
 *   HARD RAINBOW
 */
void hard_rainbow()
{
  int current = 0;

  for(int i=0; i<6; i++)
  {
    for(int j=current; j< current+STRIPES[i]; j++)
    {
      panel.setRowColor(j, RAINBOW[i]);
    }
    current += STRIPES[i];
  }
  panel.show();
}

#define FADE_FRAMES 256

/*
 *   SLIDER
 */
void slider(int duration)
{
  long x = 0;
  long y = 0;

  long target_x = random(x - 500, x + 500);
  long target_y = random(y - 500, y + 500);

  long now = millis();

  while ((millis() - now < duration) or (millis() < now))
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
  }
}

// Helper function for slider
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
 * Color Wipers
 */
void color_wipe(long duration)
{
  uint32_t color;
  int direction;
  long now = millis();

  while ((millis() - now < duration) or (millis() < now))
  {
    color = panel.random_primary();
    direction = random(0, 4);

    if (direction == 0)
      wipe_up(color);
    else if (direction == 1)
      wipe_down(color);
    else if (direction == 2)
      wipe_left(color);
    else
      wipe_right(color);
  }
}

void wipe_up(uint32_t color)
{
  for (int y=0; y < panel.rows(); y++)
  {
    panel.setRowColor(y, color);
    panel.show();
  }
}

void wipe_down(uint32_t color)
{
  for (int y=panel.rows() - 1; y >= 0; y--)
  {
    panel.setRowColor(y, color);
    panel.show();
  }
}

void wipe_left(uint32_t color)
{
  for (int x=panel.columns() - 1; x >= 0; x--)
  {
    panel.setColumnColor(x, color);
    panel.show();
  }
}

void wipe_right(uint32_t color)
{
  for (int x=0; x < panel.columns(); x++)
  {
    panel.setColumnColor(x, color);
    panel.show();
  }
}

void bounce(long duration)
{
  long now = millis();
  
  int x = 5;
  int y = 0;
  uint32_t current = panel.random_primary();
  uint32_t target = panel.random_primary();
  int x_velocity = 1;
  int y_velocity = 2;

  while ((millis() - now < duration) or (millis() < now))
  {
    target = panel.random_primary();
    for(int i=0; i <= FADE_FRAMES; i++)
    {
      if((x + x_velocity >= panel.columns()) or (x + x_velocity < 0 ))
      {
        x_velocity *= -1;
      }
      if((y + y_velocity >= panel.rows()) or (y + y_velocity < 0 ))
      {
        y_velocity *= -1;
      }

      for(int xx=x - 1; xx <= x + 1; xx++)
      {
        for(int yy=y - 1; yy <= y + 1; yy++)
        {
          panel.setPixelColor(xx, yy, panel.color_average(current, target, i, FADE_FRAMES, false));
        }
      }
      x += x_velocity;
      y += y_velocity;

      panel.show();
      panel.darken();
    }
    current = target;
    target = panel.random_color();
  }
}

/*
 *  CYCLING RAINBOW
 */
void rainbow(long duration)
{
  int i, j;
  long now = millis();

  while ((millis() - now < duration) or (millis() < now))
  {
    for(j=0; j < 384; j++)
    {
      for(i=0; i < panel.rows(); i++)
      {
        panel.setRowColor(i, Wheel( (i * 384 / panel.rows() + j) % 384) );
      }
      panel.show();
    }
  }
}

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

/*
 *  COLOR RAIN
 */
void color_rain(long duration)
{
  long now = millis();

  while ((millis() - now < duration) or (millis() < now))
  {
    panel.setPixelColor(random(0,panel.columns()-1), panel.rows()-1, panel.random_color());

    panel.show();
    panel.shiftDown();
    panel.darken();
  }
}

/*
 *  COLOR BLOSSOM
 */
#define BLOSSOM_RADIUS 5

void color_blossom(long duration)
{
  int x, y;
  uint32_t current, color;

  long now = millis();

  while ((millis() - now < duration) or (millis() < now))
  {
    color = panel.random_color();
    x = random(panel.columns());
    y = random(panel.rows());

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
            panel.setPixelAverage(xx, yy, color, step, FADE_FRAMES);
          }
        }
        panel.show();
      }
    }
  }
}
