


#include "textcanvas.h"

#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
#include <iterator>

#include <boost/math/constants/constants.hpp>

#include "dotmatrixstring.h"



#ifndef NDEBUG
//Boost.Test here
void ribi::TextCanvas::Test() noexcept
{
  //Drawing text
  {
    const int maxx = 90;
    const int maxy = 18;
    TextCanvas canvas(maxx,maxy);
    std::stringstream s_before;
    s_before << canvas;
    const std::string str_before {s_before.str() };
    assert(static_cast<int>(str_before.size()) - maxy == maxx * maxy); //-maxy, as newlines are added
    assert(std::count(str_before.begin(),str_before.end(),' ') == maxx * maxy); //Only spaces

    canvas.PutText(1,1,"Hello world");

    std::stringstream s_after;
    s_after << canvas;
    const std::string str_after {s_after.str() };
    assert(std::count(str_after.begin(),str_after.end(),' ') != maxx * maxy); //Line trly drawn
  }
  //Is a text that starts before the canvas partially accepted?
  {
    const int maxx = 3;
    const int maxy = 4;
    TextCanvas canvas(maxx,maxy);
    std::stringstream s_before;
    s_before << canvas;
    const std::string str_before {s_before.str() };
    assert(static_cast<int>(str_before.size()) - maxy == maxx * maxy); //-maxy, as newlines are added
    assert(std::count(str_before.begin(),str_before.end(),' ') == maxx * maxy); //Only spaces

    canvas.PutText(-5,1,"Hello world");

    std::stringstream s_after;
    s_after << canvas;
    const std::string str_after {s_after.str() };
    assert(std::count(str_after.begin(),str_after.end(),' ') != maxx * maxy); //Line truely drawn
  }
  //Copy constructor
  {
    const TextCanvas a(3,4);
    const TextCanvas b(a);
    assert(a==b);
  }
  //Assignment operator
  {
    const TextCanvas a(3,4);
    TextCanvas b(4,5);
    assert(a!=b);
    b = a;
    assert(a==b);
  }
}
#endif
