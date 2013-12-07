#include "TitleScene.h"
#include "Camera.h"
#include "GLFW/glfw3.h"
#include "PlayScene.h"
#include "Debugger.h"
#include "Assets.h"
#include "GLutil.h"
#include "PlayScene.h"

TitleScene::TitleScene(GameApplication *game)
  :GameScene(game)
{
  batcher = new SpriteBatcher(20);

  strPos[0] = make_tuple(0, 200-0*100, 100, 50, Assets::stringTitleRegion);
  strPos[1] = make_tuple(0, 200-1*100, 100, 50, Assets::stringPlayRegion);
  strPos[2] = make_tuple(0, 200-2*100, 100, 50, Assets::stringScoreRegion);
  strPos[3] = make_tuple(0, 200-3*100, 100, 50, Assets::stringExitRegion);
}

void TitleScene::update(float delta)
{
  auto keyboard = game->Input()->Keyboard();

  auto leapMotion = game->Input()->LeapMotion();

  const auto pushed = leapMotion->PushedPoints();

  int next = -1;
  for(const auto  p : pushed)
  {
    for(int i=1; i<4; i++)
    {
      float x = std::get<0>(strPos[i]);
      float y = std::get<1>(strPos[i]);
      float w = std::get<2>(strPos[i]);
      float h = std::get<3>(strPos[i]);
      if( (x-w/2)<=p.x && p.x<=(x+w/2) && (y-h/2)<=p.y && p.y<=(y+h/2) )
        next=i;     
    }
  }

  switch(next)
  {
  case 1:
    game->replaceScene(new PlayScene(game));
    break;
  case 2:
    break;
  case 3:
    game->terminate();
    break;
  default:
    break;
  }  
  if(keyboard->keyState(GLFW_KEY_ENTER) == GLFW_PRESS)    game->replaceScene(new PlayScene(game));  
}
        
void TitleScene::render(float delta)
{
  int width, height;
  glfwGetFramebufferSize(game->Window(), &width, &height);

  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
  glPushMatrix();
  
  Camera::getInstance()->set2DView(game->Window());  
/*
  batcher->beginBatch(Assets::titleBground);
  batcher->drawSprite(0,0,width, height, Assets::titleBackgroundRegion);
  batcher->endBatch();
*/
  batcher->beginBatch(Assets::textureAtlas);
  for(int i=0; i<4; i++)
    batcher->drawSprite(std::get<0>(strPos[i]), std::get<1>(strPos[i]),
                        std::get<2>(strPos[i]), std::get<3>(strPos[i]), std::get<4>(strPos[i]) );
  
  auto fingers = game->Input()->LeapMotion()->ScreenPointsWithTapState();
  for( auto finger : fingers)
  {
    auto pos = std::get<0>(finger);
    auto state = std::get<1>(finger);
    batcher->drawSprite(pos.x, pos.y, 15, 15, Assets::fingerRegion[max(0,state)]);
  }
  
  batcher->endBatch();
  
  glPopMatrix();
  glPopAttrib();

}

void TitleScene::finish()
{
  delete batcher;
}
