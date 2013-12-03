#include "Texture.h"
#include <GLFW/glfw3.h>

void Texture::bind()
{
  glBindTexture(GL_TEXTURE_2D, texId);
}
