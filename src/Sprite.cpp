#include "Sprite.h"
#include <GLFW/glfw3.h>

void Sprite::spriteBegin()
{
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, sprite->getTexId());

  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);//アルファテスト開始
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Sprite::spriteDraw(const SpriteRegion *region, float width, float height)
{    
  float vertices[] =
  {
    -width , -height, 0,   region->left, region->top,
    -width , +height, 0,   region->left, region->bottom,
    +width , +height, 0,   region->right, region->bottom,
    +width , -height, 0,   region->right, region->top,
  };
  
  GLuint indices[] =
    {
      0,  1,  2,  3,  0,  2,
    };
  
  int vertexSize = 5;
  
  glVertexPointer(3,  GL_FLOAT, vertexSize*sizeof(float), vertices  );
  glTexCoordPointer(2,GL_FLOAT, vertexSize*sizeof(float), vertices+3);  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
  
}

void Sprite::spriteEnd()
{  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);
  glBindTexture(GL_TEXTURE_2D,0);
  glDisable(GL_TEXTURE_2D);
  
}
