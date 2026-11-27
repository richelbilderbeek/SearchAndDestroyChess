#ifndef RIBI_CANVAS_H
#define RIBI_CANVAS_H

#include <iosfwd>
#include <string>
#include <vector>




#include "canvascolorsystem.h"
#include "canvascoordinatsystem.h"

namespace ribi {

///A Canvas is an ASCII art class for drawing (DrawCanvas),
///displaying images (ImageCanvas) and displaying text (TextCanvas)
struct Canvas
{
  Canvas();
  virtual ~Canvas() noexcept {}

  //From http://www.richelbilderbeek.nl/CppGetAsciiArtGradient.htm
  static std::vector<char> GetAsciiArtGradient() noexcept;

  ///Obtain the height of the canvas is characters
  virtual int GetHeight() const noexcept = 0;

  ///Obtain the version of this class
  static std::string GetVersion() noexcept;

  ///Obtain the version history of this class
  static std::vector<std::string> GetVersionHistory() noexcept;

  ///Obtain the width of the canvas is characters
  virtual int GetWidth() const noexcept = 0;

  ///Load a Canvas from std::strings
  ///DON'T: make it a constructor instead
  ///virtual void Load(const std::vector<std::string>& v) = 0;

  ///Convert the Canvas to std::strings
  virtual std::vector<std::string> ToStrings() const noexcept = 0;

  private:

  ///Check if a coordinat is in the range of the Canvas
  bool IsInRange(const int x, const int y) const;

  //Obtains the minimum element of a 2D container
  //From http://www.richelbilderbeek.nl/CppMinElement.htm
  template <class Container>
  static typename Container::value_type::value_type MinElement(const Container& v);

  //Obtains the maximal element of a 2D container
  //From http://www.richelbilderbeek.nl/CppMaxElement.htm
  template <class Container>
  static typename Container::value_type::value_type MaxElement(const Container& v);
};

} //~namespace ribi

#endif // RIBI_CANVAS_H
