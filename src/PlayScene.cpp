#define _USE_MATH_DEFINES
#include <cmath>

#include <string>
#include <math.h>
#include <GLFW/glfw3.h>
#include "PlayScene.h"
#include "Camera.h"
#include "Player.h"
#include "Sprite.h"

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
  SpriteBatcher::drawTexture(0,0,width, height, Assets::background);

  auto fingers = game->Input()->LeapMotion()->TappedPoints();
  batcher->beginBatch(Assets::textureAtlas);

  root->render();

  for(auto finger : fingers)
    batcher->drawSprite(finger.x, finger.y, 30, 30*Assets::fingerRegion[0]->ratio, Assets::fingerRegion[0]);  

  batcher->endBatch();
}

void PlayScene::update(float delta)
{
  root->update(delta);  
  root->checkStatus();
}

void PlayScene::reshape(int width, int height)
{
}

void PlayScene::initialize()
{
  batcher = new SpriteBatcher(500);
  root = new Actor("root", game);
  
  Player *player = new Player("player", game, batcher);
  root->addChild(player);
  root->addChild(new EnemyManager("enemyManager", game, player, batcher));
  root->addChild(new Timer("timer", game, batcher));
}
