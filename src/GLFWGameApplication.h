#ifndef GLFW_GAME_APPLICATION_H_2013_11_19
#define GLFW_GAME_APPLICATION_H_2013_11_19

#include "Game.h"
#include <sstream>
#include <map>
#include <Leap.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include "Input.h"
#include "GameScene.h"
#include "Camera.h"

class GameScene;

class GLFWGameApplication : public GameApplication
{
  GameScene *scene;
  InputManager *input;
  Camera *camera;
  double elapsedTime[2];
  float delta;
  GLFWwindow *window;

  //degub情報のハッシュ ( ファイル名 タグ 内容)
  std::map< std::string, std::map<std::string, std::string>> debugInfo;
public:
  GameApplication()
  {
    initialize(NULL);
  };

  GameApplication(GameScene *scene)
  {
    initialize(scene);
  }

  virtual void replaceScene(GameScene *scene)
  {
    if(this->scene != NULL)
      delete this->scene;
    this->scene = scene;
  };

  /* キーボードイベント */
  virtual void keyAction(int key, int action)
  {
    Input()->Keyboad()->keyAction(key, action);
  }

  /* マウスイベント*/
  virtual void mouse(int button, int state, int x, int y)
  {
    Input()->Mouse()->mouse(button, state, x, y);
  }

  virtual void motion(int x, int y)
  {
    input->Mouse()->motion(x,y);
  }

  /* ゲームループ */
  virtual void update(float delta)
  {
    input->update(delta);
    scene->update(delta);          
    if(delta != 0)
      drawDebugInfo("Game.h", "fps", 1.0/delta);
  }

  virtual void render(float delta)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->render(delta);

    renderDebug();
    glfwSwapBuffers(window);
    glfwPollEvents();

  }

  virtual void loop()
  {
  
    float delta = elapsedTime[1] - elapsedTime[0]; //前フレームの経過時間
    elapsedTime[0] = glfwGetTime();
    update(delta);
    render(delta);
    elapsedTime[1] = glfwGetTime();
    //debug情報のクリア 基本的に毎回上書きするから必要ない?
    /*
     */
  }


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

  virtual inline void setWindow(GLFWwindow* window)
  {
    this->window = window;
  }

  //----------------------debug--------------------//
  //drawしたファイルの名前, タグ, 内容
  template<typename T> void drawDebugInfo(std::string place, std::string tag, T value)
  {
    std::stringstream ss;
    ss << value;
    debugInfo[place][tag] = ss.str();
  }

private:        
  virtual void initialize(GameScene *scene)
  {
    this->scene = scene;
    input = InputManager::getInstance();
    camera = Camera::getInstance();

    window = glfwCreateWindow(640, 480, "example", glfwGetPrimaryMonitor(), NULL);                
    if(!window)
    {
      glfwTerminate();
      exit(EXIT_FAILURE);
    }
                
    elapsedTime[0] = glfwGetTime();
    elapsedTime[1] = glfwGetTime();
  }

  //--------------------debug--------------------
  virtual void renderDebug()
  {
    camera->set2DView(Window());
    glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);          
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1, 1, 1);
    float x=0, y=10.3, dy=10;
		  
    for( auto it = debugInfo.begin(); it != debugInfo.end(); it++)
    {
			  
      //情報を書いた場所を記述
      renderString((*it).first, x, y);
      y+=dy;            
      //デバッグ情報の記述
      auto localDebugInfo = (*it).second;
			
      for( auto it2 = localDebugInfo.begin(); it2 != localDebugInfo.end(); it2++, y+=dy)              
        renderString((*it2).first + " : " + (*it2).second, x, y);
			  
    }          
    glPopAttrib();
    glPopMatrix();
  }

  virtual inline void renderString(std::string str, float x, float y)
  {
    glRasterPos2d(x,y);
    for(unsigned int i=0; i<str.size(); i++)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, str[i]);
  }

  virtual inline void clearDegubInfo()
  {
    for(auto it=debugInfo.begin(); it!=debugInfo.end(); it++)
    {
      auto local = (*it).second;
      local.clear();
    }
  }

};
#endif
