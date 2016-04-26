#ifndef UnitCoordinatGetterH
#define UnitCoordinatGetterH

#include "piece_color.h"

struct coordinat_getter
{
  coordinat_getter(const piece_color color)
    : m_color(color) {}
  int get_x(const int x) const;
  int get_y(const int y) const;
  piece_color m_color;
};

#endif
 
