
/*
  SearchAndDestroyChess 2, Kriegspiel/Dark Chess game
  Copyright (C) 2008  Richel Bilderbeek

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// From http://www.richelbilderbeek.nl

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
