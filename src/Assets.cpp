#include "Assets.h"

Texture* Assets::shadow = NULL;
Texture* Assets::target = NULL;
Texture* Assets::numbers = NULL;

void Assets::loadFiles()
{
  shadow = new PngTexture("../resources/shadow.png");
  target = new PngTexture("../resources/target.png");
  numbers = new PngTexture("../resources/number.png");        
}
