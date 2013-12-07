#ifndef TITLE_SCENE_H_2013_11_19
#define TITLE_SCENE_H_2013_11_19

#include "GameScene.h"

#include "Sprite.h"
#include <tuple>

class TitleScene : public GameScene
{
  SpriteBatcher *batcher;
  std::tuple<float, float, float, float, TextureRegion*> strPos[5];
public:
  TitleScene(GameApplication *game);

  ~TitleScene()
  {
  }
        
  virtual void update(float delta);
        
  virtual void render(float delta);

  virtual void reshape(int width, int height)
  {
  }
  virtual void finish();

};
#endif
