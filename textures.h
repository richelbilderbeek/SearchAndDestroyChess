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
private:
  sf::Texture m_square_dark;
  sf::Texture m_square_light;

};

#endif // TEXTURES_H
