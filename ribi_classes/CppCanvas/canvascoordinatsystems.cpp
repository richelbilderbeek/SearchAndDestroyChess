#include "canvascoordinatsystems.h"

#include <cassert>

boost::bimap<ribi::CanvasCoordinatSystem,std::string> ribi::CanvasCoordinatSystems::m_map;

ribi::CanvasCoordinatSystems::CanvasCoordinatSystems()
{

}

boost::bimap<ribi::CanvasCoordinatSystem,std::string> ribi::CanvasCoordinatSystems::CreateMap()
{
  boost::bimap<CanvasCoordinatSystem,std::string> m;
  m.insert(boost::bimap<CanvasCoordinatSystem,std::string>::value_type(
    CanvasCoordinatSystem::graph,"graph"));
  m.insert(boost::bimap<CanvasCoordinatSystem,std::string>::value_type(
    CanvasCoordinatSystem::screen,"screen"));
  return m;
}

std::vector<ribi::CanvasCoordinatSystem> ribi::CanvasCoordinatSystems::GetAll() const noexcept
{
  const std::vector<CanvasCoordinatSystem> v {
    CanvasCoordinatSystem::graph,
    CanvasCoordinatSystem::screen
  };
  assert(static_cast<int>(v.size()) == static_cast<int>(CanvasCoordinatSystem::n_types));
  return v;
}

std::string ribi::CanvasCoordinatSystems::ToStr(const CanvasCoordinatSystem type) const noexcept
{
  if (m_map.left.empty()) m_map = CreateMap();
  assert(!m_map.left.empty());
  assert(m_map.left.count(type));
  const std::string s = m_map.left.find(type)->second;
  return s;
}

ribi::CanvasCoordinatSystem ribi::CanvasCoordinatSystems::ToType(const std::string& s) const
{
  if (m_map.right.empty()) m_map = CreateMap();
  assert(!m_map.right.empty());
  assert(m_map.right.count(s) == 1);
  const CanvasCoordinatSystem t = m_map.right.find(s)->second;
  return t;
}
