#include "panel.h"

/*****************************************************************************/
Panel::Panel(uint8_t c, StripWrapper * w) {
  strip_count = c;
  wrappers = w;
  int column = 0;
  for (int i=0; i < strip_count; i++)
  {
    columns[i] = column;
    column += wrappers[i].columns();
  }
}

/*
 *  UTILITY METHODS
 */
uint32_t Panel::Color(byte r, byte g, byte b) {
  // Take the lowest 7 bits of each value and append them end to end
  // We have the top bit set high (its a 'parity-like' bit in the protocol
  // and must be set!)
  return 0x808080 | ((uint32_t)g << 16) | ((uint32_t)r << 8) | (uint32_t)b;
}

uint16_t Panel::column_to_strip(uint16_t x) {
  // Takes a cartesian x, y coordinate and returns the strip wrapper which
  // contains that corrdinate
  for (int i=0; i < strip_count; i ++)
  {
    if ( x < columns[i] + wrappers[i].columns() )
    {
      return i;
    }
  }
}


/*
 *   BEGIN AND SHOW
 */
void Panel::begin(void) {
  for(int i=0; i < strip_count; i++)
  {
    wrappers[i].begin();
  }
}

void Panel::show(void) {
  for(int i=0; i < strip_count; i++)
  {
    wrappers[i].show();
  }
}

/*
 *   COLUMN AND ROW SETTERS
 */
void Panel::setColumnColor(uint16_t x, uint8_t r, uint8_t g, uint8_t b) {
  setColumnColor(x, Color(r, g, b));
}

void Panel::setColumnColor(uint16_t x, uint32_t c) {
  int index = column_to_strip(x);
  int strip_x = x - columns[index];
  wrappers[index].setColumnColor(strip_x, c);
  //wrappers[index].setColumnColor(strip_x, c);
}

void Panel::setRowColor(uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setRowColor(y, Color(r, g, b));
}

void Panel::setRowColor(uint8_t y, uint32_t c) {
  for(int i=0; i < strip_count; i++)
  {
    wrappers[i].setRowColor(y, c);
  }
}

/*
 *   PIXEL SETTERS
 */
void Panel::setPixelColor(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setPixelColor(x, y, Color(r, g, b));
}

void Panel::setPixelColor(uint16_t x, uint8_t y, uint32_t c) {
  int index = column_to_strip(x);
  int strip_x = x - columns[index];
  wrappers[index].setPixelColor(strip_x, y, c);
}
