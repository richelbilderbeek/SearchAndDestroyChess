#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

#include "piece_color.h"

class game_options
{
public:
  game_options();

  /// Determine if a player has a fog of war
  bool has_fog_of_war(const piece_color& player) const noexcept;
  private:
  bool m_black_sees_fog = false;
  bool m_white_sees_fog = true;
};

#endif // GAME_OPTIONS_H
