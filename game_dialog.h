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
  void click_mouse(const int x, const int y);
  void draw(sf::RenderWindow& window);
  void tick();

private:
  std::set<command> m_commands;
  int m_cursor_x;
  int m_cursor_y;
  chess_game m_game;
  game_state m_game_state;
  int m_select_x;
  int m_select_y;
  textures m_textures;
  int m_window_height;
  int m_window_width;


  void do_move();
  void do_move(const chess_move& move);
  void do_select(const int cursorX, const int cursorY);
  const sf::Texture& get_texture(
    const bool inSight,
    const chess_piece& piece
  ) const;
  void draw_game(sf::RenderWindow& window) const;
  void draw_player_won(sf::RenderWindow& window, const int player) const;
  void draw_start_turn(sf::RenderWindow& window) const;

  void process_commands();
  void process_command(const command c);
};
#endif
