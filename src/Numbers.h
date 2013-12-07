#ifndef NUMBERS_H_2013_11_26
#define NUMBERS_H_2013_11_26

#include "Actor.h"
#include "Sprite.h"
#include "Assets.h"
#include "GestureDrawer.h"

class Numbers:public Actor
{
  enum Gestures
  {
    Tap = 0,
    Circle,
    Punch,
  };
  
  Leap::Vector numbers[9];
  float numberSizes[9];
  int target;
  void reset();
  void renderNumber();
  void renderFinger();
  void renderTime();
  void checkTouch();
  void checkCircle();
  GestureDrawer* gestures[3];
  SpriteBatcher *batcher;
  Gestures gesture;
  float elapsedTime;
  float slowOutTime; //終わった後の余韻用
public:
Numbers(std::string name, GameApplication *game):Actor(name, game)
  {    
    batcher = new SpriteBatcher(100); //100個のスプライト
    gestures[Numbers::Tap] = new TapGestureDrawer("TapGesture", game);
    gestures[Numbers::Circle] = new CircleGestureDrawer("CircleGesture", game);
    reset();
  }
  ~Numbers()
  {
    delete batcher;
  }
  
  virtual void update(float delta);
  virtual void render();
};
#endif
