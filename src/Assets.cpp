#include "Assets.h"
#include <math.h>
#include "Debugger.h"

Texture* Assets::textureAtlas = NULL;
Texture* Assets::backgroundAtlas = NULL;
Texture* Assets::characterAtlas = NULL;

TextureRegion *Assets::fingerRegion[2];
TextureRegion *Assets::numberRegions[10]; //TextureRegion* の配列(ポインタではない)
TextureRegion *Assets::titleBackgroundRegion;
TextureRegion *Assets::charRegion[16];
TextureRegion *Assets::tapAnimationRegion[8];
TextureRegion *Assets::stringExitRegion;
TextureRegion *Assets::stringPlayRegion;
TextureRegion *Assets::stringTitleRegion;
TextureRegion *Assets::stringScoreRegion;
TextureRegion *Assets::player[5];
TextureRegion *Assets::strings[4];
TextureRegion *Assets::enemy[6];
TextureRegion *Assets::explosion[7];
TextureRegion *Assets::background;
TextureRegion *Assets::character[4];

void Assets::loadFiles()
{
  textureAtlas = new PngTexture("../resources/textureAtlas.png");
  int w  = textureAtlas->Width();
  int h  = textureAtlas->Height();;
  int row=16;
  int col=16;
  float pSize = 1.0*w/row;

  auto cutTexture
    = []( Texture* texture,int cellSize, int left, int top, int width, int height)
    {
      return new TextureRegion(texture, left*cellSize, top*cellSize, width*cellSize, height*cellSize);
    };  

  for(int i=0; i<11; i++)
    charRegion[i] = cutTexture(textureAtlas, pSize, i, 0, 1, 1);

  for(int i=0; i<2; i++)
    fingerRegion[i] = cutTexture(textureAtlas, pSize, i+11, 0, 1, 1);
  
  for(int i=0; i<7; i++)
    explosion[i] = cutTexture(textureAtlas, pSize, i, 2, 1, 1);
  
  for(int i=0; i<5; i++)
    player[i] = cutTexture(textureAtlas, pSize, i, 3, 1, 1);

  for(int i=0; i<6; i++)
    enemy[i] = cutTexture(textureAtlas, pSize, i, 4, 1, 1);

  stringTitleRegion = cutTexture(textureAtlas, pSize, 0, 1, 3, 1);  
  stringPlayRegion  = cutTexture(textureAtlas, pSize, 3, 1, 2, 1);
  stringScoreRegion = cutTexture(textureAtlas, pSize, 5, 1, 2, 1);
  stringExitRegion  = cutTexture(textureAtlas, pSize, 7, 1, 2, 1);
  
  titleBackgroundRegion = cutTexture(textureAtlas, pSize, 9, 1, 7, 5);
  background = cutTexture(textureAtlas, pSize, 8, 7, 8, 4);
}

void Assets::unloadFiles()
{
}
