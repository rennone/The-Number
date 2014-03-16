#ifndef PLAYSCENERENDERER_H_20131102_201401_
#define PLAYSCENERENDERER_H_20131102_201401_

#include <Leap.h>
#include "GameScene.h"
#include "Actor.h"
#include "Sprite.h"

class PlayScene : public GameScene
{
public:
  PlayScene(GameApplication *game);
  ~PlayScene();
  virtual void update(float delta);
  virtual void render(float delta);
  virtual void reshape(int width, int height);
  virtual void initialize();

  virtual void finish()
  {
    root->Status(Actor::Dead);
    root->checkStatus();
  }
  
  Actor* Root() const
  {
    return root;
  }

  void fade(const float &alpha) const;

private:
  Actor *root;
  SpriteBatcher *batcher;
  float elapsedTime;
};



#endif	// _PLAYSCENERENDERER_H_20131102_201401_
