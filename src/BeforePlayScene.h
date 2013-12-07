#ifndef BEFORE_PLAY_SCENE_H_2013_12_5
#define BEFORE_PLAY_SCENE_H_2013_12_5

#include "GameScene.h"
#include "Assets.h"
#include "Camera.h"

class BeforePlayScene: public GameScene
{
  float elapsedTime;
  SpriteBatcher *batcher;
  PlayScene *scene;
public:
BeforePlayScene(GameApplication *game):GameScene(game)
  {
    elapsedTime = 0;

    batcher = new SpriteBatcher(4);
    scene = new PlayScene(game);
  }

  virtual void update(float delta)
  {
    elapsedTime += delta;
    
    if(elapsedTime > 1)
      game->replaceScene(scene);
  }
  
  virtual void render(float delta)
  {
    if(elapsedTime<1 || elapsedTime>4)
      return;
    
    const int count = 3-floor(elapsedTime-1);
    const float size = 100;
    const float ratio =size*( elapsedTime - floor(elapsedTime)) + 30; //小数点以下

    glPushMatrix();
    Camera::getInstance()->set2DView(game->Window());
    batcher->beginBatch(Assets::textureAtlas);
    batcher->drawSprite(0, 0, ratio, ratio ,Assets::charRegion[count] );
    batcher->endBatch();
    glPopMatrix();
  }

  virtual void reshape(int width, int height)
  {
  }


  virtual void finish()
  {
    delete batcher;
  }
};
#endif
