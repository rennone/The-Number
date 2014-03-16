#ifndef RESULT_SCENE_H_2013_12_08
#define RESULT_SCENE_H_2013_12_08

#include <GLFW/glfw3.h>
#include "GameScene.h"
#include "TitleScene.h"
#include "Sprite.h"
#include "Debugger.h"

class ResultScene: public GameScene
{
  const int hittedNum;
  int x, y, width, height;
  SpriteBatcher *batcher;  
public:
ResultScene(GameApplication* game, int _hittedNum)
  :GameScene(game), hittedNum(_hittedNum)
  {
    int width, height;
    glfwGetFramebufferSize(game->Window(), &width, &height);
    batcher = new SpriteBatcher(20);
    x = 0;
    y = -0.8*height/2.0;
  }

  ~ResultScene()
  {
    delete batcher;
  }
  
  void update(float delta)
  {    
    auto tappedPoints = game->Input()->LeapMotion()->TappedPoints();
    if(!tappedPoints.empty())
      game->replaceScene(new TitleScene(game));
  }

  void render(float delta)
  {
    int width, height;
    glfwGetFramebufferSize(game->Window(), &width, &height);

    Camera::getInstance()->set2DView(game->Window());
    
    int hit = hittedNum;
    int x = 100;
    const int y=0;
    int size=50;

    batcher->beginBatch(Assets::textureAtlas);
    batcher->drawSprite(0, 0, width, height, Assets::background);

    if(hit == 0)
    {
      batcher->drawSprite(x, y, size, size, Assets::charRegion[0]);     
    }
    else {
      while(hit > 0)
      {
        batcher->drawSprite(x, y, size, size, Assets::charRegion[hit%10]);
        hit /=10;
        x-=size;
      }
    }

    batcher->endBatch();
  }

  void finish()
  {
  }

  void reshape(int width, int height)
  {
  }

private:
};
#endif
