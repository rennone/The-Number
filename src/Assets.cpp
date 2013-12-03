#include "Assets.h"

Texture* Assets::target = NULL;
Texture* Assets::numbers = NULL;
Texture* Assets::titleBground = NULL;
Texture* Assets::titleString = NULL;
Texture* Assets::playString = NULL;
Texture* Assets::scoreString = NULL;

void Assets::loadFiles()
{
  titleBground = new PngTexture("../resources/background.png");
  titleString = new PngTexture("../resources/titleString.png");
  playString = new PngTexture("../resources/playString.png");
  scoreString = new PngTexture("../resources/scoreString.png");
  target = new PngTexture("../resources/target.png");
  numbers = new PngTexture("../resources/number.png");        
}
