#include "Player.h"
#include "PlayScene.h"

void Bullet::update(float delta)
{    
  if(explode)
  {
    radius += 100*delta;
    if( (elapsedTime+=delta) > animationTime)    
      Status(Actor::NoUse);
    
    return;
  }
    
  Position( Position()+delta*speed*direction );
  auto d = Position()-dist;
  
  auto playScene = (PlayScene*)(game->Scene());
  auto root = playScene->Root();
  auto enemyManager  = (EnemyManager*)(root->searchChild("enemyManager"));
  
  if( d.magnitude() < speed*delta  || enemyManager->damage(Position(), radius))
  {
    Position(dist);
    explode=true;
    elapsedTime=0;
  }
}
