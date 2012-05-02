#include "panel.h"

/*****************************************************************************/
Panel::Panel(uint8_t c, StripWrapper * w) {
  wrapper_count = c;
  wrappers = w;
  uint16_t column = 0;
  for (uint8_t i=0; i < wrapper_count; i++)
  {
    column_starts[i] = column;
    column += wrappers[i].num_columns();
  }
  column_count = column;
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

uint16_t Panel::column_to_wrapper(uint16_t y) {
  // Takes a cartesian x, y coordinate (eas:???) and returns the strip wrapper which
  // contains that corrdinate
  for (int i=0; i < wrapper_count; i++)
  {
    if ( y < (column_starts[i] + wrappers[i].num_columns())) // eas: test this with diff-sized wrappers
    {
      return i;
    }
  }
}


/*
 *   BEGIN AND SHOW
 */
void Panel::begin(void) {
  for(int i=0; i < wrapper_count; i++)
  {
    wrappers[i].begin();
  }
}

void Panel::show(void) {
  for(int i=0; i < wrapper_count; i++)
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

/*
 *  BOX SETTER
 */
void Panel::setBoxColor(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom, uint8_t r, uint8_t g, uint8_t b) {
  setBoxColor(left, right, top, bottom, Color(r, g, b));
}

void Panel::setBoxColor(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom, uint32_t c) {
  ///Colors a box bounded by coordinates left, right, top, bottom (inclusive)
  for (uint16_t x=left; x <= right; x++)
  {
    for (uint8_t y=bottom; y <= top; y++)
    {
      setPixelColor(x, y, c);
    }
  }
}

/*
 *   COLUMN AND ROW SETTERS
 */
void Panel::setColumnColor(uint16_t y, uint8_t r, uint8_t g, uint8_t b) {
  setColumnColor(y, Color(r, g, b));
}

void Panel::setColumnColor(uint16_t y, uint32_t c) {
  uint8_t index = column_to_wrapper(y);
  uint8_t strip_y = y - column_starts[index];
  wrappers[index].setColumnColor(strip_y, c);
}

void Panel::setRowColor(uint8_t x, uint8_t r, uint8_t g, uint8_t b) {
  setRowColor(x, Color(r, g, b));
}

void Panel::setRowColor(uint8_t x, uint32_t c) {
  for(uint8_t i=0; i < wrapper_count; i++)
  {
    wrappers[i].setRowColor(x, c);
  }
}

/*
 *   PIXEL SETTERS
 */
void Panel::setPixelColor(uint16_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  setPixelColor(x, y, Color(r, g, b));
}

void Panel::setPixelColor(uint16_t x, uint8_t y, uint32_t c) {
  uint8_t index = column_to_wrapper(y);
  uint8_t starts = column_starts[index];
  wrappers[index].setPixelColor(x, y - starts , c);
}
