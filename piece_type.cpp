
#include <iostream>
#include <cassert>
#include <stdexcept>


#include "piece_type.h"

std::ostream& operator<<(std::ostream& os, const piece_type& type)
{
  switch (type)
  {
    case pawn  : os << "P"; return os;
    case knight: os << "N"; return os;
    case bishop: os << "B"; return os;
    case rook  : os << "R"; return os;
    case queen : os << "Q"; return os;
    case king  : os << "K"; return os;
  }
  assert(!"Should not get here");
  throw std::logic_error("Unknown color");
}


