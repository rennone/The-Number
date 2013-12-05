
#ifndef PNG_H_2013_11_19
#define PNG_H_2013_11_19

#include<GLFW/glfw3.h>
#include<vector>
#include"lodepng.h"
#include<sstream>

class Texture
{
protected:
GLuint texId;
public:
	virtual void load(const char*) = 0;
	virtual size_t getTexWidth()=0;	//�e�N�X�`���T�C�Y(power of 2)
	virtual size_t getTexHeight()=0;
	virtual unsigned getWidth()=0;  //�C���[�W�T�C�Y
	virtual unsigned getHeight()=0;
	virtual unsigned char* getImage()=0;
	virtual GLuint getTexId()=0;
        virtual void bind();
};

class TextureRegion
{
public:
  const float u1, v1;
  const float u2, v2;
  const Texture *texture;

  TextureRegion(Texture *_texture, float x, float y, float width, float height)
    :texture(_texture),
    u1(        1.0*x/_texture->getTexWidth()), v1(          1.0*y/_texture->getTexHeight()),
    u2(1.0*(x+width)/_texture->getTexWidth()), v2( 1.0*(y+height)/_texture->getTexHeight())
  {
  }
};

class PngTexture: public Texture
{
public:
  PngTexture();
  
  PngTexture(const char* fileName){
    load(fileName);
  }
  
  virtual void load(const char* fileName) throw(std::string)
  {
    std::vector<unsigned char> raw_image;
    unsigned int error = lodepng::decode(raw_image, width, height, fileName);

    if(error != 0)
    {
      std::stringstream ss;
      ss << "can not read file" << fileName; 
      throw ss.str();		//��O�𓊂���
    }

    unsigned int u2 = 1; while(u2 < width ) u2*=2;
    unsigned int v2 = 1; while(v2 < height) v2*=2;
    double u3 = (double) width/u2;
    double v3 = (double) height/v2;

    image = new unsigned char[u2 * v2 * 4];
    memset(image, 0, sizeof(unsigned char)*u2 * v2 * 4);	//0�ŏ�����
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
  unsigned int width, height;		   //���Ƃ̃C���[�W�̃T�C�Y
  size_t texWidth, texHeight; //�m�ۂ����e�N�X�`���C���[�W�̃T�C�Y(2�̗ݏ�)
};

#endif
