#ifndef TENKO_H_2013_12_07
#define TENKO_H_2013_12_07

#include "Actor.h"
#include "Assets.h"
#include "Camera.h"
#include "Debugger.h"

#include <random>

class Tenko:public Actor
{
  SpriteBatcher *batcher;
  int life;
public:
  Tenko(std::string name, GameApplication *game):Actor(name, game)
  {
    Position(Leap::Vector(0,0,0));
    batcher = new SpriteBatcher(20);
    life = 4;
  }
  
  void update(float delta)
  {
    
  }
  
  void render()
  {
    auto leapMotion = game->Input()->LeapMotion();
    auto fingers = leapMotion->ScreenPointsWithTapState();

    batcher->beginBatch(Assets::textureAtlas);
    for( auto finger : fingers)
    {
      auto point = std::get<0>(finger);
      auto state = std::get<1>(finger);
      if(state==-1)continue;
      Debugger::drawDebugInfo("Tenko", "tenko", point);
      batcher->drawSprite(point.x, point.y, 15, 15, Assets::fingerRegion[state]);
    }
    
    batcher->drawSprite(Position().x, Position().y, 30, 30*Assets::player->ratio, Assets::character[min(3,max(0,life-1))]);
    batcher->endBatch();
  }

  void getDamage()
  {
    life--; 
  }
};

class Onmyo: public Actor
{
  Tenko *target;
  SpriteBatcher *batcher;
  float radius;
  int life;
public:
Onmyo(std::string name, GameApplication *game, SpriteBatcher *_batcher, Tenko *tenko)
  :Actor(name, game), batcher(_batcher), radius(50), target(tenko)
  {    
  }

  void initialize()
  {
    int angle = rand()%360;
    float rad = M_PI*angle/180.0;
    const float radius = 500;
    float x =  radius*cos(rad);
    float y =  radius*sin(rad);
    Position(Leap::Vector(x, y, 0 ));
    life = rand()%9+1;
  }

  void update(float delta)
  {

    auto d = target->Position() - Position();
    float length = d.magnitude();

    if( length<30)
    {
      target->getDamage();
      Status(Actor::NoUse);
      return;
    }

    auto fingers = game->Input()->LeapMotion()->PushedPoints();

    for(auto finger : fingers)
    {
      if( finger.distanceTo(Position()) > radius) continue;
      
      if(--life>0) continue;
      
      Status(Actor::NoUse);
      return;
    }
    
    float speed = 50;
    Position( Position()+delta*speed*d.normalized() );
  }

  void render()
  {
    batcher->drawSprite(Position().x, Position().y, 50, 50, Assets::charRegion[life]);
  }
};

class OnmyoCreater : public Actor
{
  static const int OnmyoNum = 40;
  Onmyo *onmyos[OnmyoNum];
  int index;
  float elapsedTime;
  SpriteBatcher *batcher;
  
  void pop()
  {
    onmyos[index]->initialize();
    onmyos[index]->Status(Actor::Action);    
    index = (index+1)%OnmyoNum;    
  }
  
public:  
OnmyoCreater(std::string name, GameApplication *game, Tenko *tenko)
    :Actor(name, game), index(0), elapsedTime(0)
  {
    
    batcher = new SpriteBatcher(20);
    for(int i=0; i<OnmyoNum; i++)
    {
      onmyos[i] = new Onmyo("onmyo", game, batcher, tenko);
      onmyos[i]->Status(Actor::NoUse);
      addChild(onmyos[i]);
    }

  }

  virtual void update(float delta)
  {
    elapsedTime += delta;

    if (elapsedTime > 1)
    {

      pop();
      elapsedTime-=1;
    }
    Debugger::drawDebugInfo("Tenko.h", "elasped", elapsedTime);
    Actor::update(delta);
  }

  virtual void render()
  {
    batcher->beginBatch(Assets::textureAtlas);
    Actor::render();
    batcher->endBatch();
  }
};

#endif
