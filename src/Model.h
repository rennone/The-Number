#ifndef MODEL_H_2013_11_19
#define MODEL_H_2013_11_19

#include <vector>
#include <GL/glut.h>
#include "Texture.h"
using namespace std;

namespace Model
{
	//3�̃x�N�g��
	struct Vector3f{
		float x;
		float y;
		float z;
	};

	//4�̃x�N�g��
	struct Vector4f{
		float x;
		float y;
		float z;
		float w;
	};

	//4�̃J���[
	struct Color4{
		float r;
		float g;
		float b;
		float a;
	};

	//4�̔���
	struct Reflection4{
		Color4 diffuse;
		Color4 ambient;
		Color4 emission;
		Color4 specular;
	};

	//UV���W
	struct UV{
		float u;//u�l
		float v;//v�l
	};

	struct Vector4I{
		int x;
		int y;
		int z;
		int w;
	};

	//�|���S���f�[�^
	struct Triangle{
		Vector3f TriVer;
		Vector3f TriNor;
		UV TriUV;
	};

	//�|���S���f�[�^
	struct Quadrangle{
		Vector3f QuadVer;
		Vector3f QuadNor;
		UV QuadUV;
	};

	//�}�e���A���\����
	struct MATERIAL{
		string MaterialName;//�}�e���A����
		Reflection4 MaterialColor;//����
		float Shininess;//shininess
		string TextureName;//�e�N�X�`����
		int TexNo;//�e�N�X�`��NO.
		vector <Triangle> Tridata;//�O�p�ʃf�[�^
		vector <Quadrangle> Quaddata;//�l�p�ʃf�[�^
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
		vector <Texture*> TexData;//�e�N�X�`���f�[�^
		vector<GLuint> TexID;//�e�N�X�`��ID
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
