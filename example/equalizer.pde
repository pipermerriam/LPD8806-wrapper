#include "SPI.h"
#include "panel.h"

/*  
 *  This example code smoothly fades the board between a random field of
 *  smoothly faded colors.
 */

// Data and Clock pins for the single LPD8806 strip.
#define DATA_PIN 8
#define CLOCK_PIN 9

// Spectrum Analizer pins
#define SPECTRUM_RESET 5
#define SPECTRUM_STROBE 4

#define SPECTRUM_ANALOG_LEFT 0
#define SPECTRUM_ANALOG_RIGHT 1

// The row and column size for the strip we'll be using.
#define ROW_SIZE 6
#define COLUMN_SIZE 26

// Defines how many frames will be used to transition between colors.  Larger
// numbers will produce slower and smoother transitions
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
prog_uint32_t RAINBOW[] = {RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE};

void setup() {
  Serial.begin(9600);
  panel.begin();
  panel.show();

  setup_spectrum();
}


void loop() {
  Serial.println(999999);
  equalizer();
}

#define SOUND_CUTOFF 200

void equalizer() {
  digitalWrite(SPECTRUM_RESET,HIGH);
  digitalWrite(SPECTRUM_RESET,LOW);     
  // array of integer values for our spectrum equalizer
  int reading, height;
  int maximum = 0;
  int minimum = 0;
  static int avg = 0;
  static int spectrum[7];

  // band 0 = Lowest Frequencies.
  for(int band=0; band < 7; band++)
  {
    if (band == 0)
      continue;
    //Read twice and take the average by dividing by 2
    reading = (analogRead(SPECTRUM_ANALOG_LEFT) + analogRead(SPECTRUM_ANALOG_RIGHT) ) >>1; 
    if ( reading < SOUND_CUTOFF )
      reading = 0;
    else
      minimum = min(reading, minimum);
    maximum = max(reading, maximum);

    spectrum[band] = reading; 

    // Tell the spectrum board to move onto the next frequency
    digitalWrite(SPECTRUM_STROBE,HIGH);
    digitalWrite(SPECTRUM_STROBE,LOW);     
    Serial.println(spectrum[band]);
  }

  for(int x=0; x < panel.rows(); x++)
  {
    height = spectrum[x + 1] / 36;
    for(int y=0; y < panel.rows(); y++)
    {
      if (y <= height)
        panel.setPixelColor(x, y, RED);
      else
        panel.setPixelColor(x, y, ALL_OFF);
    }
  }
  panel.show();
}

void setup_spectrum() {
  //Setup pins to drive the spectrum analyzer. 
  pinMode(SPECTRUM_RESET, OUTPUT);
  pinMode(SPECTRUM_STROBE, OUTPUT);

  //Init spectrum analyzer
  digitalWrite(SPECTRUM_STROBE,LOW);
  delay(1);
  digitalWrite(SPECTRUM_RESET,HIGH);
  delay(1);
  digitalWrite(SPECTRUM_STROBE,HIGH);
  delay(1);
  digitalWrite(SPECTRUM_STROBE,LOW);
  delay(1);
  digitalWrite(SPECTRUM_RESET,LOW);
  delay(5);
  // Reading the analyzer now will read the lowest frequency.
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
