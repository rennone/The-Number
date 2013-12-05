#include "Input.h"

const std::vector<Leap::Vector> LeapMotionManager::PushedPoints()
{
  std::vector<Leap::Vector> pushedPoints;
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
      pushedPoints.push_back(Leap::Vector(x,y,0 ));
    }
  }
  return pushedPoints;
}
