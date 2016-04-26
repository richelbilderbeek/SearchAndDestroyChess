#ifndef Unitpiece_typeH
#define Unitpiece_typeH

#include <iosfwd>

enum piece_type
{
  bishop,
  king,
  knight,
  pawn,
  queen,
  rook
};

std::ostream& operator<<(std::ostream& os, const piece_type& type);

#endif
 
