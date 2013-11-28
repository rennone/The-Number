#ifndef TITLE_SCENE_H_2013_11_19
#define TITLE_SCENE_H_2013_11_19

#include "GameScene.h"
#include "LoadScene.h"
#include "Debugger.h"

class TitleScene : public GameScene
{
public:
TitleScene(GameApplication *game):GameScene(game)
  {
  }

  ~TitleScene()
  {
  }
        
  virtual void update(float delta)
  {
    auto keyboard = game->Input()->Keyboard();
    if(keyboard->keyState(GLFW_KEY_A) == GLFW_PRESS){
      game->replaceScene(new LoadScene(game));
      return;
    }
  }
        
  virtual void render(float delta)
  {  
  }

  virtual void reshape(int width, int height)
  {
  }

  virtual void finish()
  {
  }
};
#endif
