#include "chess_board.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(board_is_eight_times_eight_squares)
{
  const chess_board b;
  BOOST_CHECK_EQUAL(b.GetPieces().size(), 8);
  BOOST_CHECK_EQUAL(b.GetPieces()[0].size(), 8);
}
