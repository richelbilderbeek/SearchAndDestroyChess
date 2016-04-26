#include "chess_move.h"

bool operator==(const chess_move& lhs, const chess_move& rhs)
{
  return (lhs.type == rhs.type
    && lhs.x1      == rhs.x1
    && lhs.y1      == rhs.y1
    && lhs.capture == rhs.capture
    && lhs.x2      == rhs.x2
    && lhs.y2      == rhs.y2);
}


