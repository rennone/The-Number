#include "Assets.h"
#include <math.h>
#include "Debugger.h"

Texture* Assets::target = NULL;
Texture* Assets::numbers = NULL;
Texture* Assets::titleBground = NULL;
Texture* Assets::titleString = NULL;
Texture* Assets::playString = NULL;
Texture* Assets::scoreString = NULL;


TextureRegion *Assets::titleBackgroundRegion;

Texture *Assets::charAtlus = NULL;
TextureRegion *Assets::charRegion[16];

Texture* Assets::tapAnimation = NULL;
TextureRegion *Assets::tapAnimationRegion[16];

Texture *Assets::stringAtlus = NULL;
TextureRegion *Assets::stringExitRegion;
TextureRegion *Assets::stringPlayRegion;
TextureRegion *Assets::stringTitleRegion;
TextureRegion *Assets::stringScoreRegion;

TextureRegion *Assets::targetRegion;
TextureRegion *Assets::numberRegions[9]; //TextureRegion* の配列(ポインタではない)

void Assets::loadFiles()
{
  titleBground = new PngTexture("../resources/background.png");
  titleBackgroundRegion = new TextureRegion(titleBground, 0, 0, titleBground->getWidth(), titleBground->getHeight());
  
  titleString = new PngTexture("../resources/titleString.png");
  playString = new PngTexture("../resources/playString.png");
  scoreString = new PngTexture("../resources/scoreString.png");

  stringAtlus = new PngTexture("../resources/string.png");
  stringTitleRegion = new TextureRegion(stringAtlus, 0,                              0, stringAtlus->getWidth(), stringAtlus->getHeight()/4.0);
  stringPlayRegion  = new TextureRegion(stringAtlus, 0, 1*stringAtlus->getHeight()/4.0, stringAtlus->getWidth(), stringAtlus->getHeight()/4.0);
  stringScoreRegion = new TextureRegion(stringAtlus, 0, 2*stringAtlus->getHeight()/4.0, stringAtlus->getWidth(), stringAtlus->getHeight()/4.0);
  stringExitRegion  = new TextureRegion(stringAtlus, 0, 3*stringAtlus->getHeight()/4.0, stringAtlus->getWidth(), stringAtlus->getHeight()/4.0);
  
  target = new PngTexture("../resources/target.png");
  targetRegion = new TextureRegion(target, 0, 0, target->getWidth(), target->getHeight());  

  tapAnimation = new PngTexture("../resources/tap.png");
  charAtlus = new PngTexture("../resources/stringNumbers.png");
  for(int i=0; i<16; i++)
  {
    float t1 = (i%4)*0.25;
    float v1 = floor(i/4)*0.25;
    tapAnimationRegion[i] = new TextureRegion(tapAnimation, 256.0*t1, 256.0*v1, 256.0/4, 256.0/4);
    charRegion[i] =  new TextureRegion(charAtlus, 256.0*t1, 256.0*v1, 256.0/4, 256.0/4);
  }

  numbers = new PngTexture("../resources/number.png");
  for(int i=0; i<9; i++)
  {
    float t1 = (i%3)*0.33;
    float v1 = floor(i/3)*0.33;
    numberRegions[i] = new TextureRegion(numbers, 512*t1, 512*v1, 512.0/3, 512.0/3);
  }
  

}

void Assets::unloadFiles()
{
}
