#ifndef TEXTURES_H
#define TEXTURES_H

#include <map>
#include <SFML/Graphics/Texture.hpp>
#include "texture_type.h"
#include "UnitEnumChessPieceColor.h"
#include "UnitEnumChessPieceType.h"

///Contains all textures
class textures
{
public:
  textures();

  ///Board
  const sf::Texture& get_square(
    const EnumChessPieceColor c
  ) const;

  const sf::Texture& get(
    const EnumChessPieceType t,
    const EnumChessPieceColor c
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
