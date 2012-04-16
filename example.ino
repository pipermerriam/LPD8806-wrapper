#include "panel.h"
#include "SPI.h"

#define DATA_PIN_A 0
#define CLOCK_PIN_A 1
#define DATA_PIN_B 8
#define CLOCK_PIN_B 9

#define COLUMN_SIZE 26
#define ROW_COUNT 6

int STRIPES[6] = {5, 4, 4, 4, 4, 5};

// Example to control LPD8806-based RGB LED Modules in a strip

/*****************************************************************************/

LPD8806 * strip_a = new LPD8806(156, DATA_PIN_A, CLOCK_PIN_A);
LPD8806 * strip_b = new LPD8806(156, DATA_PIN_B, CLOCK_PIN_B);
StripWrapper wrapper_a(COLUMN_SIZE, ROW_COUNT, strip_a);
StripWrapper wrapper_b(COLUMN_SIZE, ROW_COUNT, strip_b);

StripWrapper wrappers[] = {wrapper_a, wrapper_b};

Panel panel(2, wrappers);

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
  //multi_rainbow();
  hard_rainbow();
}

void multi_rainbow()
{
  for(int r=0; r < COLUMN_SIZE; r++)
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
    for(i=0; i < COLUMN_SIZE; i++)
    {
      panel.setRowColor(i, Wheel( (i * 384 / COLUMN_SIZE + j) % 384) );
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
  return((*strip_a).Color(r,g,b));
}
