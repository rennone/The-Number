#ifndef NUMBERS_H_2013_11_26
#define NUMBERS_H_2013_11_26

#include "Actor.h"
#include "Sprite.h"
#include "Assets.h"
class Numbers:public Actor
{  
  Leap::Vector numbers[9];
  float numberSizes[9];
  int target;
  void reset();
  void renderNumber();
  void renderFinger();
  Sprite *spriteNumber, *spriteFinger;
public:
Numbers(std::string name, GameApplication *game):Actor(name, game)
  {
    spriteNumber = new Sprite(Assets::numbers);
    spriteFinger = new Sprite(Assets::target);
    reset();
  }
  ~Numbers()
  {
    delete spriteNumber;
    delete spriteFinger;
  }
  
  virtual void update(float delta);
  virtual void render();
};

#endif
