#ifndef TENKO_H_2013_12_07
#define TENKO_H_2013_12_07

#include "Actor.h"
#include "Assets.h"
#include "Camera.h"
#include "Debugger.h"

#include <random>

class Bullet:public Actor
{
  SpriteBatcher *batcher;
  Leap::Vector dist, direction;
  float elapsedTime;
  bool explode;
  float radius;
  static constexpr float speed = 100;
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
    radius = 30;
    Status(Actor::Action);
  }

  void update(float delta);

  void render()
  {
    if(explode)
    {
      //out of indexを避けるため-1している(たぶんならないけど)
      const int animationNumber = sizeof(Assets::explosion) / sizeof(Assets::explosion[0])-1;
      const int number = elapsedTime / animationTime * animationNumber;

      const float size = 50;
      batcher->drawSprite(Position().x, Position().y, radius, radius, Assets::explosion[number]);
    }
    else
    {
      batcher->drawSprite(dist.x, dist.y, radius, radius, Assets::fingerRegion[1]);
      batcher->drawSprite(Position().x, Position().y, radius, radius, Assets::fingerRegion[0]);
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

class Player:public Actor
{
  SpriteBatcher *batcher;
  BulletManager *bulletManager;
  int life;
public:
Player(std::string name, GameApplication *game, SpriteBatcher *_batcher)
  :Actor(name, game), batcher(_batcher)
  {
    bulletManager = new BulletManager("bulletManager", game, _batcher);
    addChild(bulletManager);
    Position(Leap::Vector(0,0,0));
    life = 4;
  }

  ~Player()
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
  }
  
  void update(float delta)
  {
    shoot();
    Actor::update(delta);
  }
  
  void render()
  {
    auto fingers = game->Input()->LeapMotion()->ScreenPoints();
    for(auto finger : fingers)
      batcher->drawSprite(finger.x, finger.y, 30, 30, Assets::fingerRegion[0]);
          
    batcher->drawSprite(Position().x, Position().y, 100, 100*Assets::player[min(3,max(0,life-1))]->ratio, Assets::player[max(0,life-1)]);
    Actor::render();
  }

  void getDamage()
  {
    life--; 
  }
};

class Enemy: public Actor
{
  Player* const target;
  SpriteBatcher* const batcher;
  const float radius;
  const float speed;
  int life;
  float elapsedTime;
public:
Enemy(std::string name, GameApplication *game, SpriteBatcher *_batcher, Player *player)
  :Actor(name, game), batcher(_batcher), radius(100), target(player), elapsedTime(0), speed(80)
  {    
  }
  
  ~Enemy()
  {
  }

  void initialize()
  {
    const int angle = rand()%360;
    const float rad = M_PI*angle/180.0;
    const float radius = 500;
    Position(Leap::Vector(radius*cos(rad), radius*sin(rad), 0 ));
    life = 1;
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
    batcher->drawSprite(Position().x, Position().y, radius, radius, -angle, Assets::enemy[life]);
  }

  bool damage(const Leap::Vector &pos,const float &_radius)
  {
    if( Status() != Actor::Action || pos.distanceTo(Position()) > 0.5*(radius + _radius))
      return false;
    
    Status(Actor::NoUse);
    return true;
  }
};

class EnemyManager : public Actor
{
  static const int enemyNum = 40;
  Enemy *enemies[enemyNum];
  int index;
  float elapsedTime;
  SpriteBatcher *batcher;
  
  void pop()
  {
    enemies[index]->initialize();
    enemies[index]->Status(Actor::Action);    
    index = (index+1)%enemyNum;    
  }
  
public:  
EnemyManager(std::string name, GameApplication *game, Player *player, SpriteBatcher *_batcher)
  :Actor(name, game), index(0), elapsedTime(0), batcher(_batcher)
  {
    for(int i=0; i<enemyNum; i++)
    {
      enemies[i] = new Enemy("enemy", game, batcher, player);
      enemies[i]->Status(Actor::NoUse);
      addChild(enemies[i]);
    }
  }

  ~EnemyManager()
  {
    for(auto enemy : enemies)
      delete enemy;
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

  //円と円の衝突
  bool damage(const Leap::Vector &pos,const float &_radius)
  {
    bool flag = false;
    for(auto enemy : enemies)
      flag = flag || enemy->damage(pos, _radius);

    return flag;
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
