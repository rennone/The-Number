#ifndef GESTURE_DRAWER_H_2013_12_04
#define GESTURE_DRAWER_H_2013_12_04

#include "Actor.h"
#include "Sprite.h"
#include "Assets.h"
#include "Debugger.h"
#include <algorithm>
#include <string>
#include <vector>
#include <tuple>

class GestureDrawer:public Actor
{
public:
GestureDrawer(std::string name, GameApplication *game):Actor(name, game)
  {
  }
  virtual void add(float _x, float _y)=0;
  virtual void reset()=0;
};

class TapGestureDrawer:public GestureDrawer
{
  static const int maxFinger = 5;
  float x[100], y[maxFinger], elappedTime[maxFinger];
  SpriteBatcher *batcher;
  float animationTime;
  int head, tail;
public:
TapGestureDrawer(std::string name, GameApplication *game)
  :GestureDrawer(name, game)
  {
    batcher = new SpriteBatcher(maxFinger);
    animationTime = 0.3;
    memset(x, 0, sizeof x);
    memset(y, 0, sizeof y);
    memset(elappedTime, -1, sizeof elappedTime);
    head = tail = 0;
  }

  virtual void update(float delta)
  {

    for(int it=head; it!=tail; it = (it+1)%maxFinger)
    {      
      elappedTime[it] += delta;
      if(elappedTime[it] > animationTime)      
        head = (it+1)%maxFinger;      
    }
    Debugger::drawDebugInfo("GestureDrawer.h", "num", tail-head);
  }

  virtual void render()
  {

    batcher->beginBatch(Assets::tapAnimation);


    for(int it=head; it!=tail; it = (it+1)%maxFinger)
    {
      int number = max(0.0f, min(15.0f, elappedTime[it]/animationTime*16));      
      batcher->drawSprite(x[it], y[it], 30, 30, Assets::tapAnimationRegion[number]);
    }
    batcher->endBatch();
  }

  virtual void add(float _x, float _y)
  {
    x[tail] = _x;
    y[tail] = _y;
    elappedTime[tail] = 0;
    tail = (tail+1)%maxFinger;
  }
  virtual void reset()
  {
    head = tail = 0;
  }
};

class CircleGestureDrawer:public GestureDrawer
{
  static const int maxPaint = 200;

  tuple<float, float> paints[maxPaint];
  SpriteBatcher *batcher;
  int head, tail;
public:
CircleGestureDrawer(std::string name, GameApplication *game)
  :GestureDrawer(name, game)
  {
    batcher = new SpriteBatcher(maxPaint);

    head=tail=0;
  }

  virtual void update(float delta)
  {

  }

  virtual void render()
  {
    batcher->beginBatch(Assets::tapAnimation);
    for(int it=head; it != tail; it=(it+1)%maxPaint)
    {
      Debugger::drawDebugInfo("Gesture.h", "num", std::get<0>(paints[it]));
      batcher->drawSprite(std::get<0>(paints[it]), std::get<1>(paints[it]), 30, 30, Assets::tapAnimationRegion[10]);
//      batcher->drawSprite(100, 100, 30, 30, Assets::tapAnimationRegion[10]);
    }
    batcher->endBatch();
  }

  virtual void add(float _x, float _y)
  {
    paints[tail] = make_tuple(_x,_y);
    tail = (tail+1)%maxPaint;
  }

  virtual void reset()
  {
    head=tail=0;
  }
};


#endif
