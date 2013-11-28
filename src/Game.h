#ifndef GAME_H_2013_11_19
#define GAME_H_2013_11_19

#include <sstream>
#include <map>
#include <Leap.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include "Input.h"

//インクルードが循環する場合は書かないとエラーになる
class GameScene;

class GameApplication
{
private:
  GameScene *scene;
  InputManager *input;
  double elapsedTime[2];
  float delta;
  GLFWwindow *window;  
public:
  GameApplication()
  {
    scene = NULL;
  };

  virtual inline void initialize();
  virtual void replaceScene(GameScene *scene);
  virtual inline void keyAction(int key,int action);
  virtual inline void mouse(int buton, int state, int x, int y);
  virtual inline void motion(int x, int y);
  virtual inline void loop();
  
  //------------------ゲッター--------------------//
  virtual inline InputManager* Input()
  {
    return input;
  }

  virtual inline GameScene* Scene()
  {
    return scene;
  }
        
  virtual inline GLFWwindow* Window()
  {          
    return window;
  }
  
private:
  virtual void update(float delta);  /* ゲームループ */
  virtual void render(float delta);
};

#endif
