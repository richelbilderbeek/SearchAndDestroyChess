#include "about.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>





#include <boost/lexical_cast.hpp>
#include <boost/version.hpp>



ribi::About::About(
    const std::string& author,
    const std::string& file_title,
    const std::string& file_description,
    const std::string& programmed_on,
    const std::string& years,
    const std::string& url,
    const std::string& version,
    const std::vector<std::string>& version_history) noexcept
  : m_author{author},
    m_file_title{file_title},
    m_file_description{file_description},
    m_libraries{},
    m_programmed_on{programmed_on},
    m_years{years},
    m_url{url},
    m_version{version},
    m_version_history{version_history}
{
  AddLibrary(
    "About version: "
    + GetAboutVersion());
  AddLibrary(
    "Boost version: "
    + GetBoostVersion());
  AddLibrary(
    "STL version: "
    + GetStlVersion()
    + " (GNU ISO C++ library)");
}

void ribi::About::AddLibrary(const std::string& s) noexcept
{
  m_libraries.push_back(s);
  std::sort(
    std::begin(m_libraries),
    std::end(m_libraries)
  );
}

std::vector<std::string> ribi::About::CreateAboutText() const noexcept
{
  const std::vector<std::string> v
  {
    m_file_title + ", version " + m_version,
    "Copyright (C) " + m_years + " " + m_author,
    "Programmed on " + m_programmed_on,
    "by " + m_author,
    "",
    m_file_title + " can be downloaded from " + m_url,
    "Licenced under GPL 3.0"
  };
  return v;
}

std::vector<std::string> ribi::About::CreateLibrariesUsedText() const noexcept
{
  std::vector<std::string> v;
  v.push_back("Libraries and classes used: ");
  for(const std::string& s: m_libraries)
  {
    v.push_back(" * " + s);
  }
  return v;
}

std::vector<std::string> ribi::About::CreateLicenceText() const noexcept
{
  std::vector<std::string> v{
    m_file_title + ", " + m_file_description,
    "Copyright (C) " + m_years + " " + m_author,
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    ""
  };
  return v;
}

std::vector<std::string> ribi::About::CreateVersionHistory() const noexcept
{
  std::vector<std::string> v;
  v.push_back("Version history:");
  v.push_back(" * YYYY-MM-DD: version X.Y: [description]");
  for(const std::string& s: m_version_history)
  {
    v.push_back(" * " + s);
  }
  return v;
}

std::string ribi::About::GetAboutVersion() noexcept
{
  return "1.8";
}

std::vector<std::string> ribi::About::GetAboutVersionHistory() noexcept
{
  return {
    "2011-01-07: version 1.0: initial version",
    "2011-01-11: version 1.1: added simple getters",
    "2011-09-12: version 1.2: added operator<<",
    "2012-01-08: version 1.3: fixed grammatical error",
    "2012-01-26: version 1.4: removed BOOST_FOREACH",
    "2013-04-29: version 1.5: added #ifdefs for GCC 4.4.0",
    "2013-09-05: version 1.6: transition to namespace ribi",
    "2013-09-16: version 1.7: noexcept added",
    "2015-11-30: version 1.8: pass std::string by const-reference"
  };
}

std::string ribi::About::GetBoostVersion() noexcept
{
  std::string s = BOOST_LIB_VERSION;
  std::replace(
    std::begin(s),
    std::end(s),
    '_',
    '.'
  );
  return s;
}

std::string ribi::About::GetStlVersion() noexcept
{
  return boost::lexical_cast<std::string>(__VERSION__);
}

std::ostream& ribi::operator<<(std::ostream& os,const About& a) noexcept
{
  {
    const std::vector<std::string> v{a.CreateAboutText()};
    std::copy(
      std::begin(v),
      std::end(v),
      std::ostream_iterator<std::string>(os,"\n")
    );
  }
  os
    << '\n';
  {
    std::vector<std::string> v{a.CreateLibrariesUsedText()};
    std::copy(
      std::begin(v),
      std::end(v),
      std::ostream_iterator<std::string>(os,"\n")
    );
  }
  os
    << '\n';
  {
    std::vector<std::string> v{a.CreateVersionHistory()};
    std::copy(
      std::begin(v),
      std::end(v),
      std::ostream_iterator<std::string>(os,"\n")
    );
  }
  os
    << '\n'
    << "Licence:\n";
  {
    std::vector<std::string> v{a.CreateLicenceText()};
    std::copy(
      begin(v),
      end(v),
      std::ostream_iterator<std::string>(os,"\n")
    );
  }
  os
    << '\n'
    << "Source code built on "
    << __DATE__
    << " "
    << __TIME__
    << '\n'
  #ifdef NDEBUG
    << "Release version"
  #else
    << "Debug version"
  #endif
    << '\n';
  return os;
}
