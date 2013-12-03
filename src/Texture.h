
#ifndef PNG_H_2013_11_19
#define PNG_H_2013_11_19

class Texture
{
GLuint texId;
public:
	virtual void load(const char*) = 0;
	virtual size_t getTexWidth()=0;	//テクスチャサイズ(power of 2)
	virtual size_t getTexHeight()=0;
	virtual unsigned getWidth()=0;  //イメージサイズ
	virtual unsigned getHeight()=0;
	virtual unsigned char* getImage()=0;
	virtual GLuint getTexId()=0;
virtual bind();
};

class TextureRegion
{
public:
  float u1, v1;
  float u2, v2;
  Texture *texture;

  TextureRegion(Texture *_texture, float x, float y, float width, float height)
    :texture(_texture),
    u1(x/_texture->getTexWidth()), v1(y/_texture->getTexHeight()),
    u2((x+width)/_texture->getTexWidth()), v2((y+height)/_texture->getTexHeight())
  {
  }
};

#endif
