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

  static TextureRegion *titleBackgroundRegion;

  static Texture *stringAtlus;
  static Texture *charAtlus;
  static TextureRegion *charRegion[16];
  static Texture *tapAnimation;
  static TextureRegion *tapAnimationRegion[16];
  
  static TextureRegion *targetRegion;
  static TextureRegion *numberRegions[9];

  static TextureRegion *stringPlayRegion;
  static TextureRegion *stringTitleRegion;
  static TextureRegion *stringScoreRegion;
  static TextureRegion *stringExitRegion;
  static void loadFiles();
  static void unloadFiles();

private:
  Assets();
  Assets& operator=(const Assets& obj);
};

#endif
