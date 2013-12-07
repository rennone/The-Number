#include "Game.h"
#include "GameScene.h"
#include "Debugger.h"
#include "Input.h"
void GameApplication::initialize()
{
  window = glfwCreateWindow(640, 480, "example", glfwGetPrimaryMonitor(), NULL);
//  window = glfwCreateWindow(640, 480, "example", NULL, NULL);
  if(!window)
  {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, this);    

  input = InputManager::getInstance();
    
  elapsedTime[0] = glfwGetTime();
  elapsedTime[1] = glfwGetTime();
}

  /* ゲームループ */
void GameApplication::update(float delta)
{
  input->update(delta);
  scene->update(delta);
}

void GameApplication::render(float delta)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  scene->render(delta);
  Debugger::renderDebug(Window());// debug
  glfwSwapBuffers(window);
  glfwPollEvents();
}

  /* キーボードイベント */
void GameApplication::keyAction(int key, int action)
{
  Input()->Keyboard()->keyAction(key, action);
}

/* マウスイベント*/
void GameApplication::mouse(int button, int state, int x, int y)
{
  Input()->Mouse()->mouse(button, state, x, y);
}

void GameApplication::motion(int x, int y)
{
  input->Mouse()->motion(x,y);
}

void GameApplication::loop()
{
  float delta = elapsedTime[1] - elapsedTime[0]; //前フレームの経過時間
  elapsedTime[0] = glfwGetTime();
  update(delta);
  render(delta);
  elapsedTime[1] = glfwGetTime();

  checkScene();

  if(terminated)
    finish();
}

void GameApplication::finish()
{
  if(scene != NULL)
  {
    scene->finish();
    delete scene;
  }
  glfwSetWindowShouldClose(window, GL_TRUE);       
}

void GameApplication::terminate()
{
  terminated = true;
}

void inline GameApplication::replaceScene(GameScene *scene)
{
  nextScene = scene;

  if(this->scene == NULL)
    this->scene = nextScene;
}

void GameApplication::checkScene()
{
  if(scene != nextScene && scene != NULL)
  {
    scene->finish();
    delete scene;
    scene = nextScene;
  }
}

