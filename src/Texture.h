
#ifndef PNG_H_2013_11_19
#define PNG_H_2013_11_19


#include<GLFW/glfw3.h>
#include<vector>
#include"lodepng.h"
#include<sstream>
#include "Debugger.h"
class Texture
{
  unsigned int texWidth;  //texture size (power of 2)
  unsigned int texHeight;
  unsigned int width;     //pixel size
  unsigned int height;
  GLuint texId;
public:
  virtual ~Texture()
  {
  }
  
  virtual void load(const char*) = 0;
        
  virtual const unsigned& Width() const
  {
    return width;
  }
  
  virtual const unsigned& Height() const
  {
    return height;
  }

  virtual void Width(const unsigned &_width)
  {
    width = _width;
  }
  
  virtual void Height(const unsigned &_height)
  {
    height = _height;
  }
  
  virtual void bind() const
  {
    glBindTexture(GL_TEXTURE_2D, texId);
  }
  
protected:
  virtual const unsigned& TexWidth() const
  {
    return texWidth;
  }
  
  virtual const unsigned& TexHeight() const
  {
    return texHeight;
  }

  virtual void TexWidth(const unsigned &_width)
  {
    texWidth = _width;
  }
  
  virtual void TexHeight(const unsigned &_height)
  {
    texHeight = _height;
  }

  virtual void TexId(const GLuint &_texId)
  {
    texId = _texId;
  }
  
  virtual const GLuint& TexId() const
  {
    return texId;
  } 
};

class TextureRegion
{
public:
  const float u1, v1;
  const float u2, v2;
  const float ratio; // height/width;
  const Texture * const texture;
  
  TextureRegion(const Texture *_texture,const float x,const float y,const float width,const float height)
    :texture(_texture),
    u1(        1.0*x/_texture->Width()), v1(          1.0*y/_texture->Height()),
    u2(1.0*(x+width)/_texture->Width()), v2( 1.0*(y+height)/_texture->Height()), ratio(height/width)
  {
  }
};

/*
class TextureAnimation
{
  std::vector<TextureRegion*> regions;
  const Texture* const texture;
  int index;
  bool repeat;
  float loopTime;
  float elapsedTime;
public:
TextureAnimation(Texture *_texture, float _loopTime)
  :texture(_texture), index(0), loopTime(_loopTime), elapsedTime(0)
  {
  }

  void start()
  {
    index = 0;
    elapsedTime = 0;
  }
  
  void addFrame(const TextureRegion *region)
  {
    regions.push_back(region);
  }

  void update(float delta)
  {
    elapsedTime+=delta;
    if(regions.size() == 0)
      return;
    if(elapsedTime > 1.0/region.size())
    {
      next();
      elapsedTime -= 1.0/region.size();
    }
  }
  
  void next()
  {
    if(repeat)
      index = (index+1)%regions.size();
    else
      index = min(regions.size()-1, index+1);
  }

  TextureRegion* frame() const
  {
    return regions[index];
  }

  void Repeat(bool _repeat)
  {
    repeat = _repeat;
  }

  bool isLast() const
  {
    return index == regions.size()-1;
  }

  int numFrame() const
  {
    return regions.size();
  }
};
*/
class PngTexture:public Texture
{
  unsigned char *image;
public:
  PngTexture(const char* fileName){
    load(fileName);
  }
  
  virtual void load(const char* fileName) throw(std::string)
  {
    std::vector<unsigned char> raw_image;
    unsigned int width, height;
    
    unsigned int error = lodepng::decode(raw_image, width, height, fileName);
    
    
    if(error != 0)
    {
      std::stringstream ss;
      ss << "can not read file" << fileName; 
      throw ss.str();		//—áŠO‚ð“Š‚°‚é
    }
    
    unsigned int u2 = 1; while(u2 < width ) u2*=2;
    unsigned int v2 = 1; while(v2 < height) v2*=2;
    double u3 = (double) width/u2;
    double v3 = (double) height/v2;
    
    Width(width);     Height(height);
    TexWidth(u2);    TexHeight(v2);
    
    
    image = new unsigned char[u2 * v2 * 4];
    memset(image, 0, sizeof(unsigned char)*u2 * v2 * 4);	//0‚Å‰Šú‰»
    for(size_t y = 0; y < height; y++) 
      for(size_t x = 0; x < width; x++)
        for(size_t c = 0; c < 4; c++)
          image[4 * u2 * y + 4 * x + c] = raw_image[4 * width * y + 4 * x + c];

    GLuint texId;
    glGenTextures(1, &texId);
    TexId(texId);
    
    glBindTexture(GL_TEXTURE_2D, TexId());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glEnable(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TexWidth(), TexHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glDisable(GL_TEXTURE_2D); 
    glBindTexture(GL_TEXTURE_2D, 0);    
  }

};

#endif
