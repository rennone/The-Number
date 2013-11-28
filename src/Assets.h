#ifndef ASSETS_H
#define ASSETS_H

#include "Model.h"
#include "Texture.h"
#include "Sprite.h"

class Assets
{
public :
  static Texture *shadow;
  static Texture *target;
  static Texture *numbers;
  static void loadFiles();
};

#endif
