#include "Input.h"
#include "Debugger.h"

const std::vector<Leap::Vector> LeapMotionManager::PushedPoints()
{
  std::vector<Leap::Vector> pushedPoints;
  const auto screen = getController()->locatedScreens()[0];
  const auto pointables = m_lastFrame.pointables();

  static bool flag = false;
  for(auto pointable: pointables)
  {
    auto normal =  screen.intersect(pointable.tipPosition(),pointable.direction(), true);    
    float x     = (normal.x-0.5) * screen.widthPixels();
    float y     = (normal.y-0.5) * screen.heightPixels();
    
    const float border = 0.0;
    if( pointable.touchDistance() > border && pointable.touchZone() != Leap::Pointable::Zone::ZONE_NONE)
    {
      if(flag) continue;
      flag = true;
      Debugger::drawDebugInfo("Input.cpp", "touch",  pointable.touchDistance());
      pushedPoints.push_back(Leap::Vector(x,y,0));
    } else
    {
      Debugger::drawDebugInfo("Input.cpp", "else",  pointable.touchDistance());
      flag = false;
    }
  }
  return pushedPoints;
}

const std::vector<std::tuple<Leap::Vector, int>> LeapMotionManager::ScreenPointsWithTapState()
{
  std::vector< std::tuple<Leap::Vector, int> > pushedPoints;
  const auto screen = getController()->locatedScreens()[0];
  const auto pointables = m_lastFrame.pointables();
  for(auto pointable: pointables)
  {
    auto normal =  screen.intersect(pointable.tipPosition(),pointable.direction(), true);    
    float x     = (normal.x-0.5) * screen.widthPixels();
    float y     = (normal.y-0.5) * screen.heightPixels();
    
    const float border = 0.0;      
    if(pointable.touchDistance() > border && pointable.touchZone() != Leap::Pointable::Zone::ZONE_NONE)
    {
      pushedPoints.push_back( std::make_tuple(Leap::Vector(x,y,0), 1));
    }
    else if(pointable.touchDistance() < border && pointable.touchZone() != Leap::Pointable::Zone::ZONE_NONE)
    {
      pushedPoints.push_back( std::make_tuple(Leap::Vector(x,y,0), 0));
    }
    else
    {
      pushedPoints.push_back( std::make_tuple(Leap::Vector(x,y,0), -1));
    }    
  }
  return pushedPoints;
}
