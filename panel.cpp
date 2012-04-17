#include "panel.h"

/*****************************************************************************/
Panel::Panel(uint8_t c, StripWrapper * w) {
  strip_count = c;
  wrappers = w;
  int column = 0;
  for (int i=0; i < strip_count; i++)
  {
    column_starts[i] = column;
    column += wrappers[i].columns();
  }
  column_count = column;
  row_count = wrappers[i].rows();
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
    if ( x < column_starts[i] + wrappers[i].columns() )
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
 *  INFO METHODS
 */
int Panel::columns(void) {
  return column_count;
}

int Panel::rows(void) {
  return row_count;
}

int Panel::pixels(void) {
  return row_count * column_count;
}

/*
 *  BOX SETTER
 */
void Panel::setBoxColor(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom, uint8_t r, uint8_t g, uint8_t b) {
  setBoxColor(left, right, top, bottom, Color(r, g, b));
}

void Panel::setBoxColor(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom, uint32_t c) {
  ///Colors a box bounded by coordinates left, right, top, bottom (inclusive)
  for (int x=left; x <= right; x++)
  {
    for (int y=bottom; y <= top; y++)
    {
      setPixelColor(x, y, c);
    }
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
  int strip_x = x - column_starts[index];
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
  int strip_x = x - column_starts[index];
  wrappers[index].setPixelColor(strip_x, y, c);
}
