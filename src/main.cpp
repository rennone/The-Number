#include <Leap.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <string>
#include <pthread.h>

#include "Game.h"
#include "LoadScene.h"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)  
    glfwSetWindowShouldClose(window, GL_TRUE);

  //デバッグ用 F1で強制終了
  if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    exit(2);
  
  ((GameApplication*)glfwGetWindowUserPointer(window))->keyAction(key, action);
}


int main(int argc, char *argv[]) {
  
  glfwSetErrorCallback(error_callback);
  
  if(!glfwInit())
  {
    exit(EXIT_FAILURE);
  }
  
  GameApplication* game = new GameApplication();
  game->initialize();  
  GLFWwindow* window = game->Window();
  
  glfwSetKeyCallback(window, key_callback);
  
  glClearColor(0.0, 0.0, 0.0, 1.0);  
  game->replaceScene(new LoadScene(game));
  
  while(!glfwWindowShouldClose(window))
  {
    game->loop();
  }
  
  glfwDestroyWindow(window);
  glfwTerminate();
  
  exit(EXIT_SUCCESS);
}
