#include "canvascolorsystems.h"

#include <cassert>

boost::bimap<ribi::CanvasColorSystem,std::string> ribi::CanvasColorSystems::m_map;

ribi::CanvasColorSystems::CanvasColorSystems()
{

}

boost::bimap<ribi::CanvasColorSystem,std::string> ribi::CanvasColorSystems::CreateMap()
{
  boost::bimap<CanvasColorSystem,std::string> m;
  m.insert(boost::bimap<CanvasColorSystem,std::string>::value_type(
    CanvasColorSystem::invert,"invert"));
  m.insert(boost::bimap<CanvasColorSystem,std::string>::value_type(
    CanvasColorSystem::normal,"normal"));
  return m;
}

std::vector<ribi::CanvasColorSystem> ribi::CanvasColorSystems::GetAll() const noexcept
{
  const std::vector<CanvasColorSystem> v {
    CanvasColorSystem::invert,
    CanvasColorSystem::normal
  };
  assert(static_cast<int>(v.size()) == static_cast<int>(CanvasColorSystem::n_types));
  return v;
}

std::string ribi::CanvasColorSystems::ToStr(const CanvasColorSystem type) const noexcept
{
  if (m_map.left.empty()) m_map = CreateMap();
  assert(!m_map.left.empty());
  assert(m_map.left.count(type));
  const std::string s = m_map.left.find(type)->second;
  return s;
}

ribi::CanvasColorSystem ribi::CanvasColorSystems::ToType(const std::string& s) const
{
  if (m_map.right.empty()) m_map = CreateMap();
  assert(!m_map.right.empty());
  assert(m_map.right.count(s) == 1);
  const CanvasColorSystem t = m_map.right.find(s)->second;
  return t;
}
