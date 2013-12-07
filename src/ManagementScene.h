#ifndef MANAGEMENT_SCENE_H_2013_12_5
#define MANAGEMENT_SCENE_H_2013_12_5

#include "GameScene.h"
#include "TitleScene.h"
#include "PlayScene.h"

class ManagementScene:public GameScene
{
TitleScene *titleScene;
PlayScene *playScene;

:public
ManagementScene(GameApplication *game):GameScene(game)
{

}

virtual void update(float delta)
{

}

};
#endif
