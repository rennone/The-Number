#ifndef MODEL_H_2013_11_19
#define MODEL_H_2013_11_19

#include <vector>
#include <GL/glut.h>
#include "Texture.h"
using namespace std;

namespace Model
{
	//3つのベクトル
	struct Vector3f{
		float x;
		float y;
		float z;
	};

	//4つのベクトル
	struct Vector4f{
		float x;
		float y;
		float z;
		float w;
	};

	//4つのカラー
	struct Color4{
		float r;
		float g;
		float b;
		float a;
	};

	//4つの反射
	struct Reflection4{
		Color4 diffuse;
		Color4 ambient;
		Color4 emission;
		Color4 specular;
	};

	//UV座標
	struct UV{
		float u;//u値
		float v;//v値
	};

	struct Vector4I{
		int x;
		int y;
		int z;
		int w;
	};

	//ポリゴンデータ
	struct Triangle{
		Vector3f TriVer;
		Vector3f TriNor;
		UV TriUV;
	};

	//ポリゴンデータ
	struct Quadrangle{
		Vector3f QuadVer;
		Vector3f QuadNor;
		UV QuadUV;
	};

	//マテリアル構造体
	struct MATERIAL{
		string MaterialName;//マテリアル名
		Reflection4 MaterialColor;//反射
		float Shininess;//shininess
		string TextureName;//テクスチャ名
		int TexNo;//テクスチャNO.
		vector <Triangle> Tridata;//三角面データ
		vector <Quadrangle> Quaddata;//四角面データ
	};

	class Model
	{
	public:
		virtual void draw() = 0;
		virtual void load(const char *, float) = 0;
		virtual void dispose() = 0;
	};

	class XFileModel : public Model
	{
	protected:
		vector <MATERIAL> Material;
		vector <Texture*> TexData;//テクスチャデータ
		vector<GLuint> TexID;//テクスチャID
		GLuint TexID2;
		Texture* tex;
	public:
		XFileModel(){};
		XFileModel(const char *, float);
		virtual void load(const char *, float);
		virtual void draw();
		virtual void dispose(){};
	};
}
#endif
