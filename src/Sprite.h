#ifndef SPRITE_H_2013_11_28
#define SPRITE_H_2013_11_28

#include "Texture.h"

class SpriteBatcher
{
  float *verticesBuffer;
  unsigned int *indicesBuffer;
  int bufferIndex;
  int numSprite;
public:
  SpriteBatcher(int _maxSprites);
  void beginBatch(Texture *texture);
  void drawSprite(const float &x, const float &y, const float &width, const float &height, const TextureRegion *region);
  void drawSprite(const float &x, const float &y, const float &width, const float &height, const float &angle, const TextureRegion *region);
  void endBatch() const;

  static void drawTexture(const float &x, const float &y, const float &width, const float &height, const TextureRegion *region);
  static void drawTexture(const float &x, const float &y, const float &width, const float &height, const float &angle, const TextureRegion *region);
};

#endif
