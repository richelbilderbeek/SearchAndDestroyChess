#ifndef UnitCoordinatGetterH
#define UnitCoordinatGetterH

#include "piece_color.h"

struct CoordinatGetter
{
  CoordinatGetter(const piece_color color)
    : mColor(color) {}
  int GetX(const int x) const;
  int GetY(const int y) const;
  piece_color mColor;
};

#endif
 
