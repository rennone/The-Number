#include "Input.h"
#include "Debugger.h"
#include <sstream>

const std::vector<Leap::Vector> LeapMotionManager::TappedPoints()
{
  std::vector<Leap::Vector> tappedPoints;
  const auto gestureList = m_lastFrame.gestures();
  const auto screen = getController()->locatedScreens()[0];
  
  for( auto gesture : gestureList )
  {
    if(gesture.type() != Leap::Gesture::TYPE_SCREEN_TAP) continue;
    for( auto pointable : gesture.pointables())
    {
      auto normal = screen.intersect(pointable, true);
      float x = (normal.x-0.5) * screen.widthPixels();
      float y = (normal.y-0.5) * screen.heightPixels();
      tappedPoints.push_back(Leap::Vector(x,y,0));
    }
  }

  return tappedPoints;
}

const std::vector<Leap::Vector> LeapMotionManager::PushedPoints()
{
  std::vector<Leap::Vector> pushedPoints;
  const auto screen = getController()->locatedScreens()[0];
  
  const auto pointables     = m_lastFrame.pointables();
  
  int i=0;
  
  for(auto pointable: pointables)
  {
    std::stringstream ss;
    ss << "finger" << i++;
    Debugger::drawDebugInfo("Input.cpp", ss.str(), pointable.tipVelocity());
    const float border = 0.0;
    if( pointable.touchDistance() > border && pointable.touchZone() != Leap::Pointable::Zone::ZONE_NONE)
    {      
      auto normal =  screen.intersect(pointable.tipPosition(),pointable.direction(), true);    
      float x     = (normal.x-0.5) * screen.widthPixels();
      float y     = (normal.y-0.5) * screen.heightPixels();

      pushedPoints.push_back(Leap::Vector(x,y,0));
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
