#include "Sprite.h"
#include <GLFW/glfw3.h>
#include <math.h>

SpriteBatcher::SpriteBatcher(int _maxSprites)
{
  verticesBuffer = new float[_maxSprites*4*4]; //x,y,t,u * 4点
  indicesBuffer = new unsigned int[_maxSprites*6]; // 1面あたり6点
  bufferIndex = 0;
  numSprite = 0;

  int len = _maxSprites*6;
  for(int i=0, j=0; i<len; i+=6, j+=4)
  {
    indicesBuffer[i+0] = (j+0);
    indicesBuffer[i+1] = (j+1);
    indicesBuffer[i+2] = (j+2);
    indicesBuffer[i+3] = (j+2);
    indicesBuffer[i+4] = (j+3);
    indicesBuffer[i+5] = (j+0);
  }
}

void SpriteBatcher::beginBatch(Texture *texture)
{
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST); //アルファテスト開始
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  texture->bind();
  
  numSprite=0;
  bufferIndex=0;
}

void SpriteBatcher::endBatch()
{  
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3,  GL_FLOAT, 4*sizeof(float), verticesBuffer  );
  glTexCoordPointer(2,GL_FLOAT, 4*sizeof(float), verticesBuffer+2);
  
  glDrawElements(GL_TRIANGLES, 6*numSprite, GL_UNSIGNED_INT, indicesBuffer);

  glBindTexture(GL_TEXTURE_2D, 0);
}

void SpriteBatcher::drawSprite(const float &x, const float &y, const float &width,const float &height, const TextureRegion *region)
{
  float halfWidth  = width/2;
  float halfHeight = height/2;
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

void SpriteBatcher::drawSprite
(const float &x, const float &y, const float &width, const float &height, const float &angle, const TextureRegion *region)
{
  float halfWidth  = width/2;
  float halfHeight = height/2;
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
