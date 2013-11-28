#ifndef SPRITE_H_2013_11_28
#define SPRITE_H_2013_11_28

#include "Texture.h"

class SpriteRegion
{
public:  
  float left, right, top, bottom;
SpriteRegion(float l, float r, float t, float b)
  :left(l), right(r), top(t),bottom(b)
  {
  }
};

class Sprite
{
  Texture *sprite;
public:
Sprite(Texture *_sprite):sprite(_sprite)
  {
  }    
  void spriteBegin();
  void spriteDraw(const SpriteRegion *region, float width, float height);
  void spriteEnd(); 
};
#endif
