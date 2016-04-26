#include <iostream>
#include <cassert>
#include <stdexcept>


#include "piece_color.h"

std::ostream& operator<<(std::ostream& os, const piece_color& color)
{
  switch (color)
  {
    case piece_color::white: os << "W"; return os;
    case piece_color::black: os << "B"; return os;
  }
  assert(!"Should not get here");
  throw std::logic_error("Unknown color");

}


