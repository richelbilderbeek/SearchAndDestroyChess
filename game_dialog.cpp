#include "game_dialog.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <cstdlib>
#include <ctime>
#include <cassert>
#include <vector>
#include <stdexcept>
#include <boost/shared_ptr.hpp>
#include "UnitChessBoard.h"
#include "UnitChessPiece.h"
#include "UnitCoordinatGetter.h"

game_dialog::game_dialog(
  const int window_width,
  const int window_height
)
  : m_cursor_x{3},
    m_cursor_y(3),
    m_game_state{game_state::start_turn},
    m_select_x{-1},
    m_select_y{-1},
    m_window_height{window_height},
    m_window_width{window_width}
{

}

void game_dialog::add_command(const command c)
{
  m_commands.insert(c);
}

void game_dialog::do_move()
{
  const int x1 = m_select_x;
  const int y1 = m_select_y;
  const int x2 = m_cursor_x;
  const int y2 = m_cursor_y;
  if (m_select_x == -1) return;
  if (m_cursor_x == -1) return;
  assert(x1 >=0);
  assert(x2 >=0);
  assert(x1 < 8);
  assert(x2 < 8);
  assert(y1 >=0);
  assert(y2 >=0);
  assert(y1 < 8);
  assert(y2 < 8);
  const ChessPiece piece = m_game.GetBoard().GetPiece(x1,y1);
  assert(piece.IsNull()==false
    && "User must not be able to select an empty square do do a move from");
  const EnumChessPieceType type = piece.GetType();
  const ChessPiece victim = m_game.GetBoard().GetPiece(x2,y2);
  const bool capture = (victim.IsNull()==true ? false : true);
  const ChessMove move(type,x1,y1,capture,x2,y2);
  this->do_move(move);
  m_select_x = -1;
  m_select_y = -1;
}

void game_dialog::do_move(const ChessMove& move)
{
  if (!m_game.CanDoMove(move))
  {
    m_game_state = game_state::draw;
    return;
  }

  //A -possibly deciding- move takes place
  m_game.DoMove(move);

  //Has someone won?
  if (m_game.IsGameOver())
  {
    m_game_state = m_game.GetWinner() == white ? game_state::player_1_won : game_state::player_2_won;
    return;
  }

  //Go to the next turn
  m_game_state = game_state::start_turn;
  m_cursor_x = std::rand() % 8;
  m_cursor_y = std::rand() % 8;
}

void game_dialog::do_select(const int cursorX, const int cursorY)
{
  if ( m_select_x == -1
    && m_select_y == -1
    && m_game.GetBoard().GetPiece(cursorX,cursorY).IsNull()==false)
  {
    //Nothing selected until now
    m_select_x = cursorX;
    m_select_y = cursorY;
  }
  else if (
       m_select_x != -1
    && m_select_y != -1
    && m_game.GetBoard().GetPiece(m_select_x,m_select_y).IsNull() == false
    && m_game.GetBoard().GetPiece(m_select_x,m_select_y).GetColor() == m_game.GetWhoseTurn()
    && m_game.GetBoard().GetPiece(cursorX,cursorY).IsNull() == false
    && m_game.GetBoard().GetPiece(cursorX,cursorY).GetColor() == m_game.GetWhoseTurn() )
  {
    //Selected a piece of own color and now selecting another piece
    //of own color
    m_select_x = cursorX;
    m_select_y = cursorY;
  }
  else
  {
    do_move();
  }
}

void game_dialog::draw(sf::RenderWindow& window)
{
  const std::vector<std::vector<bool>> inSight = m_game.GetInSight();

  const double squareWidth  = static_cast<double>(window.getSize().x) / 8.0;
  const double squareHeight = static_cast<double>(window.getSize().y) / 8.0;
  //The CoordinatGetter transforms the coordinats of the board,
  //according to the player whose turn it is
  const CoordinatGetter coordinatGetter ( m_game.GetWhoseTurn());

  //Draw squares, question marks and pieces
  {
    const ChessBoard board = m_game.GetBoard();

    //Draw all squares on buffer (clearing it)
    for (int y=0; y!=8; ++y)
    {
      for (int x=0; x!=8; ++x)
      {
        sf::Texture& t = m_textures.get_square((x + y) % 2 == 0 ? black : white);

        //Draw it
        ImageBuffer->Canvas->StretchDraw(
          TRect( TPoint( ( coordinatGetter.GetX(x) + 0 ) * squareWidth ,
                         ( coordinatGetter.GetY(y) + 0 ) * squareHeight),
                 TPoint( ( coordinatGetter.GetX(x) + 1 ) * squareWidth ,
                         ( coordinatGetter.GetY(y) + 1 ) * squareHeight)
            ), image->Picture->Graphic);
      }
    }

    //Draw all question marks and pieces
    for (int y=0; y!=8; ++y)
    {
      for (int x=0; x!=8; ++x)
      {
        const TImage * const image = GetImage(inSight[y][x],board.GetPiece(x,y));
        if( image == 0) continue;
        //Draw it
        ImageBuffer->Canvas->StretchDraw(
          TRect( TPoint( ( coordinatGetter.GetX(x) + 0 ) * squareWidth ,
                         ( coordinatGetter.GetY(y) + 0 ) * squareHeight),
                 TPoint( ( coordinatGetter.GetX(x) + 1 ) * squareWidth ,
                         ( coordinatGetter.GetY(y) + 1 ) * squareHeight)
            ), image->Picture->Graphic);

      }
    }
  }

  //Draw selected
  if (m_select_x != -1)
  {
    //Draw selection cursor
    ImageBuffer->Canvas->StretchDraw(
      TRect( TPoint( ( coordinatGetter.GetX(m_select_x) + 0 ) * squareWidth ,
                     ( coordinatGetter.GetY(m_select_y) + 0 ) * squareHeight),
             TPoint( ( coordinatGetter.GetX(m_select_x) + 1 ) * squareWidth ,
                     ( coordinatGetter.GetY(m_select_y) + 1 ) * squareHeight)
        ), ImageSelected->Picture->Graphic);

    //If a chesspiece is selected, draw the crosses on the areas which
    //are valid moves
    if (m_game.GetBoard().GetPiece(m_select_x, m_select_y).IsNull() == false)
    {
      const std::vector<ChessMove> possibleMoves
        = m_game.GetBoard().GetAllValidMoves(m_select_x,m_select_y);
      typedef std::vector<ChessMove>::const_iterator Iterator;
      const Iterator lastMove = possibleMoves.end();
      for (Iterator possibleMove = possibleMoves.begin();
        possibleMove != lastMove;
        ++possibleMove)
      {
        ImageBuffer->Canvas->StretchDraw(
          TRect(
            TPoint( ( coordinatGetter.GetX(possibleMove->x2) + 0 ) * squareWidth ,
                    ( coordinatGetter.GetY(possibleMove->y2) + 0 ) * squareHeight),
            TPoint( ( coordinatGetter.GetX(possibleMove->x2) + 1 ) * squareWidth ,
                    ( coordinatGetter.GetY(possibleMove->y2) + 1 ) * squareHeight)
            ), ImageCross->Picture->Graphic);
      }
    }
  }

  //Draw cursor
  ImageBuffer->Canvas->StretchDraw(
    TRect( TPoint( ( coordinatGetter.GetX(m_cursor_x) + 0 ) * squareWidth ,
                   ( coordinatGetter.GetY(m_cursor_y) + 0 ) * squareHeight),
           TPoint( ( coordinatGetter.GetX(m_cursor_x) + 1 ) * squareWidth ,
                   ( coordinatGetter.GetY(m_cursor_y) + 1 ) * squareHeight)
      ), ImageCursor->Picture->Graphic);

  //Draw board and pieces to screen
  this->Canvas->Draw(0,0,ImageBuffer->Picture->Graphic);
}

void game_dialog::process_command(const command c)
{
  switch (c)
  {
    case command::left:
      if (m_game.GetWhoseTurn()==white)
      {
        --m_cursor_x; if (m_cursor_x < 0) m_cursor_x = 0;
      }
      else
      {
        ++m_cursor_x; if (m_cursor_x > 7) m_cursor_x = 7;
      }
      break;
    case command::right:
      if (m_game.GetWhoseTurn()==white)
      {
        ++m_cursor_x; if (m_cursor_x > 7) m_cursor_x = 7;
      }
      else
      {
        --m_cursor_x; if (m_cursor_x < 0) m_cursor_x = 0;
      }
      break;
    case command::down:
      if (m_game.GetWhoseTurn()==white)
      {
        --m_cursor_y; if (m_cursor_y < 0) m_cursor_y = 0;
      }
      else
      {
        ++m_cursor_y; if (m_cursor_y > 7) m_cursor_y = 7;
      }
      break;
    case command::up:
      if (m_game.GetWhoseTurn()==white)
      {
        ++m_cursor_y; if (m_cursor_y > 7) m_cursor_y = 7;
      }
      else
      {
        --m_cursor_y; if (m_cursor_y < 0) m_cursor_y = 0;
      }
      break;
    case command::select:
      do_select(m_cursor_x, m_cursor_y);
      break;
  }
}

void game_dialog::process_commands()
{
  for (const auto c: m_commands) process_command(c);
  m_commands.clear();
}


void game_dialog::tick()
{
  process_commands();
}

/*

void game_dialog::DrawChessBoard()
{

}

const TImage * const game_dialog::GetImage(
  const bool inSight,
  const ChessPiece& piece) const
{
  //Visible?
  if (inSight== false) return ImageQuestionMark;
  //Piece located there?
  //No piece present? Then return null
  if (piece.IsNull()==true) { return 0; }
  //Piece present, draw a piece
  //Which color and type?
  const EnumChessPieceColor pieceColor = piece.GetColor();
  switch ( piece.GetType() )
  {
    case king  : return ( pieceColor == white ? ImageWhiteKing   : ImageBlackKing  );
    case queen : return ( pieceColor == white ? ImageWhiteQueen  : ImageBlackQueen );
    case rook  : return ( pieceColor == white ? ImageWhiteRook   : ImageBlackRook  );
    case bishop: return ( pieceColor == white ? ImageWhiteBishop : ImageBlackBishop);
    case knight: return ( pieceColor == white ? ImageWhiteKnight : ImageBlackKnight);
    case pawn  : return ( pieceColor == white ? ImageWhitePawn   : ImageBlackPawn  );
  }
  assert(!"Should not get here. Unknown piece type");
  throw std::logic_error("Unknown piece type");

}




const bool game_dialog::IsCurrentPlayerHuman() const
{
  return ( (m_game.GetWhoseTurn() == white && mIsWhiteHuman == true)
    || (m_game.GetWhoseTurn() == black && mIsBlackHuman == true) );
}

void __fastcall game_dialog::Timer1Timer(TObject *Sender)
{
  if (IsCurrentPlayerHuman()==false)
  {
    const ChessMove move = m_game.SuggestMove();
    this->do_move(move);
  }
  else
  {
    DrawChessBoard();
  }
}


void __fastcall game_dialog::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (m_game.GetWhoseTurn()==white)
  {
    m_cursor_x = 0 + (X / (ClientWidth  / 8));
    m_cursor_y = 7 - (Y / (ClientHeight / 8));
  }
  else
  {
    m_cursor_x = 7 - (X / (ClientWidth  / 8));
    m_cursor_y = 0 + (Y / (ClientHeight / 8));
  }

  if (m_cursor_x < 0) m_cursor_x = 0;
  if (m_cursor_y < 0) m_cursor_y = 0;
  if (m_cursor_x > 7) m_cursor_x = 7;
  if (m_cursor_y > 7) m_cursor_y = 7;

  do_select(m_cursor_x, m_cursor_y);
}

*/
