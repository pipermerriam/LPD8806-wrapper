#include "panel.h"
#include "SPI.h"

#define DATA_PIN_A 0
#define CLOCK_PIN_A 1
#define DATA_PIN_B 4
#define CLOCK_PIN_B 5
#define DATA_PIN_C 6
#define CLOCK_PIN_C 7
//#define DATA_PIN_D 6
//#define CLOCK_PIN_D 7

#define COLUMN_COUNT 26
#define ROW_COUNT 8

int STRIPES[6] = {5, 4, 4, 4, 4, 5};

LPD8806 * strip_a = new LPD8806(208, DATA_PIN_A, CLOCK_PIN_A);
StripWrapper wrapper_a(COLUMN_COUNT, ROW_COUNT, strip_a);
LPD8806 * strip_b = new LPD8806(208, DATA_PIN_B, CLOCK_PIN_B);
StripWrapper wrapper_b(COLUMN_COUNT, ROW_COUNT, strip_b);
LPD8806 * strip_c = new LPD8806(182, DATA_PIN_C, CLOCK_PIN_C);
StripWrapper wrapper_c(COLUMN_COUNT, 7, strip_c);
//LPD8806 * strip_d = new LPD8806(156, DATA_PIN_D, CLOCK_PIN_D);
//StripWrapper wrapper_d(COLUMN_COUNT, ROW_COUNT, strip_d);

StripWrapper wrappers[] = {wrapper_a, wrapper_b, wrapper_c};
//StripWrapper wrappers[] = {wrapper_a, wrapper_c, wrapper_d};

//Panel panel(4, wrappers);
Panel panel(3, wrappers);

// OTHER COLORS
uint32_t ALL_OFF = panel.Color(0, 0, 0);
uint32_t ALL_ON = panel.Color(127, 127, 127);

// PRIMARY COLORS
uint32_t RED = panel.Color(127, 0, 0);
uint32_t ORANGE = panel.Color(127, 32, 0); // Could use some tweaking
uint32_t YELLOW = panel.Color(127, 127, 0);
uint32_t GREEN = panel.Color(0, 127, 0);
uint32_t BLUE = panel.Color(0, 0, 127);
uint32_t PURPLE = panel.Color(64, 16, 100); // Could use some tweaking

// PIMARY COLORED RAINBOW
uint32_t RAINBOW[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};

void setup() {
  panel.begin();
}


void loop() {
  pixel_test(GREEN);
  pixel_test(RED);
  pixel_test(BLUE);
  //multi_rainbow();
  //hard_rainbow();
  //rainbow();
}

void pixel_test(uint32_t color)
{
  for(int y=0; y < 26; y++)
  {
    for(int x=0; x < 23; x++)
    {
      panel.setPixelColor(x, y, color);
      panel.show();
    }
  }
}

void multi_rainbow()
{
  for(int r=0; r < COLUMN_COUNT; r++)
  {
    panel.setRowColor(r, RAINBOW[r%6]);
  }
}

void hard_rainbow()
{
  int left = 0;
  int right = 0;
  for(int i=0; i < 6; i++)
  {
    right += STRIPES[i];
    for(int r=left; r < right; r++)
    {
      panel.setRowColor(r, RAINBOW[i]);
    }
    left = right;
  }
  panel.show();
}

void rainbow()
{
  int i, j;
  for(j=0; j < 384 * 5; j++)
  {
    for(i=0; i < COLUMN_COUNT; i++)
    {
      panel.setRowColor(i, Wheel( (i * 384 / COLUMN_COUNT + j) % 384) );
    }
    panel.show();
  }
}

/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

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
