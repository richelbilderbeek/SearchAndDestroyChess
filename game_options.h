#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H


class game_options
{
public:
  game_options();
  bool m_black_sees_fog = false;
  bool m_white_sees_fog = true;
};

#endif // GAME_OPTIONS_H
