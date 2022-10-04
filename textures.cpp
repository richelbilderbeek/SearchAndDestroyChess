#include "textures.h"
#include <cassert>
#include <stdexcept>

textures::textures()
  : m_question_mark{},
    m_square_dark{},
    m_square_light{},
    m_transparent{},
    m_bb{},
    m_kb{},
    m_nb{},
    m_pb{},
    m_qb{},
    m_rb{},
    m_bw{},
    m_kw{},
    m_nw{},
    m_pw{},
    m_qw{},
    m_rw{}
{
  { const bool s = m_question_mark.loadFromFile("../SearchAndDestroyChess/Sprites/qm.png"); assert(s); }
  { const bool s = m_square_dark.loadFromFile("../SearchAndDestroyChess/Sprites/d.png"); assert(s); }
  { const bool s = m_square_light.loadFromFile("../SearchAndDestroyChess/Sprites/l.png"); assert(s); }
  { const bool s = m_transparent.loadFromFile("../SearchAndDestroyChess/Sprites/t.png"); assert(s); }

  { const bool s = m_bb.loadFromFile("../SearchAndDestroyChess/Sprites/bb.png"); assert(s); }
  { const bool s = m_kb.loadFromFile("../SearchAndDestroyChess/Sprites/kb.png"); assert(s); }
  { const bool s = m_nb.loadFromFile("../SearchAndDestroyChess/Sprites/nb.png"); assert(s); }
  { const bool s = m_pb.loadFromFile("../SearchAndDestroyChess/Sprites/pb.png"); assert(s); }
  { const bool s = m_qb.loadFromFile("../SearchAndDestroyChess/Sprites/qb.png"); assert(s); }
  { const bool s = m_rb.loadFromFile("../SearchAndDestroyChess/Sprites/rb.png"); assert(s); }

  { const bool s = m_bw.loadFromFile("../SearchAndDestroyChess/Sprites/bw.png"); assert(s); }
  { const bool s = m_kw.loadFromFile("../SearchAndDestroyChess/Sprites/kw.png"); assert(s); }
  { const bool s = m_nw.loadFromFile("../SearchAndDestroyChess/Sprites/nw.png"); assert(s); }
  { const bool s = m_pw.loadFromFile("../SearchAndDestroyChess/Sprites/pw.png"); assert(s); }
  { const bool s = m_qw.loadFromFile("../SearchAndDestroyChess/Sprites/qw.png"); assert(s); }
  { const bool s = m_rw.loadFromFile("../SearchAndDestroyChess/Sprites/rw.png"); assert(s); }
}

const sf::Texture& textures::get_square(
  const piece_color c
) const
{
  switch (c)
  {
    case piece_color::white: return m_square_light;
    case piece_color::black: return m_square_dark;
  }
  throw std::logic_error("textures::get_square");
}

const sf::Texture& textures::get(
  const piece_type t,
  const piece_color c
) const
{
  if (c == piece_color::white)
  {
    switch(t)
    {
      case bishop: return m_bw;
      case king: return m_kw;
      case knight: return m_nw;
      case pawn: return m_pw;
      case queen: return m_qw;
      case rook: return m_rw;
    }
  }
  else
  {
    assert(c == piece_color::black);
    switch(t)
    {
      case bishop: return m_bb;
      case king: return m_kb;
      case knight: return m_nb;
      case pawn: return m_pb;
      case queen: return m_qb;
      case rook: return m_rb;
    }
  }
  throw std::logic_error("textures::get");
}
