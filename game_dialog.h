#ifndef SF_CHESSGAMEDIALOG_H
#define SF_CHESSGAMEDIALOG_H

#include <set>
#include "command.h"
#include "game.h"
#include "game_state.h"
#include "textures.h"
#include <SFML/Graphics/Sprite.hpp>

namespace sf { struct RenderWindow; }
namespace sf { struct Texture; }

class game_dialog
{
public:
  game_dialog(const int window_width, const int window_height);
  void add_command(const command c);
  void draw(sf::RenderWindow& window);
  void tick();

private:
  std::set<command> m_commands;
  int m_cursor_x;
  int m_cursor_y;
  ChessGame m_game;
  game_state m_game_state;
  int m_select_x;
  int m_select_y;
  textures m_textures;
  int m_window_height;
  int m_window_width;


  void do_move();
  void do_move(const ChessMove& move);
  void do_select(const int cursorX, const int cursorY);
  void process_commands();
  void process_command(const command c);

  /*
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);

private:	// User declarations
  void DrawChessBoard();
  const bool IsCurrentPlayerHuman() const;
  const TImage * const GetImage(
    const bool inSight,
    const ChessPiece& piece) const;
  */
};
#endif
