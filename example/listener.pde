#include "SPI.h"
#include "fix_fft.h"
#include "panel.h"

/*
 *  This is an attempt at doing a realtime equalizer.  Sadly the FFT library
 *  doesn't appear to work very well and I don't know what to do to fix it.
 */

// Data and Clock pins for the single LPD8806 strip.
#define DATA_PIN 8
#define CLOCK_PIN 9

// The row and column size for the strip we'll be using.
#define ROW_SIZE 6
#define COLUMN_SIZE 26

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


int equalizer[26];

char im[128];
char data[128];

void setup() {
  Serial.begin(9600);
  // Start up the LED strip
  panel.begin();
  panel.show();

}

int reading;
int diff;
int avg;

void loop() {
  
  int static i = 0;
  static long tt;
  int val;
  int m;
  
   if (millis() > tt){
        if (i < 128){
          val = analogRead(2);
          //Serial.println(val, DEC);
          data[i] = val / 4 - 128;
          im[i] = 0;
          i++;  
          
        }
        else{
          //this could be done with the fix_fftr function without the im array.
          fix_fft(data,im,7,0);
          // I am only interessted in the absolute value of the transformation
          
          for (i=0; i< 64;i++){
             data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);
          }
          
          // Attempt to scale the equalizer bands.
          int scale[] = {
            4, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
          };
          
          for (int y=0; y < panel.rows(); y++)
          {
            for(int x=0; x < panel.columns(); x++)
            {
              int height = data[1 + 2*y] + data[2*y+3];
              if ( x <= height / scale[y] )
                panel.setPixelColor(x, y, RED);
              else
                panel.setPixelColor(x, y, ALL_OFF);
            }
          }
          panel.show();
        }
    tt = millis();
   }
}
