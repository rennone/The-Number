#ifndef GL_UTIL_H_2013_11_19
#define GL_UTIL_H_2013_11_19

extern void glDrawGrid(const float xNum,const float zNum,const float size);
extern void glDrawTextureCube(const float size, const int tex, const bool reverse=false);
extern void glDrawTexture(const float width, const float height, const int tex, const float depth=0);
extern void glDrawTexture(const float width, const float height, float left, float right, float top, float bottom,const int tex);
extern void glDrawAxis();

#endif
