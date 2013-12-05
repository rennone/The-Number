#include <random>
#include <algorithm>
#include <GLFW/glfw3.h>
#include "Numbers.h"
#include "Assets.h"
#include "Camera.h"
#include "Sprite.h"
#include "Debugger.h"
#include "TitleScene.h"

void Numbers::reset()
{ 
  auto window = game->Window();
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  int x[9], y[9];
  int miniW = width/3;
  int miniH = height/3;
  
  for(int i=0; i<9; i++)
  {
    numberSizes[i] = rand()%30+50;
    int subW = miniW - numberSizes[i]*2;
    int subH = miniH - numberSizes[i]*2;
    numbers[i].x =  numberSizes[i]/2 +      (i%3)*miniW + rand()%subW - width/2;
    numbers[i].y =  numberSizes[i]/2 + floor(i/3)*miniH + rand()%subH - height/2;
  }
  //シャッフル
  for(int i=0; i<8; i++)
  {
    int j = rand()%(9-i)+i;
    swap(numbers[i], numbers[j]);
    swap(numberSizes[i], numberSizes[j]);
  }
  target = 0;
  gesture = Numbers::Tap;
  elapsedTime = 0;
}


void Numbers::checkTouch()
{
  auto leapMotion = game->Input()->LeapMotion();
  auto screen = leapMotion->getController()->locatedScreens()[0];
  auto pointables = leapMotion->Frame().pointables();
  
  static int flag = true;
  
  for(auto pointable: pointables)
  {
    auto normal =  screen.intersect(pointable.tipPosition(),pointable.direction(), true);    
    float x     = (normal.x-0.5) * screen.widthPixels();
    float y     = (normal.y-0.5) * screen.heightPixels();
    const float border = 0.0;
    Debugger::drawDebugInfo("Numbers.cpp", "distance", pointable.touchDistance());
    if(pointable.touchDistance() > border && pointable.touchZone() != Leap::Pointable::Zone::ZONE_NONE)
    {
      flag = true;
      Debugger::drawDebugInfo("Numbers.cpp", "state", "hover");          // ホバー状態
    }
    else if(pointable.touchDistance() <= border)
    {
      if(flag)
      {
        flag = false;
        Debugger::drawDebugInfo("Numbers.cpp", "state", "touch");
        gestures[gesture]->add(x,y);
        if(numbers[target].x-numberSizes[target]/2 <= x &&  x <= numbers[target].x + numberSizes[target]/2 &&
           numbers[target].y-numberSizes[target]/2 <= y &&  y <= numbers[target].y + numberSizes[target]/2
          )
        {
          target+=1;
        }
      }
    }
    else
    {
      flag = true;
      Debugger::drawDebugInfo("Numbers.cpp", "state", "none");
    }
  }
}

void Numbers::checkCircle()
{
  static int flag = true;
  
  const auto leapMotion = game->Input()->LeapMotion();
  const auto gests = leapMotion->Frame().gestures();
  const auto screen = leapMotion->getController()->locatedScreens()[0];

  if(gests.isEmpty())
    gestures[gesture]->reset();
  for( auto ges : gests)
  {
    if(ges.type() != Leap::Gesture::TYPE_CIRCLE)
    {
      gestures[gesture]->reset();
      continue;
    }

    //Debugger::drawDebugInfo("Number.cpp", "gesture", "circle");
    Leap::CircleGesture circle = ges;

    if(circle.state() == Leap::Gesture::STATE_UPDATE)
    {
      Debugger::drawDebugInfo("Number.cpp", "gesture", "circle");
      auto point = screen.intersect(circle.pointable(), true);
      point.x = (point.x-0.5)*screen.widthPixels();
      point.y = (point.y-0.5)*screen.heightPixels();
      gestures[gesture]->add(point.x,point.y);
    }
    else
    {
      gestures[gesture]->reset();
    }
  }
}

void Numbers::update(float delta)
{
  elapsedTime += delta;
  
  if(target>=9)
    game->replaceScene(new TitleScene(game));

  Debugger::drawDebugInfo("numbers.cpp", "gesture", gesture);
  switch(gesture)
  {
  case Numbers::Tap:
    checkTouch();
    break;
  case Numbers::Circle:
    checkCircle();
    break;
  default:
    break;       
  }
  gestures[gesture]->update(delta);
}

void Numbers::render()
{
  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
  glPushMatrix();  
  Camera::getInstance()->set2DView(game->Window());
  glEnable(GL_TEXTURE_2D);
  
  renderNumber();
  renderFinger();
  renderTime();
  gestures[gesture]->render();
  glPopMatrix();
  glPopAttrib();
}

void Numbers::renderTime()
{
  int timer[6];
  int upperPoint = floor(elapsedTime); //小数点以上
  int lowerPoint = ((int)(elapsedTime*100))%100; //小数点以下2桁
  
  timer[0] = ((int)(elapsedTime*100))%10;
  timer[1] = ((int)(elapsedTime*10 ))%10;
  timer[2] = 10; //小数点
  timer[3] = ((int)(elapsedTime    ))%10;
  timer[4] = ((int)(elapsedTime/10 ))%10;
  timer[5] = ((int)(elapsedTime/100))%10;

  int width, height;
  glfwGetFramebufferSize(game->Window(), &width, &height);

  const int wSize = 30;
  const int hSize = 30;
  const int x = width/2 - wSize/2;
  const int y = height/2- hSize/2;
  batcher->beginBatch(Assets::charAtlus);
  for(int i=0; i<6; i++)
    batcher->drawSprite(x-i*wSize, y, wSize, hSize, Assets::charRegion[timer[i]]);
  batcher->endBatch();
}

void Numbers::renderNumber()
{
  batcher->beginBatch(Assets::numbers);
  for(int i=target; i<9; i++)
  {
    float t1 = (i%3)*0.33;
    float v1 = floor(i/3)*0.33;
    batcher->drawSprite(numbers[i].x, numbers[i].y, numberSizes[i], numberSizes[i], Assets::numberRegions[i]);
  }
  batcher->endBatch();
}

void Numbers::renderFinger()
{
  auto leapMotion = game->Input()->LeapMotion();
  auto fingers = leapMotion->ScreenPoints();

  batcher->beginBatch(Assets::target);
  for( auto finger : fingers)
  {
    batcher->drawSprite(finger.x, finger.y, 15, 15, Assets::targetRegion);
  }
  batcher->endBatch();
}

