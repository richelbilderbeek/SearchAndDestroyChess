#ifndef chess_boardH
#define chess_boardH

#include <iosfwd>
#include <vector>
#include "piece_color.h"
#include "chess_move.h"
#include "chess_piece.h"

//ChessBoard manages the position of the pieces
//It does not know whose turn it is (ChessGame does)
struct chess_board
{
  typedef std::vector<std::vector<chess_piece> > Board;

  chess_board();

  const Board& GetPieces() const { return m_pieces; }

  bool CanDoMove(const chess_move& move) const;
  void DoMove(const chess_move& move);
  chess_piece GetPiece(const int x, const int y) const;

  std::vector<std::vector<bool> > GetInSight(const piece_color color) const;

  std::vector<chess_move> GetAllValidMoves(const int x, const int y) const;
  std::vector<chess_move> GetAllPossibleMoves(const piece_color whoseTurn) const;

  void CoutPieces(const piece_color color) const;
  void CoutSight(const piece_color color) const;

  bool IsGameOver() const;
  piece_color GetWinner() const;

  private:

  Board m_pieces;
  std::vector<chess_move> m_moves;
  void SetPiece(const chess_piece& piece, const int x, const int y);


  const std::vector<std::vector<chess_piece> > GetInitialSetup();

  bool IsValidMove(const chess_move& move) const;

  std::vector<chess_move> GetAllValidMovesPawn(const int x, const int y) const;
  std::vector<chess_move> GetAllValidMovesKnight(const int x, const int y) const;
  std::vector<chess_move> GetAllValidMovesBishop(const int x, const int y) const;
  std::vector<chess_move> GetAllValidMovesRook(const int x, const int y) const;
  std::vector<chess_move> GetAllValidMovesQueen(const int x, const int y) const;
  std::vector<chess_move> GetAllValidMovesKing(const int x, const int y) const;

  bool CanDoCastlingShort(const piece_color color) const;
  bool CanDoCastlingLong(const piece_color color) const;
};

#endif
