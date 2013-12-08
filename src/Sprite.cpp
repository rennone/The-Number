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
  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST); //アルファテスト開始  
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  texture->bind();  
  numSprite=0;
  bufferIndex=0;
}

void SpriteBatcher::endBatch() const
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


void SpriteBatcher::drawTexture(const float &x, const float &y, const float &width,const float &height, const TextureRegion *region)
{
  float halfWidth  = width/2;
  float halfHeight = height/2;
  float x1 = x - halfWidth;
  float y1 = y - halfHeight;

  float x2 = x + halfWidth;
  float y2 = y + halfHeight;

  const float vertices[] =
  {
    x1, y1, region->u1, region->v2,
    x2, y1, region->u2, region->v2,
    x2, y2, region->u2, region->v1,
    x1, y2, region->u1, region->v1,
  };
  GLuint indices[] =
    {
      0,  1,  2,  3,  0,  2,
    };

  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  region->texture->bind();

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  
  glVertexPointer(3,  GL_FLOAT, 4*sizeof(float), vertices  );
  glTexCoordPointer(2,GL_FLOAT, 4*sizeof(float), vertices+2);  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);  
  glBindTexture(GL_TEXTURE_2D, 0);  
}

void SpriteBatcher::drawTexture
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

  const float vertices[] =
  {
    x1+x, y1+y, region->u1, region->v2,
    x2+x, y2+y, region->u2, region->v2,
    x3+x, y3+y, region->u2, region->v1,
    x4+x, y4+y, region->u1, region->v1,
  };
  
  GLuint indices[] =
    {
      0,  1,  2,  3,  0,  2,
    };

  glEnable(GL_BLEND);
  glEnable(GL_ALPHA_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_TEXTURE_2D);
  region->texture->bind();

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  
  glVertexPointer(3,  GL_FLOAT, 4*sizeof(float), vertices  );
  glTexCoordPointer(2,GL_FLOAT, 4*sizeof(float), vertices+2);  
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
  
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);  
  glBindTexture(GL_TEXTURE_2D, 0);  
}
