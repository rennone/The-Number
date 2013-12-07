#ifndef SCORE_SCENE_H_2013_12_05
#define SCORE_SCENE_H_2013_12_05

class ScoreScene:public GameScene
{
  float score[3];
public:
ScoreScene(GameApplication *game):GameScene(game)
  {
  }
  virtual void update(float delta);
  virtual void render(float delta);
  virtual void reshape(int width, int height);
  virtual void finish(void);
};
#endif
