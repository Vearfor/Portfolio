/*========================================================================*\
|* cModel.h
\*========================================================================*/


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <string>
//#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "sOpenGL.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
//#include <SOIL2/SOIL2.h>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cShader;
class cTextura;
class cMalla;
//--------------------------------------------------------------------------
class cModel
{
public:
	cModel(const char* pathModel = nullptr);
	~cModel();

	int LoadModel(const char* pathModel, const char* pcDirectorio = nullptr);
	void DrawModel(GLint textureLocation=-1);
	void ClearModel();

	int GetLocations(cShader* shader);

private:

	std::vector<cMalla*> _meshesList;
	std::vector<cTextura*> _texturesList;
	std::vector<unsigned int> _meshToTex;
	std::string _directory;

	//void loadNode(aiNode* node, const aiScene* scene);
	//void loadMesh(aiMesh* mesh, const aiScene* scene);
	//void loadMaterials(const aiScene* scene, const char* pcDirectorio=nullptr);
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cModel.h
\*========================================================================*/
