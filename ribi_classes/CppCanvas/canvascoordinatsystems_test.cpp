#include "canvascoordinatsystems.h"

#include <cassert>

void CanvasCoordinatSystemsTest() noexcept
{
  const std::vector<CanvasCoordinatSystem> v = CanvasCoordinatSystems().GetAll();
  const std::size_t sz = v.size();
  for (std::size_t i=0; i!=sz; ++i)
  {
    assert(i < v.size());
    const CanvasCoordinatSystem t = v[i];
    const std::string s = CanvasCoordinatSystems().ToStr(t);
    assert(!s.empty());
    const CanvasCoordinatSystem u = CanvasCoordinatSystems().ToType(s);
    assert(u == t);
  }
}
