#ifndef PNG_TEXTURE_H_2013_12_03
#define PNG_TEXTURE_H_2013_12_03
#include "Texture.h"
#include "lodepng.h"
#include <vector>
#include <sstream>

#include <GL/glut.h>
#include <GLFW/glfw3.h>

class PngTexture: public Texture
{
public:
  PngTexture();
  
  PngTexture(const char* fileName){
    load(fileName);
  }
  
  virtual void load(const char* fileName) throw(string)
  {
    vector<unsigned char> raw_image;
    unsigned int error = lodepng::decode(raw_image, width, height, fileName);

    if(error != 0)
    {
      std::stringstream ss;
      ss << "can not read file" << fileName; 
      throw ss.str();		//例外を投げる
    }

    unsigned int u2 = 1; while(u2 < width ) u2*=2;
    unsigned int v2 = 1; while(v2 < height) v2*=2;
    double u3 = (double) width/u2;
    double v3 = (double) height/v2;

    image = new unsigned char[u2 * v2 * 4];
    memset(image, 0, sizeof(unsigned char)*u2 * v2 * 4);	//0で初期化
    for(size_t y = 0; y < height; y++) 
      for(size_t x = 0; x < width; x++)
        for(size_t c = 0; c < 4; c++)
          image[4 * u2 * y + 4 * x + c] = raw_image[4 * width * y + 4 * x + c];

    texWidth = u2;
    texHeight = v2;
		
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight,0,GL_RGBA, GL_UNSIGNED_BYTE, image);
    glDisable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  virtual inline unsigned char *getImage()
  {
    return image;
  }

  virtual inline unsigned getWidth()
  {
    return width;
  }
  virtual inline unsigned getHeight()
  {
    return height;
  }

  virtual inline size_t getTexWidth()
  {
    return texWidth;
  }
  virtual inline size_t getTexHeight()
  {
    return texHeight;
  }
  virtual inline GLuint getTexId()
  {
    return texId;
  }
public:
  unsigned char *image;
  unsigned int width, height;		   //もとのイメージのサイズ
  size_t texWidth, texHeight; //確保したテクスチャイメージのサイズ(2の累乗)
};

#endif
