#include "UnitCoordinatGetter.h"

int CoordinatGetter::GetX(const int x) const
{
  if (mColor == white)
  {
    return x;
  }
  else
  {
    return 7 - x;
  }
}

int CoordinatGetter::GetY(const int y) const
{
  if (mColor == white)
  {
    return 7 - y;
  }
  else
  {
    return y;
  }
}


