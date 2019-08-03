// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push


#define BOOST_TEST_DYN_LINK // Defines a main function

#define BOOST_TEST_MODULE test_reversi_module
#include <boost/test/unit_test.hpp>

#pragma GCC diagnostic pop
