#include <iostream>
#include "chess_piece.h"

std::ostream& operator<<(std::ostream& os, const chess_piece& piece)
{
  if (piece.IsNull())
  {
    os << "  ";
  }
  else
  {
    os << piece.GetColor() << piece.GetType();
  }
  return os;
}

bool operator==(const chess_piece& lhs, const chess_piece& rhs)
{
  return (lhs.GetType() == rhs.GetType()
    && lhs.GetColor() == rhs.GetColor()
    && lhs.IsNull() == rhs.IsNull() );
}



