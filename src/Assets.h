#ifndef ASSETS_H
#define ASSETS_H

#include "Model.h"
#include "Texture.h"
#include "Sprite.h"

class Assets
{
public :
  static Texture *textureAtlas;
  static Texture *characterAtlas;
  static Texture *backgroundAtlas;
  
  static TextureRegion *titleBackgroundRegion;  
  static TextureRegion *fingerRegion[2];  
  static TextureRegion *charRegion[16];  
  static TextureRegion *tapAnimationRegion[8];
  static TextureRegion *numberRegions[10];
  
  static TextureRegion *stringPlayRegion;
  static TextureRegion *stringTitleRegion;
  static TextureRegion *stringScoreRegion;
  static TextureRegion *stringExitRegion;

  static TextureRegion *background;
  static TextureRegion *character[4];
  static TextureRegion *enemy[6];
  static TextureRegion *strings[4];
  static TextureRegion *player[5];
  static TextureRegion *explosion[7];

  static void loadFiles();
  static void unloadFiles();

private:
  Assets();
  Assets& operator=(const Assets& obj);
};

#endif
