
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

#ifndef UnitChessGameH
#define UnitChessGameH

#include "UnitEnumChessPieceColor.h"
#include "UnitEnumChessPieceType.h"
#include "UnitChessBoard.h"
#include "UnitChessMove.h"

//Manages the correct use of ChessBoard
struct ChessGame
{
  ChessGame();

  bool CanDoMove(const ChessMove& move) const;
  void DoMove(const ChessMove& move);

  ChessMove SuggestMove() const;

  void CoutGame() const;
  void CoutBoard() const;

  const ChessBoard& GetBoard() const { return mBoard; }
  EnumChessPieceColor GetWhoseTurn() const { return mWhoseTurn; }

  const std::vector<std::vector<bool> > GetInSight() const;

  bool IsGameOver() const;
  EnumChessPieceColor GetWinner() const;

  bool ParseMove(const std::string& s,ChessMove& move) const;

  private:
  ChessBoard mBoard;
  EnumChessPieceColor mWhoseTurn;

  std::vector<double> AttributeValues(
    const std::vector<ChessMove>& moves) const;
  double AttributeValue(
    const ChessMove& move) const;
};



#endif
 
