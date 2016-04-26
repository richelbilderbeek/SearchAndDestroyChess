#ifndef TEXTURES_H
#define TEXTURES_H

#include <map>
#include <SFML/Graphics/Texture.hpp>
#include "piece_color.h"
#include "piece_type.h"

///Contains all textures
class textures
{
public:
  textures();

  ///Board
  const sf::Texture& get_square(
    const piece_color c
  ) const;

  const sf::Texture& get(
    const piece_type t,
    const piece_color c
  ) const;

  const sf::Texture& get_question_mark() const { return m_question_mark; }
  const sf::Texture& get_transparent() const { return m_transparent; }

private:
  sf::Texture m_question_mark;
  sf::Texture m_square_dark;
  sf::Texture m_square_light;
  sf::Texture m_transparent;

  sf::Texture m_bb;
  sf::Texture m_kb;
  sf::Texture m_nb;
  sf::Texture m_pb;
  sf::Texture m_qb;
  sf::Texture m_rb;

  sf::Texture m_bw;
  sf::Texture m_kw;
  sf::Texture m_nw;
  sf::Texture m_pw;
  sf::Texture m_qw;
  sf::Texture m_rw;

};

#endif // TEXTURES_H
