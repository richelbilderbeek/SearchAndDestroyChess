//---------------------------------------------------------------------------
/*
About, class for containing information about a program
Copyright (C) 2010-2016 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppAbout.htm
//---------------------------------------------------------------------------
#include "about.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ribi_about_test)
{
  const std::string author{"author"};
  const std::string file_title{"file title"};
  const std::string file_description{"file description"};
  const std::string programmed_on{"programmed on"};
  const std::string years{"years"};
  const std::string url{"url"};
  const std::string version{"version"};
  const std::vector<std::string>& version_history{
    "2016-03-19: version 1.0: initial version",
    "2016-03-19: version 1.1: initial version too"
  };

  const ribi::About about(
    author,
    file_title,
    file_description,
    programmed_on,
    years,
    url,
    version,
    version_history
  );

  BOOST_CHECK(about.GetAuthor() == author);
  BOOST_CHECK(about.GetFileTitle() == file_title);
  BOOST_CHECK(about.GetFileDescription() == file_description);
  BOOST_CHECK(about.GetProgrammedAt() == programmed_on);
  BOOST_CHECK(about.GetYears() == years);
  BOOST_CHECK(about.GetUrl() == url);
  BOOST_CHECK(about.GetVersion() == version);
  BOOST_CHECK(about.GetVersionHistory() == version_history);
  BOOST_CHECK(!about.CreateAboutText().empty());
  BOOST_CHECK(!about.CreateLibrariesUsedText().empty());
  BOOST_CHECK(!about.CreateLicenceText().empty());
  BOOST_CHECK(!about.CreateVersionHistory().empty());
  BOOST_CHECK(!about.GetAboutVersionHistory().empty());
  BOOST_CHECK(about.GetStlVersion() == __VERSION__);
  std::stringstream s;
  s << about;
  BOOST_CHECK(!s.str().empty());
}
