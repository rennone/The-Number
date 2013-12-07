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
TextureRegion *Assets::player;
TextureRegion *Assets::background;
TextureRegion *Assets::character[4];

TextureRegion* Assets::CutTextureWithCellIndex(Texture* texture, float cellSize, int left, int top, int width, int height)
{
  return new TextureRegion(texture, left*cellSize, top*cellSize, width*cellSize, height*cellSize);
}
void Assets::loadFiles()
{
  textureAtlas = new PngTexture("../resources/textureAtlas.png");
  int w  = textureAtlas->Width();
  int h  = textureAtlas->Height();;
  int row=8;
  int col=8;
  float pSize = 1.0*w/row;

  auto cutTexture
    = []( Texture* texture,int cellSize, int left, int top, int width, int height)
    {
      return new TextureRegion(texture, left*cellSize, top*cellSize, width*cellSize, height*cellSize);
    };
  

  int k=0;
  int num=10;
  float t1, v1;
  for(int i=k; i<k+num; i++)
  {
    t1 = (i%row)*pSize;
    v1 = floor(i/col)*pSize;
    numberRegions[i] = new TextureRegion(textureAtlas, t1, v1, pSize, pSize);
  }
  k+=num;
  
  num=11;  
  for(int i=k, j=0; i<k+num; i++, j++)
  {
    t1 = (i%row)*pSize;
    v1 = floor(i/col)*pSize;
    charRegion[j] =  new TextureRegion(textureAtlas, t1, v1, pSize, pSize);
  }  
  k+=num+1;
  
  num=2;
  for(int i=22, j=0; i<22+num;i++, j++)
  {
    t1 = (i%row)*pSize;
    v1 = floor(i/col)*pSize;
    fingerRegion[j] = cutTexture(textureAtlas, pSize, (i%row), floor(i/col), 1, 1);
  }
  
  stringTitleRegion = cutTexture(textureAtlas, pSize, 0, 3, 3, 1);  
  stringPlayRegion  = cutTexture(textureAtlas, pSize, 3, 3, 2, 1);
  stringScoreRegion = cutTexture(textureAtlas, pSize, 5, 3, 2, 1);
  stringExitRegion  = cutTexture(textureAtlas, pSize, 0, 4, 2, 1);

  for(int i=34, j=0; i<34+8; i++, j++)
    tapAnimationRegion[j] = cutTexture(textureAtlas, pSize, (i%row), floor(i/col), 1, 1);

  player = cutTexture(textureAtlas, pSize, 2, 5, 1, 2);

  characterAtlas = new PngTexture("../resources/character.png");
  character[0] = cutTexture(characterAtlas, 100, 0, 0, 1, 1);
  character[1] = cutTexture(characterAtlas, 100, 1, 0, 1, 1);
  character[2] = cutTexture(characterAtlas, 100, 0, 1, 1, 1);
  character[3] = cutTexture(characterAtlas, 100, 1, 1, 1, 1);
  
  backgroundAtlas = new PngTexture("../resources/bgd.png");
  titleBackgroundRegion = new TextureRegion(backgroundAtlas, 0,0, 512, 512);
  background = new TextureRegion(backgroundAtlas, 0,0, 512, 512);

}

void Assets::unloadFiles()
{
}
