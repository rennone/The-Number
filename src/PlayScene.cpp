#define _USE_MATH_DEFINES
#include <cmath>

#include <string>
#include <math.h>
#include <GLFW/glfw3.h>
#include "PlayScene.h"
#include "Camera.h"
#include "Player.h"
#include "Sprite.h"
#include "ResultScene.h"

PlayScene::PlayScene(GameApplication *game):GameScene(game)
{
  initialize();
}

PlayScene::~PlayScene()
{
  
}

void PlayScene::render(float delta)
{
  int width, height;
  glfwGetFramebufferSize(game->Window(), &width, &height);
  Camera::getInstance()->set2DView(game->Window());

  auto fingers = game->Input()->LeapMotion()->TappedPoints();
  
  batcher->beginBatch(Assets::textureAtlas);
  
  batcher->drawSprite(0,0, width, height, Assets::background);
  root->render();
  for(auto finger : fingers)
    batcher->drawSprite(finger.x, finger.y, 30, 30*Assets::fingerRegion[0]->ratio, Assets::fingerRegion[0]);  

  batcher->endBatch();

  Timer *timer = (Timer*)(root->searchChild("timer"));
  
  if(timer->timeOut())
    fade((elapsedTime-timer->limit()));
  
}

void PlayScene::update(float delta)
{
  elapsedTime += delta;
  Timer *timer = (Timer*)(root->searchChild("timer"));
  
  if(timer->timeOut())
  {
    if(elapsedTime-timer->limit() > 2)
    {
      auto player = (Player*)(root->searchChild("player"));
      game->replaceScene(new ResultScene(game, player->Hitted()));
    }
    return;
  }
  
  game->Input()->LeapMotion()->PushedPoints();
  root->update(delta);  
  root->checkStatus();
}

void PlayScene::reshape(int width, int height)
{
  
}

void PlayScene::initialize()
{
  elapsedTime = 0;
  batcher = new SpriteBatcher(500);
  root = new Actor("root", game);
  
  Player *player = new Player("player", game, batcher);
  root->addChild(player);
  root->addChild(new EnemyManager("enemyManager", game, player, batcher));
  root->addChild(new Timer("timer", game, batcher));
}


void PlayScene::fade(const float &alpha) const
{
  int width, height;
  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT );
  glfwGetFramebufferSize(game->Window(), &width, &height);
  glColor4d(0,0,0,alpha);
  glBegin(GL_POLYGON);
  glVertex2d(-width, -height);
  glVertex2d(+width, -height);
  glVertex2d(+width, +height);
  glVertex2d(+width, +height);
  glVertex2d(-width, +height);
  glVertex2d(-width, -height);
  glEnd();
  glPopAttrib();
}
