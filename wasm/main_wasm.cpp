// WebAssembly version of SearchAndDestroyChess
//
// SFML cannot be compiled to WebAssembly, so this file is
// a front-end that re-uses the (SFML-free) game logic,
// mimics the behavior of game_dialog and draws on an HTML5 canvas.
// It has no dependencies other than Emscripten itself.
//
// Build with Emscripten, see wasm/build.sh

#include <cstdlib>
#include <string>
#include <vector>

#include <emscripten.h>
#include <emscripten/html5.h>

#include "../chess_move.h"
#include "../chess_piece.h"
#include "../command.h"
#include "../coordinat_getter.h"
#include "../game.h"
#include "../game_state.h"

extern "C" void redraw();

namespace {

constexpr int window_size{640};
constexpr int block_size{window_size / 8};

// Show a message in the HTML page
EM_JS(void, set_status, (const char* s), {
  const e = document.getElementById("status");
  if (e) e.textContent = UTF8ToString(s);
});

// Load the sprites, the files are in the 'Sprites' folder next to index.html
EM_JS(void, js_init, (int size), {
  const canvas = document.getElementById("canvas");
  canvas.width = size;
  canvas.height = size;
  Module.ctx = canvas.getContext("2d");
  Module.sprites = {};
  for (const name of ["qm", "d", "l",
    "bb", "kb", "nb", "pb", "qb", "rb",
    "bw", "kw", "nw", "pw", "qw", "rw"])
  {
    const img = new Image();
    img.onload = function() { Module._redraw(); };
    img.src = "Sprites/" + name + ".png";
    Module.sprites[name] = img;
  }
});

EM_JS(void, js_draw_sprite, (const char* name, int x, int y, int size), {
  const img = Module.sprites[UTF8ToString(name)];
  if (img && img.complete && img.naturalWidth > 0)
  {
    Module.ctx.drawImage(img, x, y, size, size);
  }
});

EM_JS(void, js_fill_rect, (int x, int y, int w, int h, const char* color), {
  Module.ctx.fillStyle = UTF8ToString(color);
  Module.ctx.fillRect(x, y, w, h);
});

EM_JS(void, js_stroke_rect, (int x, int y, int w, int h, int thickness, const char* color), {
  Module.ctx.strokeStyle = UTF8ToString(color);
  Module.ctx.lineWidth = thickness;
  // Draw the outline inside of the square
  Module.ctx.strokeRect(
    x + thickness / 2, y + thickness / 2, w - thickness, h - thickness
  );
});

struct web_game
{
  chess_game game;
  game_state state{game_state::start_turn};
  int cursor_x{3};
  int cursor_y{3};
  int select_x{-1};
  int select_y{-1};
  std::string last_status;
};

web_game* g{nullptr};

std::string get_piece_name(const chess_piece& piece)
{
  std::string s;
  switch (piece.GetType())
  {
    case bishop: s = "b"; break;
    case king: s = "k"; break;
    case knight: s = "n"; break;
    case pawn: s = "p"; break;
    case queen: s = "q"; break;
    case rook: s = "r"; break;
  }
  return s + (piece.GetColor() == piece_color::white ? "w" : "b");
}

void update_status()
{
  std::string s;
  switch (g->state)
  {
    case game_state::player_1_won: s = "Player 1 (white) won! Reload the page to play again."; break;
    case game_state::player_2_won: s = "Player 2 (black) won! Reload the page to play again."; break;
    case game_state::start_turn:
      s = std::string("Turn of player ")
        + (g->game.get_whose_turn() == piece_color::white ? "1 (white)" : "2 (black)")
        + ": press Enter or click the board to start your turn";
      break;
    case game_state::playing:
      s = std::string("Player ")
        + (g->game.get_whose_turn() == piece_color::white ? "1 (white)" : "2 (black)")
        + " is playing";
      break;
  }
  if (s != g->last_status)
  {
    set_status(s.c_str());
    g->last_status = s;
  }
}

void do_move()
{
  if (g->select_x == -1 || g->cursor_x == -1) return;
  const int x1{g->select_x};
  const int y1{g->select_y};
  const int x2{g->cursor_x};
  const int y2{g->cursor_y};
  const chess_piece piece = g->game.GetBoard().GetPiece(x1, y1);
  if (piece.IsNull()) return;
  const bool capture{!g->game.GetBoard().GetPiece(x2, y2).IsNull()};
  const chess_move move(piece.GetType(), x1, y1, capture, x2, y2);
  g->select_x = -1;
  g->select_y = -1;
  if (!g->game.CanDoMove(move)) return;
  g->game.DoMove(move);
  if (g->game.IsGameOver())
  {
    g->state = g->game.GetWinner() == piece_color::white
      ? game_state::player_1_won
      : game_state::player_2_won;
    return;
  }
  g->state = game_state::start_turn;
  g->cursor_x = std::rand() % 8;
  g->cursor_y = std::rand() % 8;
}

bool is_own_piece(const int x, const int y)
{
  const chess_piece p = g->game.GetBoard().GetPiece(x, y);
  return !p.IsNull() && p.GetColor() == g->game.get_whose_turn();
}

void do_select(const int x, const int y)
{
  if (g->select_x == -1 && is_own_piece(x, y))
  {
    g->select_x = x;
    g->select_y = y;
  }
  else if (g->select_x != -1 && is_own_piece(g->select_x, g->select_y) && is_own_piece(x, y))
  {
    g->select_x = x;
    g->select_y = y;
  }
  else
  {
    do_move();
  }
}

void click_mouse(const int x, const int y)
{
  if (g->state == game_state::start_turn)
  {
    g->state = game_state::playing;
    return;
  }
  if (g->state != game_state::playing) return;
  const bool white{g->game.get_whose_turn() == piece_color::white};
  int cx = white ? x / block_size : 7 - (x / block_size);
  int cy = white ? 7 - (y / block_size) : y / block_size;
  if (cx < 0) cx = 0;
  if (cy < 0) cy = 0;
  if (cx > 7) cx = 7;
  if (cy > 7) cy = 7;
  g->cursor_x = cx;
  g->cursor_y = cy;
  do_select(cx, cy);
}

void process_command(const command c)
{
  if (g->state == game_state::start_turn)
  {
    if (c == command::ok) g->state = game_state::playing;
    return;
  }
  if (g->state != game_state::playing) return;
  // Black sees the board rotated
  const int d{g->game.get_whose_turn() == piece_color::white ? 1 : -1};
  switch (c)
  {
    case command::left: g->cursor_x -= d; break;
    case command::right: g->cursor_x += d; break;
    case command::down: g->cursor_y -= d; break;
    case command::up: g->cursor_y += d; break;
    case command::select: do_select(g->cursor_x, g->cursor_y); break;
    case command::ok: break;
  }
  if (g->cursor_x < 0) g->cursor_x = 0;
  if (g->cursor_y < 0) g->cursor_y = 0;
  if (g->cursor_x > 7) g->cursor_x = 7;
  if (g->cursor_y > 7) g->cursor_y = 7;
}

void handle_key(const std::string& k)
{
  if (k == "w" || k == "W" || k == "ArrowUp") process_command(command::up);
  else if (k == "d" || k == "D" || k == "ArrowRight") process_command(command::right);
  else if (k == "s" || k == "S" || k == "ArrowDown") process_command(command::down);
  else if (k == "a" || k == "A" || k == "ArrowLeft") process_command(command::left);
  else if (k == "q" || k == "Q" || k == "e" || k == "E" || k == " ") process_command(command::select);
  else if (k == "Enter") process_command(command::ok);
}

void draw_sprite(const std::string& name, const int screen_x, const int screen_y)
{
  js_draw_sprite(name.c_str(), screen_x * block_size, screen_y * block_size, block_size);
}

void draw_outline(const int screen_x, const int screen_y, const int thickness, const char* color)
{
  js_stroke_rect(
    screen_x * block_size, screen_y * block_size,
    block_size, block_size, thickness, color
  );
}

void draw_game()
{
  const coordinat_getter cg(g->game.get_whose_turn());
  const chess_board& board = g->game.GetBoard();
  const std::vector<std::vector<bool>> in_sight = g->game.GetInSight();

  for (int y = 0; y != 8; ++y)
  {
    for (int x = 0; x != 8; ++x)
    {
      const int sx{cg.get_x(x)};
      const int sy{cg.get_y(y)};
      draw_sprite((x + y) % 2 == 0 ? "d" : "l", sx, sy);
      if (!in_sight[y][x])
      {
        draw_sprite("qm", sx, sy);
        continue;
      }
      const chess_piece piece = board.GetPiece(x, y);
      if (!piece.IsNull()) draw_sprite(get_piece_name(piece), sx, sy);
    }
  }

  if (g->select_x != -1)
  {
    draw_outline(cg.get_x(g->select_x), cg.get_y(g->select_y), 8, "rgb(0,0,255)");
    if (!board.GetPiece(g->select_x, g->select_y).IsNull())
    {
      for (const chess_move& m: board.GetAllValidMoves(g->select_x, g->select_y))
      {
        js_fill_rect(
          cg.get_x(m.x2) * block_size, cg.get_y(m.y2) * block_size,
          block_size, block_size, "rgba(0,0,128,0.5)"
        );
      }
    }
  }
  draw_outline(cg.get_x(g->cursor_x), cg.get_y(g->cursor_y), 4, "rgb(255,0,0)");
}

EM_BOOL on_key_down(int, const EmscriptenKeyboardEvent* e, void*)
{
  const std::string k{e->key};
  handle_key(k);
  redraw();
  // Prevent the arrow keys and space from scrolling the page
  return k.rfind("Arrow", 0) == 0 || k == " " || k == "Enter";
}

EM_BOOL on_mouse_down(int, const EmscriptenMouseEvent* e, void*)
{
  if (e->button != 0) return EM_FALSE;
  // The canvas may be scaled, convert from CSS pixels to canvas pixels
  double css_w{0.0};
  double css_h{0.0};
  emscripten_get_element_css_size("#canvas", &css_w, &css_h);
  if (css_w <= 0.0 || css_h <= 0.0) return EM_FALSE;
  const int x = static_cast<int>(e->targetX * window_size / css_w);
  const int y = static_cast<int>(e->targetY * window_size / css_h);
  click_mouse(x, y);
  redraw();
  return EM_TRUE;
}

} // anonymous namespace

extern "C" EMSCRIPTEN_KEEPALIVE void redraw()
{
  js_fill_rect(0, 0, window_size, window_size, "rgb(128,128,128)");
  if (g->state == game_state::playing) draw_game();
  update_status();
}

int main()
{
  g = new web_game;
  js_init(window_size);
  emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_DOCUMENT, nullptr, EM_TRUE, on_key_down);
  emscripten_set_mousedown_callback("#canvas", nullptr, EM_TRUE, on_mouse_down);
  redraw();
  // Keep the runtime alive, as all work is done in the event callbacks
  emscripten_exit_with_live_runtime();
}
