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
  glMatrixMode(GL_PROJECTION); //���e
  glLoadIdentity();
  
  //����p, �A�X�y�N�g��, near����, far����
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
  ratio = width / (float) height;
  glViewport(0, 0, width, height);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, height, 0, -10, 10);
  
  glMatrixMode(GL_MODELVIEW);

  glPopAttrib();
}

Leap::Vector Camera::screenToWorldVector(GLFWwindow* window, float x, float y)
{
  int width, height;           
  float ratio;
  glfwGetFramebufferSize(window, &width, &height);
  ratio = width / (float) height;
  
  float screenX = x/width - 0.5;
  float screenY = 0.5 - y/height;  
  float nearHeight = near*tan(M_PI*fovy/180.0);
  float nearWidth  = nearHeight*ratio;
  float farHeight  = far*tan(M_PI*fovy/180.0);
  float farWidth   = farHeight*ratio;
  
  auto cDirection = (look - position).normalized();  //�J�����̕����x�N�g��
  
  Leap::Vector baseDirection;
  Leap::Vector nearVec, farVec;
  if( cDirection.z < 0)
  {
     //�J������-z�̕��ʂ������Ă�Ƃ���, ��x�N�g����v(0,0,-1)���g��
    baseDirection = Leap::Vector(0,0,-1);
    nearVec = Leap::Vector( screenX*nearWidth, screenY*nearHeight, -near);
    farVec  = Leap::Vector( screenX*farWidth , screenY*farHeight , -far);
  } else
  {
    //�J������+z�̕��ʂ������Ă�Ƃ���, ��x�N�g����v(0,0,+1)���g��
    baseDirection = Leap::Vector(0,0,+1);
    //�E��n������, +z�̕��������ƉE�����ɂȂ�
    nearVec = Leap::Vector( -screenX*nearWidth, screenY*nearHeight, near);
    farVec  = Leap::Vector( -screenX*farWidth , screenY*farHeight , far);
  }

  //�J���������ۂɌ����Ă�������ɂ��킹�ĉ�]������
  auto rad = baseDirection.angleTo(cDirection);    //��]�p�x  
  auto A = baseDirection.cross(cDirection).normalized();        //��]�����v�Z

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
