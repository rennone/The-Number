#ifndef INPUT_H_2013_11_19
#define INPUT_H_2013_11_19

#include <Leap.h>
#include <map>
#include <GLFW/glfw3.h>

class KeyboardManager
{
public: 
  virtual void keyAction(int key, int action)
  {
    nextKeyStateMap[key] = action;
  }

  virtual int keyState(int key)
  {
    return keyStateMap[key];
  }

  virtual void update(float delta)
  {
    keyStateMap = nextKeyStateMap;
    
    for(auto it = nextKeyStateMap.begin(); it != nextKeyStateMap.end(); it++)
      if((*it).second == GLFW_PRESS)
        (*it).second = GLFW_REPEAT;   
  }
        
  static KeyboardManager *getInstance()
  {
    static KeyboardManager instance;
    return &instance;
  }
  
private:  
  KeyboardManager(void){}        
  KeyboardManager& operator=(const KeyboardManager& obj);
  
  std::map<int, int> keyStateMap;
  std::map<int, int> nextKeyStateMap;

};

class MouseManager
{
public:
  virtual void update()
  {
  }
  virtual void mouse(int button, int state, int x, int y)
  {
  }

  virtual void motion(int x, int y)
  {
  }

  virtual void update(float delta)
  {
  }

  static MouseManager* getInstance()
  {
    static MouseManager instance;
    return &instance;
  }
private:
  MouseManager(void)
  {
  }
  MouseManager& operator=(const MouseManager& obj);        
};

class LeapMotionManager
{
public:
  static Leap::Controller* getController()
  {
    static Leap::Controller s_controller;
    return &s_controller;
  }

  static LeapMotionManager* getInstance()
  {
    static LeapMotionManager instance;    
    return &instance;
  }

  virtual void update(float delta)
  {
    auto frame = LeapMotionManager::getController()->frame();
    if(frame.id() == m_lastFrame.id()) return;

    m_lastFrame = frame;
  }

  //中心が原点 左下が(-width/2, -height/2)
  const std::vector<Leap::Vector> ScreenPoints()
  {
    std::vector<Leap::Vector> screenFingers;
    auto screen = getController()->locatedScreens()[0];
    for( auto finger : m_lastFrame.fingers() )
    {
      auto normal =  screen.intersect(finger, true);
      screenFingers.push_back( Leap::Vector( (normal.x-0.5)*screen.widthPixels(), (normal.y-0.5)*screen.heightPixels(), 0) );
    }
    return screenFingers;
  }

  const std::vector<Leap::Vector> PushedPoints();
          
  Leap::Frame Frame()
  {
    return m_lastFrame;
  }
  
/*
  CircleGesture()
  {
  }
  
*/

private:
  LeapMotionManager(){
    getController()->enableGesture( Leap::Gesture::TYPE_SCREEN_TAP );
    getController()->enableGesture( Leap::Gesture::TYPE_CIRCLE );
  }
  LeapMotionManager& operator=(const LeapMotionManager& obj);
  Leap::Frame m_lastFrame;
};

class InputManager
{
public:
  static InputManager *getInstance()
  {
    static InputManager instance;
    return &instance;
  }

  virtual void update(float delta)
  {
    keyboad->update(delta);
    mouse->update(delta);
    leapMotion->update(delta);
  }

  virtual KeyboardManager* Keyboard()
  {
    return keyboad;
  }

  virtual MouseManager* Mouse()
  {
    return mouse;
  }

  virtual LeapMotionManager* LeapMotion()
  {
    return leapMotion;
  }

private:
  InputManager()
  {
       keyboad = KeyboardManager::getInstance();
         mouse = MouseManager::getInstance();
    leapMotion = LeapMotionManager::getInstance();
  }

  ~InputManager()
  {
  }

  InputManager& operator=(const InputManager& obj);

  KeyboardManager *keyboad;
  MouseManager *mouse;
  LeapMotionManager *leapMotion;
};

#endif
