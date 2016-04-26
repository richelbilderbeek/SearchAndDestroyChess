#include "UnitChessMove.h"

bool operator==(const ChessMove& lhs, const ChessMove& rhs)
{
  return (lhs.type == rhs.type
    && lhs.x1      == rhs.x1
    && lhs.y1      == rhs.y1
    && lhs.capture == rhs.capture
    && lhs.x2      == rhs.x2
    && lhs.y2      == rhs.y2);
}


