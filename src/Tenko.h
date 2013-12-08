#ifndef TENKO_H_2013_12_07
#define TENKO_H_2013_12_07

#include "Actor.h"
#include "Assets.h"
#include "Camera.h"
#include "Debugger.h"

#include <random>

class FingerPaint: public Actor
{
  static constexpr float animationTime = 1.0f;  //floatをコンパイル時定数にする為に constexprを使う
  float elapsedTime;
  SpriteBatcher* const batcher;
public:
FingerPaint(std::string name, GameApplication *game, SpriteBatcher *_batcher)
  :Actor(name, game), elapsedTime(0), batcher(_batcher)
  {
  }

  void set(Leap::Vector pos)
  {
    elapsedTime = 0;
    Position(pos);
    Status(Actor::Action);
  }

  void update(float delta)
  {
    elapsedTime += delta;
    if(elapsedTime>=animationTime)
      Status(Actor::NoUse);
  }

  void render()
  {
    //out of indexを避けるため-1している(たぶんならないけど)
    const int animationNumber = sizeof(Assets::tapAnimationRegion) / sizeof(Assets::tapAnimationRegion[0])-1;
    const int number = elapsedTime / animationTime * animationNumber;

    const float size = 50;
    batcher->drawSprite(Position().x, Position().y, size, size, Assets::tapAnimationRegion[number]);    
  }  
};

class FingerPaintManager: public Actor
{
  static constexpr int maxPaints=30;
  SpriteBatcher* batcher;
  FingerPaint*   paints[maxPaints];
  int index;
public:
FingerPaintManager(std::string name, GameApplication *game, SpriteBatcher *_batcher)
  :Actor(name, game), batcher(_batcher), index(0)
  {
    for(int i=0; i<maxPaints; i++)
    {
      paints[i] = new FingerPaint("fingerPaint", game, batcher);
      paints[i]->Status(Actor::NoUse);
      addChild(paints[i]);
    }
  }

  void update(float delta)
  {
    Actor::update(delta);
  }

  void render()
  {
    Actor::render();
  }

  void addFingerPaint(const Leap::Vector paint)
  {
    paints[index]->set(paint);
    index = (index+1)%maxPaints;
  }
};

class Bullet:public Actor
{
  SpriteBatcher *batcher;
  Leap::Vector dist, direction;
  float elapsedTime;
  static constexpr float speed = 100;
  bool explode;
  static constexpr float animationTime=1.0;
public:
  Bullet(std::string name, GameApplication *game, SpriteBatcher *_batcher)
    :Actor(name, game), batcher(_batcher)
  {
  }

  ~Bullet()
  {
  }
  
  void set(Leap::Vector src, Leap::Vector dist)
  {
    Position(src);
    this->dist = dist;
    auto d = dist - src;
    direction = d.normalized();
    elapsedTime=0;
    explode = false;
    Status(Actor::Action);
  }

  void update(float delta)
  {
    Debugger::drawDebugInfo("Tenko.h", "direction", direction);
    auto playScene = (PlayScene*)(game->Scene());
    auto root = playScene->Root();
    auto onmyoCreater       = (OnmyoCreater*)(root->searchChild("onmyoCreater"));
    onmyoCreater->tapCheck(Position());
    
    if(explode)
    {
      if( (elapsedTime+=delta) > animationTime)
        Status(Actor::NoUse);
      
      return;
    }
    
    Position( Position()+delta*speed*direction );
    auto d = Position()-dist;
    if( d.magnitude() < speed*delta )
    {
      Position(dist);
      explode=true;
      elapsedTime=0;
    }
  }

  void render()
  {
    const int size = 30;
    if(explode)
    {
      //out of indexを避けるため-1している(たぶんならないけど)
      const int animationNumber = sizeof(Assets::tapAnimationRegion) / sizeof(Assets::tapAnimationRegion[0])-1;
      const int number = elapsedTime / animationTime * animationNumber;

      const float size = 50;
      batcher->drawSprite(Position().x, Position().y, size, size, Assets::tapAnimationRegion[number]);    
    }
    else
    {
      batcher->drawSprite(dist.x, dist.y, size, size, Assets::fingerRegion[1]);
      batcher->drawSprite(Position().x, Position().y, size, size, Assets::fingerRegion[0]);
    }
  }
};

class BulletManager: public Actor
{
  static constexpr int bulletNum = 50;
  Bullet* bullets[bulletNum];
  int index;
public:
BulletManager(std::string name, GameApplication *game, SpriteBatcher *batcher)
  :Actor(name, game),index(0)
  {
    for(int i=0; i<bulletNum; i++)
    {
      bullets[i] = new Bullet("bullet",game, batcher);
      bullets[i]->Status(Actor::NoUse);
      addChild(bullets[i]);
    }
  }
  ~BulletManager()
  {
    for(int i=0; i<bulletNum; i++)    
      delete bullets[i];    
  }

  void shoot(Leap::Vector src, Leap::Vector dist)
  {
    bullets[index]->set(src, dist);
    index = (index+1)%bulletNum;
  }
};

class Tenko:public Actor
{
  SpriteBatcher *batcher;
  BulletManager *bulletManager;
  int life;
public:
Tenko(std::string name, GameApplication *game, SpriteBatcher *_batcher)
  :Actor(name, game), batcher(_batcher)
  {
    bulletManager = new BulletManager("bulletManager", game, _batcher);
    addChild(bulletManager);
    Position(Leap::Vector(0,0,0));
    life = 4;
  }

  ~Tenko()
  {
    delete bulletManager;
  }

  void shoot()
  {
    auto fingers = game->Input()->LeapMotion()->TappedPoints();
    for(auto finger : fingers)   
    {
      bulletManager->shoot(Position(), finger);
    }    
    /*
    auto onmyoCreater       = (OnmyoCreater*)(root->searchChild("onmyoCreater"));
    auto fingerPaintManager = (FingerPaintManager*)(root->searchChild("fingerPaintManager"));

    if(fingerPaintManager == NULL)
      Debugger::drawDebugInfo("PlayScene.cpp", "fingerPaintManager", "NULL");

    auto fingers = game->Input()->LeapMotion()->TappedPoints();
    for(auto finger : fingers)
    {
      onmyoCreater->tapCheck(finger);
      fingerPaintManager->addFingerPaint(finger);    
    }
    */
  }
  
  void update(float delta)
  {
    shoot();
    Actor::update(delta);
  }
  
  void render()
  {    
    batcher->drawSprite(Position().x, Position().y, 100, 100*Assets::character[min(3,max(0,life-1))]->ratio, Assets::character[min(3,max(0,life-1))]);
    Actor::render();
  }

  void getDamage()
  {
    life--; 
  }
};

class Onmyo: public Actor
{
  Tenko* const target;
  SpriteBatcher* const batcher;
  const float radius;
  const float speed;
  int life;
  float elapsedTime;
public:
Onmyo(std::string name, GameApplication *game, SpriteBatcher *_batcher, Tenko *tenko)
  :Actor(name, game), batcher(_batcher), radius(150), target(tenko), elapsedTime(0), speed(30)
  {    
  }

  void initialize()
  {
    const int angle = rand()%360;
    const float rad = M_PI*angle/180.0;
    const float radius = 500;
    Position(Leap::Vector(radius*cos(rad), radius*sin(rad), 0 ));
    life = rand()%9+1;
  }

  void update(float delta)
  {
    elapsedTime += delta;
    auto d = target->Position() - Position();
    float length = d.magnitude();

    if( length<30)
    {
      target->getDamage();
      Status(Actor::NoUse);
      return;
    }
    Position( Position()+delta*speed*d.normalized() );
  }

  void render()
  {
    auto angle = (int)(elapsedTime*360)%360;
    batcher->drawSprite(Position().x, Position().y, radius, radius, -angle, Assets::enemy);
    batcher->drawSprite(Position().x, Position().y, radius/2, radius/2, Assets::charRegion[life]);
  }

  void tapCheck(Leap::Vector pos)
  {
    if( pos.distanceTo(Position()) > radius) return;      
    if(--life > 0) return;
      
    Status(Actor::NoUse);
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
OnmyoCreater(std::string name, GameApplication *game, Tenko *tenko, SpriteBatcher *_batcher)
  :Actor(name, game), index(0), elapsedTime(0), batcher(_batcher)
  {
    for(int i=0; i<OnmyoNum; i++)
    {
      onmyos[i] = new Onmyo("onmyo", game, batcher, tenko);
      onmyos[i]->Status(Actor::NoUse);
      addChild(onmyos[i]);
    }
  }

  ~OnmyoCreater()
  {

    for(auto onmyo : onmyos)
      delete onmyo;
  }    

  virtual void update(float delta)
  {
    elapsedTime += delta;

    if (elapsedTime > 2)
    {
      pop();
      elapsedTime-=2;
    }
    Actor::update(delta);
  }

  virtual void render()
  {
    Actor::render();
  }

  void tapCheck(Leap::Vector pos)
  {
    for(auto onmyo : onmyos)    
      onmyo->tapCheck(pos);    
  }
};

class Timer:public Actor
{
  float elapsedTime;
  SpriteBatcher *batcher;
public:
Timer(std::string name, GameApplication *game, SpriteBatcher *_batcher)
  :Actor(name, game), elapsedTime(0), batcher(_batcher)
  {
  }

  ~Timer()
  {
  }

  void update(float delta)
  {
    elapsedTime += delta;
  }

  void render()
  {
    int timer[6];  
    timer[0] = ((int)(elapsedTime*100))%10;
    timer[1] = ((int)(elapsedTime*10 ))%10;
    timer[2] = 10; //小数点
    timer[3] = ((int)(elapsedTime    ))%10;
    timer[4] = ((int)(elapsedTime/10 ))%10;
    timer[5] = ((int)(elapsedTime/100))%10;

    int width, height;
    glfwGetFramebufferSize(game->Window(), &width, &height);

    const int wSize = 30;
    const int hSize = 30;
    const int x = width/2 - wSize/2;
    const int y = height/2- hSize/2;

    for(int i=0; i<6; i++)
      batcher->drawSprite(x-i*wSize, y, wSize, hSize, Assets::charRegion[timer[i]]);
   
  }
};

#endif
