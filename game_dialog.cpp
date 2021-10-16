#include "game_dialog.h"

#include <cstdlib>
#include <ctime>
#include <cassert>
#include <vector>
#include <stdexcept>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "chess_board.h"
#include "chess_piece.h"
#include "coordinat_getter.h"

game_dialog::game_dialog(
  const int window_width,
  const int window_height
)
  : m_commands{},
    m_cursor_x{3},
    m_cursor_y(3),
    m_game{},
    m_game_state{game_state::start_turn},
    m_select_x{-1},
    m_select_y{-1},
    m_textures{},
    m_window_height{window_height},
    m_window_width{window_width}
{

}

void game_dialog::add_command(const command c)
{
  m_commands.insert(c);
}

void game_dialog::click_mouse(const int x, const int y)
{
  switch (m_game_state)
  {
    case game_state::player_1_won: return;
    case game_state::player_2_won: return;
    case game_state::playing: break;
    case game_state::start_turn: return;
  }

  const int w = m_window_width;
  const int h = m_window_height;

  if (m_game.get_whose_turn() == piece_color::white)
  {
    m_cursor_x = 0 + (x / (w  / 8));
    m_cursor_y = 7 - (y / (h / 8));
  }
  else
  {
    m_cursor_x = 7 - (x / (w  / 8));
    m_cursor_y = 0 + (y / (h / 8));
  }

  if (m_cursor_x < 0) m_cursor_x = 0;
  if (m_cursor_y < 0) m_cursor_y = 0;
  if (m_cursor_x > 7) m_cursor_x = 7;
  if (m_cursor_y > 7) m_cursor_y = 7;

  do_select(m_cursor_x, m_cursor_y);
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
  const chess_piece piece = m_game.GetBoard().GetPiece(x1,y1);
  assert(piece.IsNull()==false
    && "User must not be able to select an empty square do do a move from");
  const piece_type type = piece.GetType();
  const chess_piece victim = m_game.GetBoard().GetPiece(x2,y2);
  const bool capture = (victim.IsNull() ? false : true);
  const chess_move move(type,x1,y1,capture,x2,y2);
  this->do_move(move);
  m_select_x = -1;
  m_select_y = -1;
}

void game_dialog::do_move(const chess_move& move)
{
  if (!m_game.CanDoMove(move))
  {
    return;
  }

  //A -possibly deciding- move takes place
  m_game.DoMove(move);

  //Has someone won?
  if (m_game.IsGameOver())
  {
    m_game_state = m_game.GetWinner() == piece_color::white ? game_state::player_1_won : game_state::player_2_won;
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
    && m_game.GetBoard().GetPiece(cursorX,cursorY).IsNull()==false
    && m_game.GetBoard().GetPiece(cursorX,cursorY).GetColor() == m_game.get_whose_turn()
  )
  {
    //Nothing selected until now
    m_select_x = cursorX;
    m_select_y = cursorY;
  }
  else if (
       m_select_x != -1
    && m_select_y != -1
    && m_game.GetBoard().GetPiece(m_select_x,m_select_y).IsNull() == false
    && m_game.GetBoard().GetPiece(m_select_x,m_select_y).GetColor() == m_game.get_whose_turn()
    && m_game.GetBoard().GetPiece(cursorX,cursorY).IsNull() == false
    && m_game.GetBoard().GetPiece(cursorX,cursorY).GetColor() == m_game.get_whose_turn() )
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
  switch (m_game_state)
  {
    case game_state::player_1_won: draw_player_won(window, 1); break;
    case game_state::player_2_won: draw_player_won(window, 2); break;
    case game_state::playing: draw_game(window); break;
    case game_state::start_turn: draw_start_turn(window); break;
  }
}

void game_dialog::draw_game(sf::RenderWindow& window) const
{
  window.setTitle("SearchAndDestroyChess");
  const std::vector<std::vector<bool>> inSight = m_game.GetInSight();

  const double block_width  = static_cast<double>(m_window_width) / 8.0;
  const double block_height = static_cast<double>(m_window_height) / 8.0;
  const double scale_x = block_width / 129.0; //The images are 129x129
  const double scale_y = block_height / 129.0; //The images are 129x129
  //The CoordinatGetter transforms the coordinats of the board,
  //according to the player whose turn it is
  const coordinat_getter coordinatGetter ( m_game.get_whose_turn());

  //Draw squares, question marks and pieces
  {
    const chess_board board = m_game.GetBoard();

    //Draw all squares on buffer (clearing it)
    for (int y=0; y!=8; ++y)
    {
      for (int x=0; x!=8; ++x)
      {
        const sf::Texture& t = m_textures.get_square((x + y) % 2 == 0
          ? piece_color::black
          : piece_color::white
        );
        sf::Sprite sprite;
        sprite.setTexture(t);
        sprite.setPosition(
          ( coordinatGetter.get_x(x) + 0 ) * block_width,
          ( coordinatGetter.get_y(y) + 0 ) * block_width
        );
        sprite.setScale( sf::Vector2f(scale_x, scale_y) );
        window.draw(sprite);
      }
    }

    //Draw all question marks and pieces
    for (int y=0; y!=8; ++y)
    {
      for (int x=0; x!=8; ++x)
      {
        //If there is nothing, t will be transparent :-)
        bool is_in_sight = inSight[y][x];
        if (!m_options.has_fog_of_war(get_whose_turn())) is_in_sight = true;
        const sf::Texture& t = get_texture(is_in_sight, board.GetPiece(x,y));
        sf::Sprite sprite;
        sprite.setTexture(t);
        sprite.setPosition(
          ( coordinatGetter.get_x(x) + 0 ) * block_width,
          ( coordinatGetter.get_y(y) + 0 ) * block_width
        );
        sprite.setScale( sf::Vector2f(scale_x, scale_y) );
        window.draw(sprite);
      }
    }
  }

  //Draw selected
  if (m_select_x != -1)
  {
    {
      //Draw selection cursor
      sf::RectangleShape s(sf::Vector2f(block_width, block_height));
      s.setPosition(
        sf::Vector2f(
          (coordinatGetter.get_x(m_select_x) + 0) * block_width ,
          (coordinatGetter.get_y(m_select_y) + 0) * block_height
        )
      );
      s.setOutlineThickness(8);
      s.setOutlineColor(sf::Color::Blue);
      s.setFillColor(sf::Color::Transparent);
      window.draw(s);
    }

    //If a chesspiece is selected, draw the crosses on the areas which
    //are valid moves
    if (m_game.GetBoard().GetPiece(m_select_x, m_select_y).IsNull() == false)
    {
      const std::vector<chess_move> possibleMoves
        = m_game.GetBoard().GetAllValidMoves(m_select_x,m_select_y);
      typedef std::vector<chess_move>::const_iterator Iterator;
      const Iterator lastMove = possibleMoves.end();
      for (Iterator possibleMove = possibleMoves.begin();
        possibleMove != lastMove;
        ++possibleMove)
      {
        sf::RectangleShape s(sf::Vector2f(block_width, block_height));
        s.setFillColor(sf::Color(0,0,128,128));
        s.setPosition(
          sf::Vector2f(
            (coordinatGetter.get_x(possibleMove->x2) + 0) * block_width,
            (coordinatGetter.get_y(possibleMove->y2) + 0) * block_height
          )
        );
        window.draw(s);
      }
    }
  }

  //Draw cursor
  sf::RectangleShape s(sf::Vector2f(block_width, block_height));
  s.setPosition(
    sf::Vector2f(
      (coordinatGetter.get_x(m_cursor_x) + 0) * block_width,
      (coordinatGetter.get_y(m_cursor_y) + 0) * block_height
    )
  );
  s.setOutlineThickness(4);
  s.setOutlineColor(sf::Color::Red);
  s.setFillColor(sf::Color::Transparent);
  window.draw(s);
}

void game_dialog::draw_player_won(sf::RenderWindow& window, const int player) const
{
  assert(player == 1 || player == 2);
  switch (player)
  {
    case 1: window.setTitle("Player 1 won"); break;
    case 2: window.setTitle("Player 2 won"); break;
    default: assert(!"Should not get here");
  }
  sf::RectangleShape r(sf::Vector2f(m_window_width, m_window_height));
  r.setPosition(0,0);
  r.setFillColor(sf::Color(128,128,128));
  window.draw(r);
}

void game_dialog::draw_start_turn(sf::RenderWindow& window) const
{
  window.setTitle("Press enter to start the next turn");
  sf::RectangleShape r(sf::Vector2f(m_window_width, m_window_height));
  r.setPosition(0,0);
  r.setFillColor(sf::Color(128,128,128));
  window.draw(r);
}


const sf::Texture& game_dialog::get_texture(
  const bool inSight,
  const chess_piece& piece) const
{
  //Visible?
  if (!inSight) return m_textures.get_question_mark();
  //Piece located there?
  //No piece present? Then return null
  if (piece.IsNull()) { return m_textures.get_transparent(); }
  //Piece present, draw a piece
  return m_textures.get(piece.GetType(), piece.GetColor());

}


void game_dialog::process_command(const command c)
{
  switch (c)
  {
    case command::left:
      if (m_game.get_whose_turn()==piece_color::white)
      {
        --m_cursor_x; if (m_cursor_x < 0) m_cursor_x = 0;
      }
      else
      {
        ++m_cursor_x; if (m_cursor_x > 7) m_cursor_x = 7;
      }
      break;
    case command::right:
      if (m_game.get_whose_turn()==piece_color::white)
      {
        ++m_cursor_x; if (m_cursor_x > 7) m_cursor_x = 7;
      }
      else
      {
        --m_cursor_x; if (m_cursor_x < 0) m_cursor_x = 0;
      }
      break;
    case command::down:
      if (m_game.get_whose_turn()==piece_color::white)
      {
        --m_cursor_y; if (m_cursor_y < 0) m_cursor_y = 0;
      }
      else
      {
        ++m_cursor_y; if (m_cursor_y > 7) m_cursor_y = 7;
      }
      break;
    case command::up:
      if (m_game.get_whose_turn()==piece_color::white)
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
    case command::ok:
      //This happens by accident, when someone presses enter
      //for a new screen
      break;
  }
}

void game_dialog::process_commands()
{
  switch (m_game_state)
  {
    case game_state::player_1_won: return;
    case game_state::player_2_won: return;
    case game_state::playing: break;
    case game_state::start_turn:
    {
      if (!m_commands.empty() && m_commands.count(command::ok))
      {
        m_game_state = game_state::playing;
        m_commands.clear();
      }
    }
    return;
  }

  for (const auto c: m_commands) process_command(c);
  m_commands.clear();
}


void game_dialog::tick()
{
  process_commands();
}

