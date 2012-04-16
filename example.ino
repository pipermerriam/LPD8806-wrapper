#include "panel.h"
#include "SPI.h"

#define DATA_PIN_A 0
#define CLOCK_PIN_A 1
#define DATA_PIN_B 8
#define CLOCK_PIN_B 9

#define COLUMN_SIZE 26
#define ROW_COUNT 6

// Example to control LPD8806-based RGB LED Modules in a strip

/*****************************************************************************/

LPD8806 * strip_a = new LPD8806(156, DATA_PIN_A, CLOCK_PIN_A);
LPD8806 * strip_b = new LPD8806(156, DATA_PIN_B, CLOCK_PIN_B);
//StripWrapper * wrapper_a = new StripWrapper(COLUMN_SIZE, ROW_COUNT, strip_a);
//StripWrapper * wrapper_b = new StripWrapper(COLUMN_SIZE, ROW_COUNT, strip_b);
StripWrapper wrapper_a(COLUMN_SIZE, ROW_COUNT, strip_a);
StripWrapper wrapper_b(COLUMN_SIZE, ROW_COUNT, strip_b);

StripWrapper wrappers[] = {wrapper_a, wrapper_b};

Panel panel(2, wrappers);

void setup() {
  panel.begin();
  //wrapper_a.begin();
  //wrapper_b.begin();
}


void loop() {
  hard_rainbow();
}

void hard_rainbow()
{
  uint32_t colors[] = {
    panel.Color(70, 0, 127),
    panel.Color(0, 0, 127),
    panel.Color(0, 127, 0),
    panel.Color(127, 127, 0),
    panel.Color(127, 32, 0),
    panel.Color(127, 0, 0),
  };
  for(int i=0; i < 6; i++)
  {
    for(int r=i * COLUMN_SIZE / 6; r < (i+1) * COLUMN_SIZE / 6; r++)
    {
      panel.setRowColor(r, colors[i]);
    }
  }
  panel.show();
}

void rainbow()
{
  int i, j;
  for(j=0; j < 384 * 5; j++)
  {
    for(i=0; i < 26; i++)
    {
      panel.setRowColor(i, Wheel( (i * 384 / 26 + j) % 384) );
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
