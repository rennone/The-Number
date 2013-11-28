#define _USE_MATH_DEFINES
#include <cmath>

#include <string>
#include <math.h>
#include <GLFW/glfw3.h>
#include "PlayScene.h"
#include "Camera.h"
#include "Numbers.h"
#include "GLutil.h"

PlayScene::PlayScene(GameApplication *game):GameScene(game)
{
  initialize();
}

PlayScene::~PlayScene()
{
}

void PlayScene::render(float delta)
{
  
//  Camera::getInstance()->set3DView(game->Window());
  //glLoadIdentity();
  /*
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);  
  glEnable(GL_TEXTURE_2D);  
  GLfloat red[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat lightpos[] = { 200.0, 400.0, -300.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
  */
  root->render();  
}

void PlayScene::update(float delta)
{
    static float theta = 0, phi=0;
    auto keyboard = game->Input()->Keyboard();
    
    if( keyboard->keyState(GLFW_KEY_RIGHT) != GLFW_RELEASE)
    {
      theta+=1;   if(theta > 360) theta-=360;
    } else if(keyboard->keyState(GLFW_KEY_LEFT) != GLFW_RELEASE)
    {
      theta-=1;      if(theta < 0) theta+=360;
    }
    
    if(keyboard->keyState(GLFW_KEY_UP) != GLFW_RELEASE)
    {
      phi += 1;      if(phi>360) phi-=360;
    } else if(keyboard->keyState(GLFW_KEY_DOWN) != GLFW_RELEASE)
    {
      phi -= 1;      if(phi<0) phi+=360;
    }
    const float R = 500;
    Camera::getInstance()->Position(Leap::Vector(R*cos(phi*M_PI/180.0)*sin(theta*M_PI/180.0),
                                                 R*sin(phi*M_PI/180.0),
                                                 R*cos(phi*M_PI/180.0)*cos(theta*M_PI/180.0)  )  );
    Camera::getInstance()->Look(Leap::Vector());

    root->update(delta);
    root->checkStatus();
}

void PlayScene::reshape(int width, int height)
{
}

void PlayScene::initialize()
{
  root = new Actor("root", game);
  root->addChild(new Numbers("numbers", game));
}
