#include "TitleScene.h"
#include "Camera.h"
#include "GLFW/glfw3.h"
#include "PlayScene.h"
#include "Debugger.h"
#include "Assets.h"
#include "GLutil.h"
void TitleScene::update(float delta)
{
  auto keyboard = game->Input()->Keyboard();
  if(keyboard->keyState(GLFW_KEY_A) == GLFW_PRESS){
    game->replaceScene(new PlayScene(game));
    return;
  }
}
        
void TitleScene::render(float delta)
{  
  glPushAttrib(GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
  glPushMatrix();

  int width, height;
  glfwGetFramebufferSize(game->Window(), &width, &height);
  
  Camera::getInstance()->set2DView(game->Window());
  glEnable(GL_TEXTURE_2D);

  glDrawTexture(width,height, Assets::titleBground->getTexId());
  
  glTranslatef(0,200, 0);  
  glDrawTexture(100, 50, Assets::titleString->getTexId());
  
  glTranslatef(0,-200, 0);
  glDrawTexture(100, 50, Assets::playString->getTexId());
  
  glTranslatef(0,-100, 0);
  glDrawTexture(100, 50, Assets::scoreString->getTexId());  
  
  glPopMatrix();
  glPopAttrib();

}
