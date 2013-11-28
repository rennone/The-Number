#ifndef APPLICATION_H_2013_11_19
#define APPLICATION_H_2013_11_19

#include <GLFW/glfw3.h>
#include "input.h"

class Application
{
  virtual void keyAction(int key, int action) = 0;
  virtual void mouse(int button, int state, int x, int y)=0;
  virtual void motion(int x, int y)=0;
  virtual void loop()=0;
  virutal InputManager* Input()=0;
  virtual GameScene* Scene()=0;
  virtual GLFWwindow* Window()=0;
}

#endif
