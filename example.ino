#include "panel.h"
#include "SPI.h"

#define DATA_PIN_A 0
#define CLOCK_PIN_A 1
#define DATA_PIN_B 4
#define CLOCK_PIN_B 5
#define DATA_PIN_C 2
#define CLOCK_PIN_C 3
#define DATA_PIN_D 6
#define CLOCK_PIN_D 7

#define COLUMN_COUNT 6
#define ROW_COUNT 26

uint8_t STRIPES[6] = {5, 4, 4, 4, 4, 5};

LPD8806 * strip_a = new LPD8806(104, DATA_PIN_A, CLOCK_PIN_A);
StripWrapper wrapper_a(4, ROW_COUNT, strip_a);
LPD8806 * strip_b = new LPD8806(52, DATA_PIN_B, CLOCK_PIN_B);
StripWrapper wrapper_b(2, ROW_COUNT, strip_b);
LPD8806 * strip_c = new LPD8806(78, DATA_PIN_C, CLOCK_PIN_C);
StripWrapper wrapper_c(3, ROW_COUNT, strip_c);
LPD8806 * strip_d = new LPD8806(78, DATA_PIN_D, CLOCK_PIN_D);
StripWrapper wrapper_d(3, ROW_COUNT, strip_d);

StripWrapper wrappers[] = {wrapper_a, wrapper_b, wrapper_c, wrapper_d};
//StripWrapper wrappers[] = {wrapper_a, wrapper_c, wrapper_d};

Panel panel(4, wrappers);
//Panel panel(1, wrappers);

// OTHER COLORS
#define ALL_OFF 0x000000
//uint32_t ALL_OFF = panel.Color(0, 0, 0);
#define ALL_ON 0x7f7f7f
//uint32_t ALL_ON = panel.Color(127, 127, 127);

// PRIMARY COLORS
// color order is GRB, not RGB
//uint32_t RED = panel.Color(127, 0, 0);
#define RED 0x00f700
#define ORANGE 0x207f00
#define YELLOW 0x7f7f00
#define GREEN 0x007f00
//uint32_t ORANGE = panel.Color(127, 32, 0); // Could use some tweaking
//uint32_t YELLOW = panel.Color(127, 127, 0);
#define GREEN 0x7f0000
//uint32_t GREEN = panel.Color(0, 127, 0);
#define BLUE 0x00007f
//uint32_t BLUE = panel.Color(0, 0, 127);
#define TEAL 0x7f007f
#define VIOLET 0x007f7f
#define PURPLE 0x10407f
//uint32_t PURPLE = panel.Color(64, 16, 100); // Could use some tweaking

// PIMARY COLORED RAINBOW
prog_uint32_t RAINBOW[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};

void setup() {
  panel.begin();
}


void loop() {
  //fill_rainbow();
  //hard_rainbow();
  //multi_rainbow();
  //rainbow();
  //knight_rider();
  
  //panel.setPixelColor(0, 6, RED);
  //panel.setPixelColor(6, 6, BLUE);
  //panel.setPixelColor(6, 0, GREEN);

  //panel.setRowColor(4, TEAL);

  //panel.setColumnColor(8, YELLOW);
  //panel.show();

  //panel.setPixelColor(2, 8, YELLOW);
  //panel.setPixelColor(8, 2, YELLOW);
  //panel.show();
  //delay(100000);

  //wrapper_a.setPixelColor(0, 2, BLUE);
  //wrapper_a.setPixelColor(2, 0, RED);
  //wrapper_a.setRowColor(4, BLUE);
  //wrapper_a.setColumnColor(1, PURPLE);
  //wrapper_a.show();

  //wrapper_b.setPixelColor(1, 0, ORANGE);
  //wrapper_b.setPixelColor(0, 1, GREEN);
  //wrapper_b.setRowColor(5, RED);
  //wrapper_b.setColumnColor(1, TEAL);
  //wrapper_b.show();

  //wrapper_c.setPixelColor(1, 0, RED);
  //wrapper_c.setPixelColor(0, 1, YELLOW);
  //wrapper_c.setRowColor(6, GREEN);
  //wrapper_c.setColumnColor(1, ALL_ON);
  //wrapper_c.show();

  //wrapper_d.setPixelColor(1, 0, PURPLE);
  //wrapper_d.setPixelColor(0, 1, ALL_ON);
  //wrapper_d.setRowColor(7, ORANGE);
  //wrapper_d.setColumnColor(1, RED);
  //wrapper_d.show();
  //delay(10000000);

  pixel_test(ALL_ON);
  pixel_test(RED);
  pixel_test(ORANGE);
  pixel_test(YELLOW);
  pixel_test(GREEN);
  pixel_test(BLUE);
  pixel_test(TEAL);
  pixel_test(VIOLET);
  pixel_test(PURPLE);
  pixel_test(ALL_OFF);
}

void pixel_test(uint32_t color)
{
  for (uint8_t y=0; y < 26; y++)
  {
    for (uint8_t x=0; x < 12 ; x++)
    {
      panel.setPixelColor(x, y, color);
      panel.show();
    }
  }
}

void knight_rider()
{
  panel.setColumnColor(0, ALL_OFF);
  panel.setColumnColor(1, ALL_OFF);

  bool reverse;
  for(uint8_t i=0; i < 30; i++)
  {
    reverse = (i%2);
    for(uint8_t j=0; j < ROW_COUNT / 2; j++)
    {
      uint8_t p;
      if (reverse)
      {
        p = ROW_COUNT - (2 * j);
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

void fill_rainbow()
{
  panel.setColumnColor(0, ALL_OFF);
  panel.setColumnColor(1, ALL_OFF);

  for(uint8_t i=ROW_COUNT; i > 0; i--)
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

void multi_rainbow()
{
  for(uint8_t r=0; r < ROW_COUNT / 2; r++)
  {
    panel.setRowColor(r * 2, RAINBOW[r%6]);
    panel.setRowColor(r * 2 + 1, RAINBOW[r%6]);
  }
  panel.show();
  delay(60000);
}

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

void rainbow()
{
  uint16_t i, j;
  for(j=0; j < 384 * 15; j++)
  {
    for(i=0; i < ROW_COUNT; i++)
    {
      panel.setRowColor(i, Wheel( (i * 384 / ROW_COUNT + j) % 384) );
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
