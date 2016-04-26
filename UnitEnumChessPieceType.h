#ifndef UnitEnumChessPieceTypeH
#define UnitEnumChessPieceTypeH

#include <iosfwd>

enum EnumChessPieceType
{
  pawn,
  knight,
  bishop,
  rook,
  queen,
  king
};

std::ostream& operator<<(std::ostream& os, const EnumChessPieceType& type);

#endif
 
