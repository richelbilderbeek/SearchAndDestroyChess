#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include "chess_piece.h"


#include "chess_board.h"

chess_board::chess_board()
  : m_pieces(GetInitialSetup()),
    m_moves{}
{

}

chess_piece chess_board::GetPiece(const int x, const int y) const
{
  return m_pieces[y][x];
}

void chess_board::SetPiece(const chess_piece& piece, const int x, const int y)
{
  m_pieces[y][x] = piece;
  assert(this->GetPiece(x,y) == piece);
}

bool chess_board::IsGameOver() const
{
  bool whiteHasKing = false;
  bool blackHasKing = false;

  for (int y=0; y!=8; ++y)
  {
    for (int x=0; x!=8; ++x)
    {
      const chess_piece piece = m_pieces[y][x];
      if (piece.IsNull()==false && piece.GetType()==king)
      {
        if (piece.GetColor() == piece_color::white)
          whiteHasKing = true;
        else
          blackHasKing = true;
      }
    }
  }
  return (whiteHasKing == false || blackHasKing == false);
}

piece_color chess_board::GetWinner() const
{
  assert(this->IsGameOver());
  for (int y=0; y!=8; ++y)
  {
    for (int x=0; x!=8; ++x)
    {
      const chess_piece piece = m_pieces[y][x];
      if (piece.GetType()==king)
      {
        if (piece.GetColor() == piece_color::white)
          return piece_color::white;
        else
          return piece_color::black;
      }
    }
  }
  assert(!"Should not get here");
  throw std::logic_error("Cannot find winner");
}

bool chess_board::CanDoMove(const chess_move& move) const
{
  const chess_piece piece = this->GetPiece(move.x1,move.y1);
  //Is there a chesspiece?
  if (piece.IsNull()) return false;
  //Is it of the right type?
  if (piece.GetType()!=move.type) return false;
  //Is it of the right color? ->Checked by ChessGame
  //Is the destination free or is there an enemy?
  const chess_piece destination = this->GetPiece(move.x2,move.y2);
  if ( destination.IsNull()
    && move.capture == true) return false; //Free space && capture
  if ( destination.IsNull()==false)
  {
    //Cannot capture own pieces
    if (destination.GetColor()==piece.GetColor())
      return false;
    if (destination.GetColor()!=piece.GetColor()
      && move.capture == false) return false; //An enemy && no capture
  }
  return this->IsValidMove(move);
}

void chess_board::DoMove(const chess_move& move)
{
  assert(this->CanDoMove(move));
  const chess_piece piece = this->GetPiece(move.x1,move.y1);

  if (piece.GetType()==king
    && move.x1 == 4
    && (move.x2 == 2 || move.x2 == 6) )
  {
    const piece_color color = piece.GetColor();
    assert( (color == piece_color::white && move.y1 == 0 && move.y2 == 0)
      ||    (color == piece_color::black && move.y1 == 7 && move.y2 == 7) );
    const int y = move.y2;
    //Castling
    if (move.x2 == 2)
    {
      //Castling long
      this->SetPiece(chess_piece(color,king),2,y); //Move King
      this->SetPiece(chess_piece(color,rook),3,y); //Move rook
      this->SetPiece(chess_piece(),4,y);           //Erase old king
      this->SetPiece(chess_piece(),0,y);           //Erase old rook
    }
    else
    {
      //Castling short
      this->SetPiece(chess_piece(color,king),6,y); //Move King
      this->SetPiece(chess_piece(color,rook),5,y); //Move rook
      this->SetPiece(chess_piece(),4,y);           //Erase old king
      this->SetPiece(chess_piece(),7,y);           //Erase old rook
    }
  }
  else if (piece.GetType()==pawn
    && this->GetPiece(move.x2,move.y2).IsNull()
    && move.x1 != move.x2
    && move.y1 != move.y2)
  {
    //En passant
    this->SetPiece(chess_piece(piece.GetColor(),pawn),move.x2,move.y2); //Move pawn
    this->SetPiece(chess_piece(),move.x1,move.y1); //Erase old pawn
    this->SetPiece(chess_piece(),move.x2,move.y1); //Capture pawn
  }
  else if (piece.GetType()==pawn
    && (move.y2 == 0 || move.y2 == 7) )
  {
    //Promotion to queen
    this->SetPiece(chess_piece(),move.x1,move.y1); //Overwrite old piece by NullType
    this->SetPiece(chess_piece(piece.GetColor(),queen),move.x2,move.y2);
  }
  else
  {
    //Regular move (i.e. non-castling)
    this->SetPiece(chess_piece(),move.x1,move.y1); //Overwrite old piece by NullType
    this->SetPiece(piece,move.x2,move.y2);
  }
  m_moves.push_back(move);
}

bool chess_board::CanDoCastlingShort(const piece_color color) const
{
  //Determine the y
  const int y = (color == piece_color::white ? 0 : 7);
  //King must be in place
  if(this->GetPiece(4,y).GetType() != king) return false;
  //Is the rook in place?
  if(this->GetPiece(7,y).GetType() != rook) return false;
  //Is nothing in between?
  if(this->GetPiece(5,y).IsNull() == false) return false;
  if(this->GetPiece(6,y).IsNull() == false) return false;

  typedef std::vector<chess_move>::const_iterator Iter;
  const Iter j = m_moves.end();
  for (Iter i = m_moves.begin(); i!=j; ++i)
  {
    //Check if king has moved
    if ( (*i).type == king && (*i).y1 == y) return false;
    //Check if kingside rook has moved
    if ( (*i).type == rook && (*i).y1 == y && (*i).x1 == 7) return false;
  }
  return true;
}

bool chess_board::CanDoCastlingLong(const piece_color color) const
{
  //Determine the y
  const int y = (color == piece_color::white ? 0 : 7);
  //King must be in place
  if(this->GetPiece(4,y).GetType() != king) return false;
  //Is the rook in place?
  if(this->GetPiece(0,y).GetType() != rook) return false;
  //Is nothing in between?
  if(this->GetPiece(1,y).IsNull() == false) return false;
  if(this->GetPiece(2,y).IsNull() == false) return false;
  if(this->GetPiece(3,y).IsNull() == false) return false;

  typedef std::vector<chess_move>::const_iterator Iter;
  const Iter j = m_moves.end();
  for (Iter i = m_moves.begin(); i!=j; ++i)
  {
    //Check if king has moved
    if ( (*i).type == king && (*i).y1 == y) return false;
    //Check if kingside rook has moved
    if ( (*i).type == rook && (*i).y1 == y && (*i).x1 == 0) return false;
  }
  return true;
}

const std::vector<std::vector<chess_piece> > chess_board::GetInitialSetup()
{
  std::vector<std::vector<chess_piece> > v(8,std::vector<chess_piece>(8));
  //v has [y][x] index
  v[0][0] = chess_piece(piece_color::white,rook  );
  v[0][1] = chess_piece(piece_color::white,knight);
  v[0][2] = chess_piece(piece_color::white,bishop);
  v[0][3] = chess_piece(piece_color::white,queen );
  v[0][4] = chess_piece(piece_color::white,king  );
  v[0][5] = chess_piece(piece_color::white,bishop);
  v[0][6] = chess_piece(piece_color::white,knight);
  v[0][7] = chess_piece(piece_color::white,rook  );
  v[1][0] = chess_piece(piece_color::white,pawn  );
  v[1][1] = chess_piece(piece_color::white,pawn  );
  v[1][2] = chess_piece(piece_color::white,pawn  );
  v[1][3] = chess_piece(piece_color::white,pawn  );
  v[1][4] = chess_piece(piece_color::white,pawn  );
  v[1][5] = chess_piece(piece_color::white,pawn  );
  v[1][6] = chess_piece(piece_color::white,pawn  );
  v[1][7] = chess_piece(piece_color::white,pawn  );
  v[7][0] = chess_piece(piece_color::black,rook  );
  v[7][1] = chess_piece(piece_color::black,knight);
  v[7][2] = chess_piece(piece_color::black,bishop);
  v[7][3] = chess_piece(piece_color::black,queen );
  v[7][4] = chess_piece(piece_color::black,king  );
  v[7][5] = chess_piece(piece_color::black,bishop);
  v[7][6] = chess_piece(piece_color::black,knight);
  v[7][7] = chess_piece(piece_color::black,rook  );
  v[6][0] = chess_piece(piece_color::black,pawn  );
  v[6][1] = chess_piece(piece_color::black,pawn  );
  v[6][2] = chess_piece(piece_color::black,pawn  );
  v[6][3] = chess_piece(piece_color::black,pawn  );
  v[6][4] = chess_piece(piece_color::black,pawn  );
  v[6][5] = chess_piece(piece_color::black,pawn  );
  v[6][6] = chess_piece(piece_color::black,pawn  );
  v[6][7] = chess_piece(piece_color::black,pawn  );
  return v;
}

//Color denotes the player who's turn it is, i.e. the player looking at the board
void chess_board::CoutPieces(
  const piece_color color) const
{
  const int yBegin = (color == piece_color::black ? 0 : 7);
  const int yEnd   = (color == piece_color::black ? 8 : -1);
  const int yStep  = (color == piece_color::black ? 1 : -1);
  const int xBegin = (color == piece_color::white ? 0 : 7);
  const int xEnd   = (color == piece_color::white ? 8 : -1);
  const int xStep  = (color == piece_color::white ? 1 : -1);

  if (color == piece_color::white)
    std::cout << "   A  B  C  D  E  F  G  H " << std::endl;
  else
    std::cout << "   H  G  F  E  D  C  B  A " << std::endl;

  for (int y=yBegin; y!=yEnd; y+=yStep)
  {
    std::cout << "  -------------------------" << std::endl;
    std::cout << (y+1) << " ";
    for (int x=xBegin; x!=xEnd; x+=xStep)
    {
      std::cout << "|" << m_pieces[y][x];
    }
    std::cout << "| " << (y+1) << std::endl;

  }
  std::cout << "  -------------------------" << std::endl;
  if (color == piece_color::white)
    std::cout << "   A  B  C  D  E  F  G  H " << std::endl;
  else
    std::cout << "   H  G  F  E  D  C  B  A " << std::endl;

}

//Color denotes the player who's turn it is, i.e. the player looking at the board
void chess_board::CoutSight(
  const piece_color color) const
{
  //
  const std::vector<std::vector<bool> > inSight = this->GetInSight(color);

  const int yBegin = (color == piece_color::black ? 0 : 7);
  const int yEnd   = (color == piece_color::black ? 8 : -1);
  const int yStep  = (color == piece_color::black ? 1 : -1);
  const int xBegin = (color == piece_color::white ? 0 : 7);
  const int xEnd   = (color == piece_color::white ? 8 : -1);
  const int xStep  = (color == piece_color::white ? 1 : -1);

  if (color == piece_color::white)
    std::cout << "   A  B  C  D  E  F  G  H " << std::endl;
  else
    std::cout << "   H  G  F  E  D  C  B  A " << std::endl;

  for (int y=yBegin; y!=yEnd; y+=yStep)
  {
    std::cout << "  -------------------------" << std::endl;
    std::cout << (y+1) << " ";
    for (int x=xBegin; x!=xEnd; x+=xStep)
    {
      std::cout << "|" << (inSight[y][x] ? m_pieces[y][x] : chess_piece() );
    }
    std::cout << "| " << (y+1) << std::endl;

  }
  std::cout << "  -------------------------" << std::endl;
  if (color == piece_color::white)
    std::cout << "   A  B  C  D  E  F  G  H " << std::endl;
  else
    std::cout << "   H  G  F  E  D  C  B  A " << std::endl;


}

std::vector<std::vector<bool>> chess_board::GetInSight(const piece_color color) const
{
  std::vector<std::vector<bool>> inSight(8, std::vector<bool>(8,false));

  for (int y=0; y!=8; ++y)
  {
    for (int x=0; x!=8; ++x)
    {
      //Get the piece there
      const chess_piece piece = this->GetPiece(x,y);
      //Empty or occupied by enemy? Then continue
      if (piece.IsNull() || piece.GetColor()!=color) continue;
      //Occupied by this color
      inSight[y][x] = true;
      //Then get all its valid moves
      const std::vector<chess_move> moves = this->GetAllValidMoves(x,y);
      const std::vector<chess_move>::const_iterator j = moves.end();
      for (std::vector<chess_move>::const_iterator i = moves.begin(); i!=j; ++i)
      {
        inSight[(*i).y2][(*i).x2] = true;
      }
      //Special treatment of pawns
      if (piece.GetType() == pawn)
      {
        if (piece.GetColor()==piece_color::white)
        {
          //Always look a single square forward
          inSight[y+1][x] = true;
          //Look two squares forward, if the first is not occupied
          if (y==1 && GetPiece(x,y+1).IsNull())
          {
            inSight[y+2][x] = true;
          }
          //Look sideways left
          if (x > 0 && GetPiece(x-1,y+1).IsNull())
          {
            inSight[y+1][x-1] = true;
          }
          //Look sideways right
          if (x < 7 && GetPiece(x+1,y+1).IsNull())
          {
            inSight[y+1][x+1] = true;
          }
          //Look sideways if en passant is possible
          if (y == 4                            //4th row
            && this->m_moves.back().type == pawn //Black moved a pawn
            && this->m_moves.back().y1 == 6      //Black moved two places
            && this->m_moves.back().y2 == 4)     //Black moved two places
          {
            if (this->m_moves.back().x1 == x-1)
              inSight[y][x-1] = true;
            else if (this->m_moves.back().x1 == x+1)
              inSight[y][x+1] = true;
          }
        }
        else
        {
          //Always look a single square forward
          inSight[y-1][x] = true;
          //Look two squares forward, if the first is not occupied
          if (y==6 && GetPiece(x,y-1).IsNull())
          {
            inSight[y-2][x] = true;
          }
          //Look sideways left
          if (x > 0 && GetPiece(x-1,y-1).IsNull())
          {
            inSight[y-1][x-1] = true;
          }
          //Look sideways right
          if (x < 7 && GetPiece(x+1,y-1).IsNull())
          {
            inSight[y-1][x+1] = true;
          }
          //Look sideways if en passant is possible
          if (y == 3                            //3rd row
            && this->m_moves.back().type == pawn //White moved a pawn
            && this->m_moves.back().y1 == 1      //White moved two places
            && this->m_moves.back().y2 == 3)     //White moved two places
          {
            if (this->m_moves.back().x1 == x-1)
              inSight[y][x-1] = true;
            else if (this->m_moves.back().x1 == x+1)
              inSight[y][x+1] = true;
          }
        }
      }
    }
  }
  return inSight;
}

bool chess_board::IsValidMove(const chess_move& move) const
{
  const std::vector<chess_move> moves = this->GetAllValidMoves(move.x1, move.y1);
  return std::find(std::begin(moves),std::end(moves), move) != std::end(moves);
}

std::vector<chess_move> chess_board::GetAllPossibleMoves(
  const piece_color whoseTurn) const
{
  std::vector<chess_move> allMoves;
  for (int y=0; y!=8; ++y)
  {
    for (int x=0; x!=8; ++x)
    {
      if (this->GetPiece(x,y).IsNull() == false 
       && this->GetPiece(x,y).GetColor()==whoseTurn)
      {
        //Get all valid moves of this player's piece here
        const std::vector<chess_move> thisPieceMoves = GetAllValidMoves(x,y);
        //Append it to allMoves
        std::copy(
          thisPieceMoves.begin(),thisPieceMoves.end(),
          std::back_inserter(allMoves));
      }
    }
  }
  return allMoves;
}

std::vector<chess_move> chess_board::GetAllValidMoves(
  const int x, const int y) const
{
  const chess_piece piece = GetPiece(x,y);
  assert(piece.IsNull()==false);
  switch (piece.GetType())
  {
    case pawn  : return GetAllValidMovesPawn(x,y);
    case knight: return GetAllValidMovesKnight(x,y);
    case bishop: return GetAllValidMovesBishop(x,y);
    case rook  : return GetAllValidMovesRook(x,y);
    case queen : return GetAllValidMovesQueen(x,y);
    case king  : return GetAllValidMovesKing(x,y);
  }
  assert(!"Should not get here");
  throw std::logic_error("Unknown piece_type");
}

std::vector<chess_move> chess_board::GetAllValidMovesPawn(
  const int x, const int y) const
{
  const chess_piece piece = GetPiece(x,y);
  assert(piece.IsNull()==false);
  assert(piece.GetType() == pawn);

  std::vector<chess_move> moves;

  if (piece.GetColor()==piece_color::white)
  {
    //Move single square forward
    if (GetPiece(x,y+1).IsNull())
    {
      moves.push_back(chess_move(pawn,x,y,false,x,y+1));
    }
    //Move two squares forward
    if (y==1
      && GetPiece(x,y+1).IsNull()
      && GetPiece(x,y+2).IsNull())
    {
      moves.push_back(chess_move(pawn,x,y,false,x,y+2));
    }
    //Capture left
    if (x > 0
      && GetPiece(x-1,y+1).IsNull()==false
      && GetPiece(x-1,y+1).GetColor()==piece_color::black)
    {
      moves.push_back(chess_move(pawn,x,y,true,x-1,y+1));
    }
    //Capture right
    if (x < 7
      && GetPiece(x+1,y+1).IsNull()==false
      && GetPiece(x+1,y+1).GetColor()==piece_color::black)
    {
      moves.push_back(chess_move(pawn,x,y,true,x+1,y+1));
    }
    //En passant
    if (y == 4
      && m_moves.back().y1 == 6 //Black had moved two squares
      && m_moves.back().y2 == 4)
    {
      if (m_moves.back().x1 == x - 1) //To the left
      {
        //Note: En passant is not regarded as a capture, as the spot moved to is empty
        moves.push_back(chess_move(pawn,x,y, false /* note */ ,x-1,y+1));
      }
      else if (m_moves.back().x1 == x + 1) //To the right
      {
        //Note: En passant is not regarded as a capture, as the spot moved to is empty
        moves.push_back(chess_move(pawn,x,y,false /* note */,x+1,y+1));
      }
    }
  }
  else
  {
    //Move single square forward
    if (GetPiece(x,y-1).IsNull())
    {
      moves.push_back(chess_move(pawn,x,y,false,x,y-1));
    }
    //Move two squares forward
    if (y==6
      && GetPiece(x,y-1).IsNull()
      && GetPiece(x,y-2).IsNull())
    {
      moves.push_back(chess_move(pawn,x,y,false,x,y-2));
    }
    //Capture left
    if (x > 0
      && GetPiece(x-1,y-1).IsNull()==false
      && GetPiece(x-1,y-1).GetColor()==piece_color::white)
    {
      moves.push_back(chess_move(pawn,x,y,true,x-1,y-1));
    }
    //Capture right
    if (x < 7
      && GetPiece(x+1,y-1).IsNull()==false
      && GetPiece(x+1,y-1).GetColor()==piece_color::white)
    {
      moves.push_back(chess_move(pawn,x,y,true,x+1,y-1));
    }
    //En passant
    if (y == 3
      && m_moves.back().y1 == 1  //White had moved two squares
      && m_moves.back().y2 == 3)
    {
      if (m_moves.back().x1 == x - 1) //To the left
      {
        //Note: En passant is not regarded as a capture, as the spot moved to is empty
        moves.push_back(chess_move(pawn,x,y, false /* note */ ,x-1,y-1));
      }
      else if (m_moves.back().x1 == x + 1) //To the right
      {
        //Note: En passant is not regarded as a capture, as the spot moved to is empty
        moves.push_back(chess_move(pawn,x,y,false /* note */,x+1,y-1));
      }
    }
  }
  return moves;
}

std::vector<chess_move> chess_board::GetAllValidMovesKnight(
  const int x, const int y) const
{
  const chess_piece piece = GetPiece(x,y);
  assert(piece.IsNull()==false);
  assert(piece.GetType() == knight);

  std::vector<chess_move> moves;

  //-2 -1
  if (x > 1 && y > 0)
  {
    //Is it a capture?
    if (GetPiece(x-2,y-1).IsNull() == true)
    {
      moves.push_back(chess_move(knight,x,y,false,x-2,y-1)); //No capture
    }
    else if (GetPiece(x-2,y-1).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(knight,x,y,true,x-2,y-1)); //Capture of enemy
    }
  }
  //-1 -2
  if (x > 0 && y > 1)
  {
    //Is it a capture?
    if (GetPiece(x-1,y-2).IsNull() == true)
    {
      moves.push_back(chess_move(knight,x,y,false,x-1,y-2)); //No capture
    }
    else if (GetPiece(x-1,y-2).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(knight,x,y,true,x-1,y-2)); //Capture of enemy
    }
  }
  //+2 -1
  if (x < 6 && y > 0)
  {
    //Is it a capture?
    if (GetPiece(x+2,y-1).IsNull() == true)
    {
      moves.push_back(chess_move(knight,x,y,false,x+2,y-1)); //No capture
    }
    else if (GetPiece(x+2,y-1).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(knight,x,y,true,x+2,y-1)); //Capture of enemy
    }
  }
  //-1 +2
  if (x > 0 && y < 6)
  {
    //Is it a capture?
    if (GetPiece(x-1,y+2).IsNull() == true)
    {
      moves.push_back(chess_move(knight,x,y,false,x-1,y+2)); //No capture
    }
    else if (GetPiece(x-1,y+2).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(knight,x,y,true,x-1,y+2)); //Capture of enemy
    }
  }
  //-2 +1
  if (x > 1 && y < 7)
  {
    //Is it a capture?
    if (GetPiece(x-2,y+1).IsNull() == true)
    {
      moves.push_back(chess_move(knight,x,y,false,x-2,y+1)); //No capture
    }
    else if (GetPiece(x-2,y+1).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(knight,x,y,true,x-2,y+1)); //Capture of enemy
    }
  }
  //+1 -2
  if (x < 7 && y > 1)
  {
    //Is it a capture?
    if (GetPiece(x+1,y-2).IsNull() == true)
    {
      moves.push_back(chess_move(knight,x,y,false,x+1,y-2)); //No capture
    }
    else if (GetPiece(x+1,y-2).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(knight,x,y,true,x+1,y-2)); //Capture of enemy
    }
  }

  //+2 +1
  if (x < 6 && y < 7)
  {
    //Is it a capture?
    if (GetPiece(x+2,y+1).IsNull() == true)
    {
      moves.push_back(chess_move(knight,x,y,false,x+2,y+1)); //No capture
    }
    else if (GetPiece(x+2,y+1).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(knight,x,y,true,x+2,y+1)); //Capture of enemy
    }
  }
  //+1 +2
  if (x < 7 && y < 6)
  {
    //Is it a capture?
    if (GetPiece(x+1,y+2).IsNull() == true)
    {
      moves.push_back(chess_move(knight,x,y,false,x+1,y+2)); //No capture
    }
    else if (GetPiece(x+1,y+2).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(knight,x,y,true,x+1,y+2)); //Capture of enemy
    }
  }

  return moves;
}

std::vector<chess_move> chess_board::GetAllValidMovesBishop(
  const int x, const int y) const
{
  const chess_piece piece = GetPiece(x,y);
  assert(piece.IsNull()==false);
  assert(piece.GetType() == bishop);

  std::vector<chess_move> moves;

  //+x +x
  for (int i=1; x+i<8 && y+i<8; ++i)
  {
    //Is it a capture
    if (GetPiece(x+i,y+i).IsNull() == true)
    {
      moves.push_back(chess_move(bishop,x,y,false,x+i,y+i)); //No capture
    }
    else if (GetPiece(x+i,y+i).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(bishop,x,y,true,x+i,y+i)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  //-x +x
  for (int i=1; x-i>-1 && y+i<8; ++i)
  {
    //Is it a capture
    if (GetPiece(x-i,y+i).IsNull() == true)
    {
      moves.push_back(chess_move(bishop,x,y,false,x-i,y+i)); //No capture
    }
    else if (GetPiece(x-i,y+i).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(bishop,x,y,true,x-i,y+i)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  //+x -x
  for (int i=1; x+i<8 && y-i>-1; ++i)
  {
    //Is it a capture
    if (GetPiece(x+i,y-i).IsNull() == true)
    {
      moves.push_back(chess_move(bishop,x,y,false,x+i,y-i)); //No capture
    }
    else if (GetPiece(x+i,y-i).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(bishop,x,y,true,x+i,y-i)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  //-x -x
  for (int i=1; x-i>-1 && y-i>-1; ++i)
  {
    //Is it a capture
    if (GetPiece(x-i,y-i).IsNull() == true)
    {
      moves.push_back(chess_move(bishop,x,y,false,x-i,y-i)); //No capture
    }
    else if (GetPiece(x-i,y-i).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(bishop,x,y,true,x-i,y-i)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  return moves;
}

std::vector<chess_move> chess_board::GetAllValidMovesRook(
  const int x, const int y) const
{
  const chess_piece piece = GetPiece(x,y);
  assert(piece.IsNull()==false);
  assert(piece.GetType() == rook);

  std::vector<chess_move> moves;

  //+x +0
  for (int i=1; x+i<8; ++i)
  {
    //Is it a capture
    if (GetPiece(x+i,y+0).IsNull() == true)
    {
      moves.push_back(chess_move(rook,x,y,false,x+i,y+0)); //No capture
    }
    else if (GetPiece(x+i,y+0).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(rook,x,y,true,x+i,y+0)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }
  //-x +0
  for (int i=1; x-i>-1; ++i)
  {
    //Is it a capture
    if (GetPiece(x-i,y+0).IsNull() == true)
    {
      moves.push_back(chess_move(rook,x,y,false,x-i,y+0)); //No capture
    }
    else if (GetPiece(x-i,y+0).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(rook,x,y,true,x-i,y+0)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  //+0 +x
  for (int i=1; y+i<8; ++i)
  {
    //Is it a capture
    if (GetPiece(x+0,y+i).IsNull() == true)
    {
      moves.push_back(chess_move(rook,x,y,false,x+0,y+i)); //No capture
    }
    else if (GetPiece(x+0,y+i).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(rook,x,y,true,x+0,y+i)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  //+0 -x
  for (int i=1; y-i>-1; ++i)
  {
    //Is it a capture
    if (GetPiece(x-0,y-i).IsNull() == true)
    {
      moves.push_back(chess_move(rook,x,y,false,x-0,y-i)); //No capture
    }
    else if (GetPiece(x-0,y-i).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(rook,x,y,true,x-0,y-i)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  return moves;
}

std::vector<chess_move> chess_board::GetAllValidMovesQueen(
  const int x, const int y) const
{
  const chess_piece piece = GetPiece(x,y);
  assert(piece.IsNull()==false);
  assert(piece.GetType() == queen);

  std::vector<chess_move> moves;


  //+x +x
  for (int i=1; x+i<8 && y+i<8; ++i)
  {
    //Is it a capture
    if (GetPiece(x+i,y+i).IsNull() == true)
    {
      moves.push_back(chess_move(queen,x,y,false,x+i,y+i)); //No capture
    }
    else if (GetPiece(x+i,y+i).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(queen,x,y,true,x+i,y+i)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  //-x +x
  for (int i=1; x-i>-1 && y+i<8; ++i)
  {
    //Is it a capture
    if (GetPiece(x-i,y+i).IsNull() == true)
    {
      moves.push_back(chess_move(queen,x,y,false,x-i,y+i)); //No capture
    }
    else if (GetPiece(x-i,y+i).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(queen,x,y,true,x-i,y+i)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  //+x -x
  for (int i=1; x+i<8 && y-i>-1; ++i)
  {
    //Is it a capture
    if (GetPiece(x+i,y-i).IsNull() == true)
    {
      moves.push_back(chess_move(queen,x,y,false,x+i,y-i)); //No capture
    }
    else if (GetPiece(x+i,y-i).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(queen,x,y,true,x+i,y-i)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  //-x -x
  for (int i=1; x-i>-1 && y-i>-1; ++i)
  {
    //Is it a capture
    if (GetPiece(x-i,y-i).IsNull() == true)
    {
      moves.push_back(chess_move(queen,x,y,false,x-i,y-i)); //No capture
    }
    else if (GetPiece(x-i,y-i).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(queen,x,y,true,x-i,y-i)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  //+x +0
  for (int i=1; x+i<8; ++i)
  {
    //Is it a capture
    if (GetPiece(x+i,y+0).IsNull() == true)
    {
      moves.push_back(chess_move(queen,x,y,false,x+i,y+0)); //No capture
    }
    else if (GetPiece(x+i,y+0).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(queen,x,y,true,x+i,y+0)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }
  //-x +0
  for (int i=1; x-i>-1; ++i)
  {
    //Is it a capture
    if (GetPiece(x-i,y+0).IsNull() == true)
    {
      moves.push_back(chess_move(queen,x,y,false,x-i,y+0)); //No capture
    }
    else if (GetPiece(x-i,y+0).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(queen,x,y,true,x-i,y+0)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  //+0 +x
  for (int i=1; y+i<8; ++i)
  {
    //Is it a capture
    if (GetPiece(x+0,y+i).IsNull() == true)
    {
      moves.push_back(chess_move(queen,x,y,false,x+0,y+i)); //No capture
    }
    else if (GetPiece(x+0,y+i).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(queen,x,y,true,x+0,y+i)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  //+0 -x
  for (int i=1; y-i>-1; ++i)
  {
    //Is it a capture
    if (GetPiece(x-0,y-i).IsNull() == true)
    {
      moves.push_back(chess_move(queen,x,y,false,x-0,y-i)); //No capture
    }
    else if (GetPiece(x-0,y-i).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(queen,x,y,true,x-0,y-i)); //Capture of enemy
      break;
    }
    else
    {
      break; //Own color
    }
  }

  return moves;
}

std::vector<chess_move> chess_board::GetAllValidMovesKing(
  const int x, const int y) const
{
  const chess_piece piece = GetPiece(x,y);
  assert(piece.IsNull()==false);
  assert(piece.GetType() == king);

  std::vector<chess_move> moves;

  //+0 -1
  if (y > 0)
  {
    //Is it a capture
    if (GetPiece(x+0,y-1).IsNull() == true)
    {
      moves.push_back(chess_move(king,x,y,false,x+0,y-1)); //No capture
    }
    else if (GetPiece(x+0,y-1).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(king,x,y,true,x+0,y-1)); //Capture of enemy
    }
  }
  //-1 -1
  if (y > 0 && x > 0)
  {
    //Is it a capture
    if (GetPiece(x-1,y-1).IsNull() == true)
    {
      moves.push_back(chess_move(king,x,y,false,x-1,y-1)); //No capture
    }
    else if (GetPiece(x-1,y-1).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(king,x,y,true,x-1,y-1)); //Capture of enemy
    }
  }
  //+1 -1
  if (y > 0 && x < 7)
  {
    //Is it a capture
    if (GetPiece(x+1,y-1).IsNull() == true)
    {
      moves.push_back(chess_move(king,x,y,false,x+1,y-1)); //No capture
    }
    else if (GetPiece(x+1,y-1).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(king,x,y,true,x+1,y-1)); //Capture of enemy
    }
  }


  //+0 +1
  if (y < 7)
  {
    //Is it a capture
    if (GetPiece(x+0,y+1).IsNull() == true)
    {
      moves.push_back(chess_move(king,x,y,false,x+0,y+1)); //No capture
    }
    else if (GetPiece(x+0,y+1).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(king,x,y,true,x+0,y+1)); //Capture of enemy
    }
  }
  //-1 +1
  if (y < 7 && x > 0)
  {
    //Is it a capture
    if (GetPiece(x-1,y+1).IsNull() == true)
    {
      moves.push_back(chess_move(king,x,y,false,x-1,y+1)); //No capture
    }
    else if (GetPiece(x-1,y+1).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(king,x,y,true,x-1,y+1)); //Capture of enemy
    }
  }
  //+1 +1
  if (y < 7 && x < 7)
  {
    //Is it a capture
    if (GetPiece(x+1,y+1).IsNull() == true)
    {
      moves.push_back(chess_move(king,x,y,false,x+1,y+1)); //No capture
    }
    else if (GetPiece(x+1,y+1).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(king,x,y,true,x+1,y+1)); //Capture of enemy
    }
  }


  //-1 +0
  if (x > 0)
  {
    //Is it a capture
    if (GetPiece(x-1,y+0).IsNull() == true)
    {
      moves.push_back(chess_move(king,x,y,false,x-1,y+0)); //No capture
    }
    else if (GetPiece(x-1,y+0).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(king,x,y,true,x-1,y+0)); //Capture of enemy
    }
  }
  //+1 +1
  if (x < 7)
  {
    //Is it a capture
    if (GetPiece(x+1,y+0).IsNull() == true)
    {
      moves.push_back(chess_move(king,x,y,false,x+1,y+0)); //No capture
    }
    else if (GetPiece(x+1,y+0).GetColor() != piece.GetColor())
    { //Is it an enemy?
      moves.push_back(chess_move(king,x,y,true,x+1,y+0)); //Capture of enemy
    }
  }

  //Can do castling?
  if (this->CanDoCastlingLong(piece.GetColor()))
    moves.push_back(chess_move(king,x,y,false,x-2,y));
  if (this->CanDoCastlingShort(piece.GetColor()))
    moves.push_back(chess_move(king,x,y,false,x+2,y));

  return moves;
}







