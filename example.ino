#include "wrapper.h"
#include "SPI.h"

#define DATA_PIN 0
#define CLOCK_PIN 1
#define COLUMN_SIZE 26
#define ROW_COUNT 6

// Example to control LPD8806-based RGB LED Modules in a strip

/*****************************************************************************/

LPD8806 * strip = new LPD8806(156, DATA_PIN, CLOCK_PIN);
StripWrapper wrapper(COLUMN_SIZE, ROW_COUNT, strip);

void setup() {
  wrapper.begin();
}


void loop() {
  //for (int y=0; y < 26; y++)
  //{
  //  wrapper.setRowColor(y, 127, 127, 127);
  //  //wrapper.setPixelColor(x, y, 127, 127, 127);
  //  wrapper.show();
  //}
  //for (int y=0; y < 26; y++)
  //{
  //  wrapper.setRowColor(y, 0, 0, 0);
  //  //wrapper.setPixelColor(x, y, 127, 127, 127);
  //  wrapper.show();
  //}
  //for (int x=0; x < 156; x++)
  //{
  //    //(*strip).setPixelColor(x, 0, 0, 0);
  //    (*strip).setPixelColor(x, 127, 127, 127);
  //    //wrapper.setPixelColor(5, 10, 127, 127, 127);
  //    (*strip).show();
  //}
  rainbow();
}

void rainbow()
{
  int i, j;
  for(j=0; j < 384 * 5; j++)
  {
    for(i=0; i < 26; i++)
    {
      wrapper.setRowColor(i, Wheel( (i * 384 / 26 + j) % 384) );
    }
    wrapper.show();
    //delay(10);
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
  return((*strip).Color(r,g,b));
}
