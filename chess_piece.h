#ifndef chess_pieceH
#define chess_pieceH

#include <iosfwd>
#include "piece_type.h"
#include "piece_color.h"

//Read-only
struct chess_piece
{
  chess_piece()
    : mColor(piece_color::white), mType(pawn), mIsNull(true) {}
  chess_piece(
    const piece_color color,
    const piece_type type)
    : mColor(color), mType(type), mIsNull(false) {}

  piece_color GetColor() const { return mColor; }
  piece_type GetType() const { return mType; }
  int IsNull() const { return mIsNull; }

  private:
    piece_color mColor;
    piece_type mType;
    bool mIsNull;
};

bool operator==(const chess_piece& lhs, const chess_piece& rhs);
std::ostream& operator<<(std::ostream& os, const chess_piece& piece);

#endif
