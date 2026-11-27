#include "help.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>

ribi::Help::Option::Option(
  const char option_short,
  const std::string& option_long,
  const std::string& option_description)
  : m_short(option_short),
    m_long(option_long),
    m_description(option_description)
{
  const int max_chars_per_line = 80;
  const int chars_for_padding = 7;
  const int max_chars = max_chars_per_line - chars_for_padding;
  const int chars_used = static_cast<int>(1 + m_long.size() + m_description.size());
  if (chars_used > max_chars)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "options must be kept short to fit on a line, "
      << "please shorten the description and/or long option. "
      << "max_chars: " << max_chars << ", "
      << "chars_used: " << chars_used << ", "
      << "option_short: " << option_short << ", "
      << "option_long: " << option_long << ", "
      << "option_description: " << option_description
    ;
    throw std::invalid_argument(msg.str());
  }
}


ribi::Help::Help(
  const std::string& program_name,
  const std::string& program_description,
  const std::vector<Option>& options,
  const std::vector<std::string> example_uses)
  : m_example_uses(example_uses),
    m_options(AddDefaultOptions(options)),
    m_program_description(program_description),
    m_program_name(program_name
)
{
  //checks if there are no short or long option occurring twice
  const std::size_t sz = m_options.size();
  for (std::size_t i=0; i!=sz-1; ++i)
  {
    const Option& a { m_options[i] };
    for (std::size_t j=i+1; j!=sz; ++j)
    {
      assert(j < m_options.size());
      const Option& b { m_options[j] };
      if (a.m_short == b.m_short)
      {
        std::stringstream msg;
        msg
          << __func__ << ": "
          << "Every short option must be unique, "
          << "supplied short option was '" << a.m_short
          << "'"
        ;
        throw std::invalid_argument(msg.str());
      }
      if (a.m_long == b.m_long)
      {
        std::stringstream msg;
        msg
          << __func__ << ": "
          << "Every long option must be unique, "
          << "supplied long option was '" << a.m_long
          << "'"
        ;
        throw std::invalid_argument(msg.str());
      }
    }
  }
}

const std::vector<ribi::Help::Option> ribi::Help::AddDefaultOptions(const std::vector<Option>& options)
{
  //v: unpadded Options
  std::vector<Option> v { options };
  v.push_back(Option('a',"about","display about message"));
  v.push_back(Option('h',"help","display this help message"));
  v.push_back(Option('i',"history","display version history"));
  v.push_back(Option('l',"licence","display licence"));
  v.push_back(Option('v',"version","display version"));

  //Find the longest long option, for padding
  const int max_length {
    static_cast<int>(
      std::max_element(
        std::begin(v),
        std::end(v),
        [](const Option& lhs, const Option& rhs)
        {
          return lhs.m_long.size() < rhs.m_long.size();
        }
      )->m_long.size()
    )
  };
  //w: padded options
  std::vector<Option> w;
  for (Option& p: v)
  {
    const int sz = static_cast<int>(p.m_long.size());
    assert(max_length >= sz);
    const int n_spaces = max_length - sz;
    assert(n_spaces >= 0);
    const std::string s = p.m_long
      + (n_spaces > 0 ? std::string(n_spaces,' '): std::string());
    assert(max_length == static_cast<int>(s.size()));
    const Option q(p.m_short,s,p.m_description);
    w.push_back(q);
  }

  //Sorts by short option
  //Also checks if there are no short or long option occurring twice
  std::sort(
    std::begin(w),
    std::end(w),
    [](const Option& lhs, const Option& rhs)
      {
        if (lhs.m_short == rhs.m_short)
        {
          std::stringstream msg;
          msg
            << __func__ << ": "
            << "Every short option must be unique, "
            << "supplied short option was '" << lhs.m_short
            << "'"
          ;
          throw std::invalid_argument(msg.str());
        }
        if (lhs.m_long == rhs.m_long)
        {
          std::stringstream msg;
          msg
            << __func__ << ": "
            << "Every long option must be unique, "
            << "supplied long option was '" << lhs.m_long
            << "'"
          ;
          throw std::invalid_argument(msg.str());
        }
        assert(lhs.m_short != rhs.m_short
          && "Every short option must be unique");
        assert(lhs.m_long != rhs.m_long
          && "Every long option must be unique");
        return lhs.m_short < rhs.m_short;
      }
    );

  return w;
}

std::string ribi::Help::GetVersion() noexcept
{
  return "1.2";
}

std::vector<std::string> ribi::Help::GetVersionHistory() noexcept
{
  return {
    "201x-xx-xx: Version 1.0: initial version",
    "2014-02-27: Version 1.1: started versioning",
    "2016-03-19: Version 1.2: use of Boost.Test",
  };
}

std::ostream& ribi::operator<<(std::ostream& os, const Help& help)
{
  os
    << help.GetProgramName() << " help menu\n"
    << "\n"
    << help.GetProgramDescription() << "\n"
    << "\n"
    << "Allowed options for " << help.GetProgramName() << ":\n";
  for (const Help::Option& p: help.GetOptions())
  {
    os << "-" << p.m_short << ", --" << p.m_long << "  " << p.m_description << '\n';
  }
  os
    << "\n"
    << "Example uses:\n";
  for (const std::string& s: help.GetExampleUses())
  {
    os << "  " << s << '\n';
  }
  return os;
}

bool ribi::operator==(const Help::Option& lhs, const Help::Option& rhs) noexcept
{
  return lhs.m_short == rhs.m_short
    && lhs.m_long == rhs.m_long
    && lhs.m_description == rhs.m_description
  ;
}
