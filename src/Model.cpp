#include "Model.h"
#include "Png.h"
#include <iostream>
using namespace Model;

Vector3f& operator*(Vector3f &v,float size){
	v.x *= size;
	v.y *= size;
	v.z *= size;
	return v;
}

XFileModel::XFileModel(const char *fileName, float size)
{
	load(fileName, size);
} 

static struct Vector3f vec3d;
static struct Vector4f vec4d;
static struct UV vec2d;
static struct Triangle Tri;
static struct Quadrangle Quad;
static struct MATERIAL mtl;

void XFileModel::load(const char *fileName, float size)
{
	vector <Vector3f> Vertex;//頂点
	vector <Vector3f> Normal;//法線
	vector <UV> uv;//UV
	vector <int> VertexIndex;
	vector <int> NormalIndex;
	vector <int> MaterialIndex;

	char key[255];
	char buf[255];
	//Xファイルを開いて内容を読み込む
	FILE* fp=NULL;
	errno_t err;

	if( (fp = fopen(fileName, "rt")) == NULL)
	{
		cout << "can not open file" << endl;
		return;
	}

	int v1=0,v2=0,v3=0, v4=0;
	int Count=0;
	bool flag=false;
	string str="";
	int row=0;
	//読み込み 
	fseek(fp,SEEK_SET,0);
	while(!feof(fp))
	{
		//キーワード 読み込み
		memset(key, 0, sizeof(key));
                fscanf(fp,"%s ",key);
		//fscanf(fp,"%s ",key,sizeof(key));

		if(strcmp(key, "template") == 0)	//テンプレートの行は跳ばす
			fgets(buf,sizeof(buf),fp);

		if(strcmp(key,"Material")==0)
		{
			if(flag)Material.push_back(mtl);
			flag=true;
			fgets(buf,sizeof(buf),fp);//直後の行にあると推定　改行する
			//ディフューズ
			fscanf(fp,"%f;%f;%f;%f;;",&vec4d.x,&vec4d.y,&vec4d.z,&vec4d.w);
			mtl.MaterialColor.diffuse=(const Color4 &)vec4d;
			//SHININESS  
			fscanf(fp,"%f;",&mtl.Shininess);
			//スペキュラー 
			fscanf(fp,"%f;%f;%f;;",&vec4d.x,&vec4d.y,&vec4d.z);
			mtl.MaterialColor.specular=(const Color4 &)vec4d;
			//エミッシブ 
			fscanf(fp,"%f;%f;%f;;",&vec4d.x,&vec4d.y,&vec4d.z);
			mtl.MaterialColor.ambient=(const Color4 &)vec4d;
			//テクスチャー
                        fscanf(fp,"%s ",key);
			//fscanf(fp,"%s ",key,sizeof(key));
			if(strcmp(key,"TextureFilename")==0)
			{
				fgets(buf,sizeof(buf),fp);//直後の行にあると推定　改行する
				//map_Kd　テクスチャー
                                fscanf(fp,"%s",buf);
				//fscanf(fp,"%s",buf,255);
				mtl.TextureName=buf;
				str=mtl.TextureName;
				mtl.TextureName=str.substr(str.find_first_of('\"',0)+1,str.find_last_of('\"',255)-1);
				//テクスチャを作成
				TexData.push_back(tex);

				TexData[TexData.size()-1] = new PngTexture(mtl.TextureName.c_str());

				mtl.TexNo=TexData.size();
				TexID.push_back(TexID2);

				TexID[TexID.size()-1] = TexData[TexData.size()-1]->getTexId();
				/*
				glGenTextures(1, (GLuint *)&TexID[TexData.size()-1]);
				glBindTexture(GL_TEXTURE_2D, TexID[TexData.size()-1]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glEnable(GL_TEXTURE_2D);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,TexData[TexData.size()-1]->getWidth(), TexData[TexData.size()-1]->getHeight(),
					0,GL_RGBA, GL_UNSIGNED_BYTE, TexData[TexData.size()-1]->getImage());
				glDisable(GL_TEXTURE_2D); 				
				*/
			}
		}
		//頂点 読み込み
		if(strcmp(key,"Mesh")==0)
		{
			fgets(buf,sizeof(buf),fp);//データは2行下にあると推定　改行する
			fgets(buf,sizeof(buf),fp);
			Count=atoi(buf);
			for(int i=0;i<Count ;i++)
			{
				fscanf(fp,"%f;%f;%f;,",&vec3d.x,&vec3d.y,&vec3d.z);
				Vertex.push_back(vec3d*(float)size);
			} 
			//頂点インデックス読み込み  
			fgets(buf,sizeof(buf),fp);//データは2行下にあると推定　改行する
			fgets(buf,sizeof(buf),fp);
			while(strchr(buf,';')==NULL){fgets(buf,sizeof(buf),fp);}//空行対策
			Count=atoi(buf);   
			for(int i=0;i<Count ;i++)
			{
				int dammy;
				fscanf(fp,"%d;",&dammy);
				VertexIndex.push_back(dammy);
				if(dammy == 3){
					fscanf(fp,"%d,%d,%d;,",&v1,&v2,&v3);
					VertexIndex.push_back(v1);
					VertexIndex.push_back(v2);
					VertexIndex.push_back(v3);
				} else{
					fscanf(fp,"%d,%d,%d,%d;,",&v1,&v2,&v3,&v4);
					VertexIndex.push_back(v1);
					VertexIndex.push_back(v2);
					VertexIndex.push_back(v3);
					VertexIndex.push_back(v4);
				}
			} 
		}

		//法線 読み込み
		if(strcmp(key,"MeshNormals")==0)
		{
			row++;
			fgets(buf,sizeof(buf),fp);//データは2行下にあると推定　改行する
			fgets(buf,sizeof(buf),fp);
			Count=atoi(buf);
			for(int i=0;i<Count ;i++)
			{
				fscanf(fp,"%f;%f;%f;,",&vec3d.x,&vec3d.y,&vec3d.z);
				Normal.push_back(vec3d);
			}
			//法線インデックス読み込み  
			fgets(buf,sizeof(buf),fp);//データは2行下にあると推定　改行する
			fgets(buf,sizeof(buf),fp);
			while(strchr(buf,';')==NULL){fgets(buf,sizeof(buf),fp);}//空行対策
			Count=atoi(buf);   
			for(int i=0;i<Count ;i++)
			{
				int dammy;
				fscanf(fp,"%d;",&dammy);
				NormalIndex.push_back(dammy);
				if(dammy == 3){
					fscanf(fp,"%d,%d,%d;,",&v1,&v2,&v3);
					NormalIndex.push_back(v1);
					NormalIndex.push_back(v2);
					NormalIndex.push_back(v3);
				} else{
					fscanf(fp,"%d,%d,%d,%d;,",&v1,&v2,&v3,&v4);
					NormalIndex.push_back(v1);
					NormalIndex.push_back(v2);
					NormalIndex.push_back(v3);
					NormalIndex.push_back(v4);
				}
			} 
		}

		//テクスチャー座標 読み込み
		if(strcmp(key,"MeshTextureCoords")==0)
		{
			row++;
			fgets(buf,sizeof(buf),fp);//データは2行下にあると推定　改行する
			fgets(buf,sizeof(buf),fp);
			while(strchr(buf,';')==NULL){fgets(buf,sizeof(buf),fp);}//空行対策
			Count=atoi(buf);
			for(int i=0;i<Count ;i++)
			{
				fscanf(fp,"%f;%f;,",&vec2d.u,&vec2d.v);
				uv.push_back(vec2d);
			}   
		}
		//マテリアルリスト
		if(strcmp(key,"MeshMaterialList")==0)
		{
			row++;
			fgets(buf,sizeof(buf),fp);//空改行
			fgets(buf,sizeof(buf),fp);//マテリアル数
			fgets(buf,sizeof(buf),fp);//リスト要素数
			Count=atoi(buf);
			for(int i=0;i<Count;i++)
			{
				fgets(buf,sizeof(buf),fp);
				int test=atoi(buf);
				MaterialIndex.push_back(test);
			}
		}
	}
	if(flag)Material.push_back(mtl);

	fclose(fp);

	for(int i=0;i<Material.size();i++){
		for(int j=0;j<VertexIndex.size();){
			if(VertexIndex[j] == 3){
				for( int k=1; k<=3; k++)
				{
					Tri.TriVer=Vertex[VertexIndex[j+k]];
					Tri.TriNor=Normal[NormalIndex[j+k]];
					Tri.TriUV=uv[VertexIndex[j+k]];
					Material[i].Tridata.push_back(Tri);
				}
				j+=4;
			} else {
				for( int k=1; k<=4; k++)
				{

					Quad.QuadVer=Vertex[VertexIndex[j+k]];
					Quad.QuadNor=Normal[NormalIndex[j+k]];
					Quad.QuadUV=uv[VertexIndex[j+k]];
					Material[i].Quaddata.push_back(Quad);
				}
				j+=5;
			}
		}
	}


	Vertex.clear(); 
	Normal.clear();
	uv.clear();
	VertexIndex.clear();
	NormalIndex.clear();

	return;
}

//描画
void XFileModel::draw(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	for(int i=0;i<Material.size();i++){
		glPushMatrix();
		glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,(const GLfloat *)&Material[i].MaterialColor.ambient);
		glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,(const GLfloat *)&Material[i].MaterialColor.diffuse);
		glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,(const GLfloat *)&Material[i].MaterialColor.specular);
		glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Material[i].Shininess);
		if(Material[i].TexNo>0){
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, TexID[Material[i].TexNo-1]);
		}else{
			glDisable(GL_TEXTURE_2D);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
		if(Material[i].Tridata.size()>1){

			glVertexPointer(3, GL_FLOAT, sizeof(Tri) , &Material[i].Tridata[0].TriVer.x);
			glNormalPointer(   GL_FLOAT, sizeof(Tri) , &Material[i].Tridata[0].TriNor.x);
			if(Material[i].TexNo>0)glTexCoordPointer(2, GL_FLOAT, sizeof(Tri), &Material[i].Tridata[0].TriUV.u);
			glDrawArrays(GL_TRIANGLES,0,Material[i].Tridata.size());
		}
		if(Material[i].Quaddata.size()>1){
			glVertexPointer(3, GL_FLOAT,sizeof(Quad) , &Material[i].Quaddata[0].QuadVer.x);
			glNormalPointer(   GL_FLOAT,sizeof(Quad),&Material[i].Quaddata[0].QuadNor.x);
			if(Material[i].TexNo>0)glTexCoordPointer(2, GL_FLOAT, sizeof(Quad), &Material[i].Quaddata[0].QuadUV.u);
			glDrawArrays(GL_QUADS,0,Material[i].Quaddata.size());
		}
		glPopMatrix();
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}
