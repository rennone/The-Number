#ifndef ASSETS_H
#define ASSETS_H

#include "Model.h"
#include "Texture.h"
#include "Sprite.h"

class Assets
{
public :
  static Texture *titleBground;
  static Texture *titleString;
  static Texture *playString;
  static Texture *scoreString;
  static Texture *target;
  static Texture *numbers;
  static void loadFiles();
private:
  Assets();
  Assets& operator=(const Assets& obj);
};

#endif
