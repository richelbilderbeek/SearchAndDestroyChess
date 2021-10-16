#include "game_options.h"

#include <cassert>
game_options::game_options()
{

}

bool game_options::has_fog_of_war(const piece_color& player) const noexcept
{
  if (player == piece_color::black)
  {
    return m_black_sees_fog;
  }
  assert(player == piece_color::white);
  return m_white_sees_fog;
}
