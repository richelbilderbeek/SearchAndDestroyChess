#include "chess_board.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(board_has_sixteen_pieces)
{
  chess_board b;
  BOOST_CHECK_EQUAL(b.GetPieces().size(), 16);
}
