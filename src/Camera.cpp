#define _USE_MATH_DEFINES
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
#include "Camera.h"

void Camera::set3DView(GLFWwindow* window)
{  
  float ratio;
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  ratio = width / (float) height;
  glViewport(0, 0, width, height);

  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION); //投影
  glLoadIdentity();
  
  //視野角, アスペクト比, near平面, far平面
  gluPerspective(fovy, ratio, near, far);  
  gluLookAt(position.x, position.y, position.z, look.x, look.y, look.z, 0.0, 1.0, 0.0);
  glMatrixMode(GL_MODELVIEW);  
}

void Camera::set2DView(GLFWwindow* window)
{
  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT);
  
  glDisable(GL_DEPTH_TEST);  
  float ratio;
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-width/2, width/2, -height/2, height/2, -10, 10);
  
  glMatrixMode(GL_MODELVIEW);

  glPopAttrib();
}

Leap::Vector Camera::screenToWorldVector(GLFWwindow* window, float x, float y)
{
  int width, height;           
  float ratio;
  glfwGetFramebufferSize(window, &width, &height);
  ratio = width / (float) height;
  
  float screenX = x/width;
  float screenY = y/height;  
  float nearHeight = near*tan(M_PI*fovy/180.0);
  float nearWidth  = nearHeight*ratio;
  float farHeight  = far*tan(M_PI*fovy/180.0);
  float farWidth   = farHeight*ratio;
  
  auto cDirection = (look - position).normalized();  //カメラの方向ベクトル
  
  Leap::Vector baseDirection;
  Leap::Vector nearVec, farVec;
  if( cDirection.z < 0)
  {
     //カメラが-zの方面を向いてるときは, 基準ベクトルはv(0,0,-1)を使う
    baseDirection = Leap::Vector(0,0,-1);
    nearVec = Leap::Vector( screenX*nearWidth, screenY*nearHeight, -near);
    farVec  = Leap::Vector( screenX*farWidth , screenY*farHeight , -far);
  } else
  {
    //カメラが+zの方面を向いてるときは, 基準ベクトルはv(0,0,+1)を使う
    baseDirection = Leap::Vector(0,0,+1);
    //右手系だから, +zの方を向くと右が負になる
    nearVec = Leap::Vector( -screenX*nearWidth, screenY*nearHeight, near);
    farVec  = Leap::Vector( -screenX*farWidth , screenY*farHeight , far);
  }

  //カメラが実際に向いている方向にあわせて回転させる
  auto rad = baseDirection.angleTo(cDirection);    //回転角度  
  auto A = baseDirection.cross(cDirection).normalized();        //回転軸を計算

  auto P = (farVec-nearVec).normalized();
  auto Q = A.cross(P);
  auto projA_P = A.dot(P)*A;
  auto perpA_P = P - A.dot(P)*A;
  auto direction = perpA_P*cos(rad) + Q*sin(rad) + projA_P;
  
  return direction.normalized();
}

void Camera::update(float delta)
{
  if(!hasTarget) return;  
/*
  setLook(target->getPosition());

  float x = radius*cos(phi)*sin(theta);
  float y = radius*sin(phi);
  float z = radius*cos(phi)*cos(theta);

  setPosition(Leap::Vector(x,y,z)+target->getPosition());
*/
}
