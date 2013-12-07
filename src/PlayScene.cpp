#define _USE_MATH_DEFINES
#include <cmath>

#include <string>
#include <math.h>
#include <GLFW/glfw3.h>
#include "PlayScene.h"
#include "Camera.h"
#include "Numbers.h"
#include "GLutil.h"
#include "Tenko.h"
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
  root->render();  
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
  root = new Actor("root", game);
  // root->addChild(new Numbers("numbers", game));
  Tenko *tenko = new Tenko("tenko", game);
  root->addChild(tenko);
  root->addChild(new OnmyoCreater("onmyoCreater", game, tenko));
}
