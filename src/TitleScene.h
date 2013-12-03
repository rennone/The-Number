#ifndef TITLE_SCENE_H_2013_11_19
#define TITLE_SCENE_H_2013_11_19

#include "GameScene.h"

class TitleScene : public GameScene
{
public:
TitleScene(GameApplication *game):GameScene(game)
  {
  }

  ~TitleScene()
  {
  }
        
  virtual void update(float delta);
        
  virtual void render(float delta);

  virtual void reshape(int width, int height)
  {
  }

  virtual void finish()
  {
  }
};
#endif
