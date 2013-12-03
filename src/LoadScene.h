#ifndef LOAD_SCENE_H_2013_11_19
#define LOAD_SCENE_H_2013_11_19

#include "GameScene.h"
#include "Assets.h"
#include "TitleScene.h"

class LoadScene:public GameScene
{
public:
	LoadScene(GameApplication *game):GameScene(game)
	{
	}
	~LoadScene()
	{
	}
	void update(float delta)
	{
          Assets::loadFiles();
          game->replaceScene(new TitleScene(game));
          return;
	}

	void render(float delta)
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
