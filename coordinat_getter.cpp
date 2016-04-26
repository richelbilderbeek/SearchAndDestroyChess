#include "coordinat_getter.h"

int coordinat_getter::get_x(const int x) const
{
  if (m_color == piece_color::white)
  {
    return x;
  }
  else
  {
    return 7 - x;
  }
}

int coordinat_getter::get_y(const int y) const
{
  if (m_color == piece_color::white)
  {
    return 7 - y;
  }
  else
  {
    return y;
  }
}


