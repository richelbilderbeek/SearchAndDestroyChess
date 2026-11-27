#include "canvas.h"

#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include <iterator>

#include <boost/math/constants/constants.hpp>
#include <boost/algorithm/string/split.hpp>

#include <QString>

//Older Qt version:
//#include <QRegExp>
#include <QRegularExpression>


#include "canvascolorsystems.h"
#include "canvascoordinatsystems.h"
//#include "fileio.h"

ribi::Canvas::Canvas()
{

}

std::vector<char> ribi::Canvas::GetAsciiArtGradient() noexcept
{
  return { 'M','N','m','d','h','y','s','o','+','/',':','-','.','`',' ' };
}

std::string ribi::Canvas::GetVersion() noexcept
{
  return "1.0";
}

std::vector<std::string> ribi::Canvas::GetVersionHistory() noexcept
{
  return {
    "2014-01-10: version 1.0: initial version, split up from DrawCanvas"
  };
}

template <class Container>
typename Container::value_type::value_type ribi::Canvas::MinElement(const Container& v)
{
  assert(v.empty() == false && "Container must have a size");
  //Obtain an initial lowest value
  typename Container::value_type::value_type minValue
    = *(std::min_element(v[0].begin(),v[0].end()));

  //Set the iterators
  const typename Container::const_iterator rowEnd = v.end();
  typename Container::const_iterator row = v.begin();
  ++row; //Move to the next position, as index 0 is already read from

  for ( ; row != rowEnd; ++row) //row is already initialized
  {
    const typename Container::value_type::value_type localMinVal
      = *(std::min_element(row->begin(),row->end()));
    if (localMinVal < minValue) minValue = localMinVal;
  }
  return minValue;
}

template <class Container>
typename Container::value_type::value_type ribi::Canvas::MaxElement(const Container& v)
{
  assert(v.empty() == false && "Container must have a size");

  //Obtain an initial heighest value
  typename Container::value_type::value_type maxValue
    = *(std::max_element(v[0].begin(),v[0].end()));

  //Set the iterators
  const typename Container::const_iterator rowEnd = v.end();
  typename Container::const_iterator row = v.begin();
  ++row; //Move to the next position, as index 0 is already read from
  for ( ; row != rowEnd; ++row) //row is already initialized
  {
    const typename Container::value_type::value_type localMaxVal
      = *(std::max_element(row->begin(),row->end()));
    if (localMaxVal > maxValue) maxValue = localMaxVal;
  }
  return maxValue;
}
