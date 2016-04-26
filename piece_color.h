#ifndef Unitpiece_colorH
#define Unitpiece_colorH

#include <iosfwd>

enum class piece_color
{
  black, white
};

std::ostream& operator<<(std::ostream& os, const piece_color& color);

#endif
