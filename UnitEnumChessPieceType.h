#ifndef UnitEnumChessPieceTypeH
#define UnitEnumChessPieceTypeH

#include <iosfwd>

enum EnumChessPieceType
{
  bishop,
  king,
  knight,
  pawn,
  queen,
  rook
};

std::ostream& operator<<(std::ostream& os, const EnumChessPieceType& type);

#endif
 
