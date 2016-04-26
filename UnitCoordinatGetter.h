#ifndef UnitCoordinatGetterH
#define UnitCoordinatGetterH

#include "UnitEnumChessPieceColor.h"

struct CoordinatGetter
{
  CoordinatGetter(const EnumChessPieceColor color)
    : mColor(color) {}
  int GetX(const int x) const;
  int GetY(const int y) const;
  EnumChessPieceColor mColor;
};

#endif
 
