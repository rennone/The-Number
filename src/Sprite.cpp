#include "Sprite.h"
#include <GLFW/glfw3.h>
#include <math.h>

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



SpriteBacher::SpriteBacher(int _maxSprites)
{
  verticesBuffer = new float[_maxSprites*4*4];
  indicesBuffer = new short[_maxSprites*6];
  bufferIndex = 0;
  numSprite = 0;

  int len = _maxSprites*6;
  for(int i=0, j=0; i<len; i+=6, j+=4)
  {
    indicesBuffer[i+0] = (short)(j+0);
    indicesBuffer[i+1] = (short)(j+1);
    indicesBuffer[i+2] = (short)(j+2);
    indicesBuffer[i+3] = (short)(j+2);
    indicesBuffer[i+4] = (short)(j+3);
    indicesBuffer[i+5] = (short)(j+0);
  }
}

void SpriteBacher::beginBatch()
{
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);//アルファテスト開始
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  
  glBindTexture(GL_TEXTURE_2D, texture->getTexId());
}

void SpriteBacher::endBatch()
{  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3,  GL_FLOAT, vertexSize*sizeof(float), vertices  );
  glTexCoordPointer(2,GL_FLOAT, vertexSize*sizeof(float), vertices+3);
  
  glDrawElements(GL_TRIANGLES, 6*numSprite, GL_FLOAT, indices);

  glBindTexture(GL_TEXTURE_2D, 0);
  
}

void SpriteBacher::drawSprite(const float &x, const float &y, const float &width,const float &height, const TextureRegion *region)
{
  float halfWidth  = width/2;
  float haldHeight = height/2;
  float x1 = x - halfWidth;
  float y1 = y - halfHeight;

  float x2 = x + halfWidth;
  float y2 = y + halfHeight;

  verticesBuffer[bufferIndex++] = x1;
  verticesBuffer[bufferIndex++] = y1;
  verticesBuffer[bufferIndex++] = region->u1;
  verticesBuffer[bufferIndex++] = region->v2;

  verticesBuffer[bufferIndex++] = x2;
  verticesBuffer[bufferIndex++] = y1;
  verticesBuffer[bufferIndex++] = region->u2;
  verticesBuffer[bufferIndex++] = region->v2;

  verticesBuffer[bufferIndex++] = x2;
  verticesBuffer[bufferIndex++] = y2;
  verticesBuffer[bufferIndex++] = region->u2;
  verticesBuffer[bufferIndex++] = region->v1;

  verticesBuffer[bufferIndex++] = x1;
  verticesBuffer[bufferIndex++] = y2;
  verticesBuffer[bufferIndex++] = region->u1;
  verticesBuffer[bufferIndex++] = region->v1;

  numSprite++;
}

void SpriteBacher::drawSprite
(const float &x, const float &y, const float &width, const float &height, const float &angle, const TextureRegion *region)
{
  float halfWidth  = width/2;
  float haldHeight = height/2;
  float rad = angle * M_PI / 180.0;
  float _cos = cos(rad);
  float _sin = sin(rad);

  float x1 = -halfWidth*_cos - (-halfHeight)*_sin;
  float y1 = -halfWidth*_sin + (-halfHeight)*_cos;
  
  float x2 = halfWidth*_cos - (-halfHeight)*_sin;
  float y2 = halfWidth*_sin + (-halfHeight)*_cos;

  float x3 = halfWidth*_cos - (halfHeight)*_sin;
  float y3 = halfWidth*_sin + (halfHeight)*_cos;

  float x4 = -halfWidth*_cos - (halfHeight)*_sin;
  float y4 = -halfWidth*_sin + (halfHeight)*_cos;


  verticesBuffer[bufferIndex++] = x1 + x;
  verticesBuffer[bufferIndex++] = y1 + y;
  verticesBuffer[bufferIndex++] = region->u1;
  verticesBuffer[bufferIndex++] = region->v2;

  verticesBuffer[bufferIndex++] = x2 + x;
  verticesBuffer[bufferIndex++] = y2 + y;
  verticesBuffer[bufferIndex++] = region->u2;
  verticesBuffer[bufferIndex++] = region->v2;

  verticesBuffer[bufferIndex++] = x3 + x;
  verticesBuffer[bufferIndex++] = y3 + y;
  verticesBuffer[bufferIndex++] = region->u2;
  verticesBuffer[bufferIndex++] = region->v1;

  verticesBuffer[bufferIndex++] = x4 + x;
  verticesBuffer[bufferIndex++] = y4 + y;
  verticesBuffer[bufferIndex++] = region->u1;
  verticesBuffer[bufferIndex++] = region->v1;

  numSprite++;
}
