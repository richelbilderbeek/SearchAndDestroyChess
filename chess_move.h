#ifndef chess_moveH
#define chess_moveH

#include "piece_type.h"

struct chess_move
{
  chess_move(
    const piece_type anyType = king,
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
  piece_type type;
  int x1;
  int y1;
  bool capture;
  int x2;
  int y2;
};

bool operator==(const chess_move& lhs, const chess_move& rhs);
#endif
