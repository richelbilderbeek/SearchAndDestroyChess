#ifndef UnitChessMoveH
#define UnitChessMoveH

#include "UnitEnumChessPieceType.h"

struct ChessMove
{
  ChessMove(
    const EnumChessPieceType anyType = king,
    const int anyX1 = 0,
    const int anyY1 = 0,
    const bool anyCapture = true,
    const int anyX2 = 0,
    const int anyY2 = 0)
    : type(anyType),
      x1(anyX1),
      y1(anyY1),
      capture(anyCapture),
      x2(anyX2),
      y2(anyY2)
  {

  }
  EnumChessPieceType type;
  int x1;
  int y1;
  bool capture;
  int x2;
  int y2;
};

bool operator==(const ChessMove& lhs, const ChessMove& rhs);
#endif
