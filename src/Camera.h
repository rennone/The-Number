#ifndef CAMERA_H_2013_11_19
#define CAMERA_H_2013_11_19

#include <Leap.h>
#include <GLFW/glfw3.h>

class Camera
{
public:
  static Camera* getInstance()
  {
    static Camera instance;
    return &instance;
  }
  
  void Position(Leap::Vector _position)
  {
    position = _position;
  }

  void Position(float x, float y, float z)
  {
    position = Leap::Vector(x,y,z);
  }

  Leap::Vector Position()
  {
    return position;
  }

  void Look(Leap::Vector _look)
  {
    look = _look;
  }

  void Look(float x, float y, float z)
  {
    look = Leap::Vector(x,y,z);
  }

  Leap::Vector Look()
  {
    return look;
  }

  void setTheta(float _theta)
  {
    theta = _theta;
  }

  float getTheta()
  {
    return theta;
  }

  void setPhi(float _phi)
  {
    phi = _phi;
  }

  float getPhi()
  {
    return phi;
  }

  void setRadius(float _radius)
  {
    radius = _radius;
  }

  float getRadius()
  {
    return radius;
  }

  void deleteTarget()
  {
    hasTarget = false;
//    target = NULL;
  }
/*        
  void setTarget(Actor* _target)
  {
    target = _target;
    hasTarget = true;
  }

  Actor* getTarget()
  {
    return target;
  }
*/      
  void set3DView(GLFWwindow* window);
  void set2DView(GLFWwindow* window);        
  void update(float delta);
  Leap::Vector screenToWorldVector(GLFWwindow* window, float x, float y);
  
private:
  Camera()
  {
    fovy     = 30;        
    near     = 1.0;
    far      = 3000.0;
    //target = NULL;
    hasTarget = false;

    theta = 0;
    phi = 0;
    radius = 500;

    position = Leap::Vector(0, 0, -radius);
    look     = Leap::Vector(0,0,0);
  }        
  ~Camera(){}
  bool hasTarget;
//  Actor* target;   //observation target
  Leap::Vector position;
  Leap::Vector look;
  int fovy;	//Ž‹–ì
  float near, far;
        
  float radius, theta, phi; //Šp“x
};

#endif
