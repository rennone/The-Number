#include <random>
#include <algorithm>
#include <GLFW/glfw3.h>
#include "Numbers.h"
#include "Assets.h"
#include "Camera.h"
#include "Sprite.h"

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
    numberSizes[i] = rand()%20+15;
    int subW = miniW - numberSizes[i]*2;
    int subH = miniH - numberSizes[i]*2;
    numbers[i].x =  numberSizes[i]/2 +      (i%3)*miniW + rand()%subW;
    numbers[i].y =  numberSizes[i]/2 + floor(i/3)*miniH + rand()%subH;
  }

  //シャッフル
  for(int i=0; i<8; i++)
  {
    int j = rand()%(9-i)+i;
    swap(numbers[i], numbers[j]);
    swap(numberSizes[i], numberSizes[j]);
  }
  target = 0;
}

void Numbers::update(float delta)
{
  
}

void Numbers::render()
{
  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
  glPushMatrix();  
  Camera::getInstance()->set2DView(game->Window());
  glEnable(GL_TEXTURE_2D);
  
  renderNumber();
  renderFinger();
  glPopMatrix();
  glPopAttrib();
}

void Numbers::renderNumber()
{
  spriteNumber->spriteBegin();
  for(int i=target; i<9; i++)
  {
    glPushMatrix();
    glTranslatef(numbers[i].x, numbers[i].y, 0);
    float t1 = (i%3)*0.33;
    float v1 = floor(i/3)*0.33;
    SpriteRegion region(t1, t1+0.33, v1, v1+0.33);
    spriteNumber->spriteDraw(&region, numberSizes[i], numberSizes[i]);
    glPopMatrix();
  }
  spriteNumber->spriteEnd();
}

void Numbers::renderFinger()
{
  auto leapMotion = game->Input()->LeapMotion();

  auto fingers = leapMotion->ScreenPoints();
  spriteFinger->spriteBegin();
  SpriteRegion region(0,1, 0,1);
  for( auto finger : fingers)
  {
    glPushMatrix();
    glTranslatef(finger.x, finger.y, 0);
    spriteFinger->spriteDraw(&region, 15, 15);
    glPopMatrix();
  }
spriteFinger->spriteEnd();

}

