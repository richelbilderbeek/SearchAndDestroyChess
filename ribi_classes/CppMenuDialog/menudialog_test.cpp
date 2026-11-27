#include "menudialog.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_convert_arguments_test)
{
  char ** argv = new char*[2];
  argv[0] = new char[16];
  argv[1] = new char[16];
  const std::string s1 = "012345678901234";
  const std::string s2 = "ABCDEFGHIJKLMNO";
  std::strcpy(argv[0],s1.c_str());
  std::strcpy(argv[1],s2.c_str());
  const auto v = ribi::MenuDialog::ConvertArguments(2, argv);
  BOOST_CHECK(v.size() == 2);
  BOOST_CHECK(v[0] == s1);
  BOOST_CHECK(v[1] == s2);
  delete[] argv[0];
  delete[] argv[1];
  delete[] argv;
}

struct test_ribi_menu_dialog_custom_dialog : public ribi::MenuDialog
{
  ribi::About GetAbout() const noexcept {
    return ribi::About(
      "author",
      "name",
      "description",
      "today",
      "this year",
      "https://www.richelbilderbeek.nl",
      GetVersion(),
      GetVersionHistory()
    );
  }

  ///Obtain the help information of this class
  ribi::Help GetHelp() const noexcept {
    return ribi::Help(
      "name",
      "description",
      {},
      {}
    );
  }

  ///Obtain the version of this class
  std::string GetVersion() const noexcept { return "1.0"; }

  ///Obtain the version history of this class
  std::vector<std::string> GetVersionHistory() const noexcept { return {"today: first version" }; }

  protected:
  ///When Execute has done the default parse, the arguments are passed to ExecuteSpecific.
  ///Here the program itself is called.
  ///Returns the error code to give back to the operatings system
  ///Make it a non-const member function: a menu is allowed to have state
  int ExecuteSpecific(const std::vector<std::string>&) noexcept { return 0; }

};

BOOST_AUTO_TEST_CASE(test_ribi_menu_dialog_execute_custom_menu)
{
  test_ribi_menu_dialog_custom_dialog d;
  BOOST_CHECK_EQUAL(d.Execute( {} ), 0);
  BOOST_CHECK_EQUAL(d.Execute( { "test" } ), 0);
  BOOST_CHECK_EQUAL(d.Execute( { "test", "--about" } ), 0);
  BOOST_CHECK_EQUAL(d.Execute( { "test", "--help" } ), 0);
  BOOST_CHECK_EQUAL(d.Execute( { "test", "--history" } ), 0);
  BOOST_CHECK_EQUAL(d.Execute( { "test", "--license" } ), 0);
  BOOST_CHECK_EQUAL(d.Execute( { "test", "--version" } ), 0);
}
