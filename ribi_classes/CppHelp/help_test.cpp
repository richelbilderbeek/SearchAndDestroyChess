#include "help.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_help_basic)
{
  const std::string program_name{"program name"};
  const std::string program_description{"program description"};
  const std::vector<ribi::Help::Option> options{
    ribi::Help::Option('b',"abs","take absolute"),
    ribi::Help::Option('r',"rel","take relative")
  };
  const std::vector<std::string> example_uses{
   "./my_program -a",
   "./my_program --rel"
  };

  const ribi::Help help(
    program_name,
    program_description,
    options,
    example_uses
  );
  BOOST_CHECK_EQUAL(help.GetProgramName(), program_name);
  BOOST_CHECK_EQUAL(help.GetProgramDescription(), program_description);
  BOOST_CHECK(help.GetOptions() == ribi::Help::AddDefaultOptions(options));
  BOOST_CHECK(help.GetExampleUses() == example_uses);
  std::stringstream s;
  s << help;
  BOOST_CHECK(!s.str().empty());
  BOOST_CHECK(!help.GetVersion().empty());
  BOOST_CHECK(!help.GetVersionHistory().empty());
}

BOOST_AUTO_TEST_CASE(ribi_help_too_long_option)
{
  BOOST_CHECK_THROW(
  ribi::Help::Option(
      'x',
      "long",
      "too long of a description that does not fit the 80 characters per line rule anymore, especially when I continue here"
    ), std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_help_short_option_duplicate)
{
  const std::string program_name{"program name"};
  const std::string program_description{"program description"};
  const std::vector<ribi::Help::Option> options{
    ribi::Help::Option('b',"abs","take absolute"),
    ribi::Help::Option('b',"rel","take relative")
  };
  const std::vector<std::string> example_uses{};
  BOOST_CHECK_THROW(
    ribi::Help help(
      program_name,
      program_description,
      options,
      example_uses
    ), std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_help_long_option_duplicate)
{
  const std::string program_name{"program name"};
  const std::string program_description{"program description"};
  const std::vector<ribi::Help::Option> options{
    ribi::Help::Option('b',"abs","take absolute"),
    ribi::Help::Option('r',"abs","take relative")
  };
  const std::vector<std::string> example_uses{};
  BOOST_CHECK_THROW(
    ribi::Help help(
      program_name,
      program_description,
      options,
      example_uses
    ), std::invalid_argument
  );
}


BOOST_AUTO_TEST_CASE(ribi_help_short_option_duplicate_with_default)
{
  const std::string program_name{"program name"};
  const std::string program_description{"program description"};
  const std::vector<ribi::Help::Option> options{
    ribi::Help::Option('l',"abs","take absolute"), //Licence
    ribi::Help::Option('r',"rel","take relative")
  };
  const std::vector<std::string> example_uses{};
  BOOST_CHECK_THROW(
    ribi::Help help(
      program_name,
      program_description,
      options,
      example_uses
    ), std::invalid_argument
  );
}

BOOST_AUTO_TEST_CASE(ribi_help_long_option_duplicate_with_default)
{
  const std::string program_name{"program name"};
  const std::string program_description{"program description"};
  const std::vector<ribi::Help::Option> options{
    ribi::Help::Option('b',"licence","take absolute"),
    ribi::Help::Option('r',"abs","take relative")
  };
  const std::vector<std::string> example_uses{};
  BOOST_CHECK_THROW(
    ribi::Help(
      program_name,
      program_description,
      options,
      example_uses
    ), std::invalid_argument
  );
}
