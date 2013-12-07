#ifndef _ACTOR_H_2013_11_19
#define _ACTOR_H_2013_11_19

#include <vector>
#include <string>
#include <algorithm>
#include "Game.h"

class Actor
{
public:
  enum ActorStatus
  {
    Action,
    UpdateOnly,
    RenderOnly,
    NoUse,
    Dead,
  };

Actor(std::string _name, GameApplication *_game):name(_name), game(_game)
  {
    status = Actor::Action;
  }

  virtual ~Actor()
  {
  }
  virtual void render()
  {
    std::vector<Actor*>::iterator it = children.begin();
    while(it != children.end())
    {
      Actor* child = *it;
      if(child->status == Actor::Action || child->status == Actor::RenderOnly)
        child->render();
      it++;
    }

  }

  virtual void update(float delta)
  {

    std::vector<Actor*>::iterator it = children.begin();
    while(it != children.end())
    {
      Actor* child = *it;
                        
      if(child->status == Actor::Action || child->status == Actor::UpdateOnly)
        child->update(delta);
      it++;
    }                
  }

  virtual void finish()
  {
    auto it = children.begin();
    while(it != children.end())
    {
      Actor* child = *it;

      child->finish();
      it++;
    }

    for(auto it=children.begin(); it!=children.end(); it++)
    {
      delete (*it);     
    }    
  }

  virtual void checkStatus()
  {
    if(status == Actor::Dead)
    {
      for( auto child : children)
        child->Status(Actor::Dead);
    }

    for( auto child : children)          
      child->checkStatus();

    auto end_it = std::remove_if(children.begin(), children.end(), [](Actor* p) -> bool { return p->Status() == Dead; });

    for( auto it=end_it; it != children.end(); it++)
      delete (*it);
          
    children.erase(end_it, children.end());
  }

  virtual void addChild(Actor* child)
  {
    children.push_back(child);
  }
        
  Actor* searchChild(std::string name)
  {
    if(this->name == name)
      return this;

    Actor* res = NULL;
    for(auto it = children.begin(); it != children.end(); it++)
    {
      Actor* child = *it;
      res = child->searchChild(name);
      if(res != NULL)
        return res;
    }

    return NULL;
  }

//-------------------- getter setter --------------------//
  const enum ActorStatus& Status() const
  {
    return status;
  }
        
  void Status(enum ActorStatus _status)
  {
    status = _status;
  }

  const Leap::Vector& Position() const
  {
    return position;
  }

  void Position(const Leap::Vector _position)
  {
    position = _position;
  }

  
protected:
  GameApplication* const game;  
  const std::string name;
  std::vector<Actor*> children;  
private:
  Leap::Vector position;
  enum ActorStatus status;  
};

#endif
