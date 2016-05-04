#ifndef UnitChessGameH
#define UnitChessGameH

#include "piece_color.h"
#include "piece_type.h"
#include "chess_board.h"
#include "chess_move.h"

//Manages the correct use of ChessBoard
struct chess_game
{
  chess_game();

  bool CanDoMove(const chess_move& move) const;
  void DoMove(const chess_move& move);

  chess_move SuggestMove() const;

  void CoutGame() const;
  void CoutBoard() const;

  const chess_board& GetBoard() const { return m_board; }
  piece_color GetWhoseTurn() const { return m_whose_turn; }

  const std::vector<std::vector<bool>> GetInSight() const;

  bool IsGameOver() const;
  piece_color GetWinner() const;

  bool ParseMove(const std::string& s,chess_move& move) const;

  private:
  chess_board m_board;
  piece_color m_whose_turn;

  std::vector<double> AttributeValues(
    const std::vector<chess_move>& moves) const;
  double AttributeValue(
    const chess_move& move) const;
};



#endif
