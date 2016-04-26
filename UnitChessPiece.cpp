#include <iostream>
#include "UnitChessPiece.h"

std::ostream& operator<<(std::ostream& os, const ChessPiece& piece)
{
  if (piece.IsNull()==true)
  {
    os << "  ";
  }
  else
  {
    os << piece.GetColor() << piece.GetType();
  }
  return os;
}

bool operator==(const ChessPiece& lhs, const ChessPiece& rhs)
{
  return (lhs.GetType() == rhs.GetType()
    && lhs.GetColor() == rhs.GetColor()
    && lhs.IsNull() == rhs.IsNull() );
}



