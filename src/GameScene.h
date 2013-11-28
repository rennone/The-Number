#ifndef GAME_SCENE_H_2013_11_19
#define GAME_SCENE_H_2013_11_19

#include "Game.h"
#include "Camera.h"

class GameApplication;

class GameScene
{
protected:
  GameApplication *game;

public:
GameScene(GameApplication *_game):game(_game)
  {  }
  virtual ~GameScene(){ }
  virtual void render(float delta) = 0;
  virtual void update(float delta) = 0;
  virtual void reshape(int width, int height) = 0;
  virtual void finish()=0;
};
#endif
