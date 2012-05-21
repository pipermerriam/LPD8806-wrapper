#include <SPI.h>
#include "panel.h"

#define DATA_PIN 8
#define CLOCK_PIN 9

#define ROW_SIZE 6
#define COLUMN_SIZE 26

#define FADE_FRAMES 64

// Define a strip wrapper, this one a has 6 columns of 26 pixels.  These would
// be wired in a zig-zag pattern which the wrapper and panel classes abstract
// into a 2d coordinate plane.
StripWrapper wrapper_a(ROW_SIZE, COLUMN_SIZE, DATA_PIN, CLOCK_PIN);

// Initialize an array of wrappers (singular in this example) for use in
// initializing our panel class.
StripWrapper wrappers[] = {wrapper_a};

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

int reading;
int diff;
int avg;
int height;

void setup() {
  Serial.begin(9600);
  panel.begin();
  panel.show();
}


void loop() {
  static long tt;
  
  if (millis() > tt)
  {
    reading = analogRead(2);
    Serial.println(reading);
    diff = reading - avg;
    avg += diff / 5;
    if ( avg != 0 )
      height = (panel.rows() * reading - avg) / ( 2 * avg );
    else
      height = 0;
    wave(height);
  }
  tt = millis();

}

void wave(int height) {
  //Shift all of the colomns over one.
  for (int x=panel.columns() - 1; x > 0; x--)
  {
    for (int y=0; y < panel.rows(); y++)
    {
      panel.setPixelColor(x, y, panel.getPixelColor(x - 1, y));
    }
  }
  for (int y=0; y < panel.rows(); y++)
  {
    if ( y <= height )
    {
      panel.setPixelColor(0, y, RAINBOW[y/4]);
    }
    else
      panel.setPixelColor(0, y, ALL_OFF);
  }
  panel.show();
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
