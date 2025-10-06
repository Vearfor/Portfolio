/*========================================================================*\
|* cModel.cpp
\*========================================================================*/


#include "cModel.h"
#include "buffers/cMalla.h"
#include "texturas/cGestorTexturas.h"
// #include "cShader.h"
#include "../tool/cTool.h"


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cModel::cModel(const char *pathModel)
    : _directory("")
{
}

cModel::~cModel()
{
    ClearModel();
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
void cModel::DrawModel(GLint textureLocation)
{
    for (size_t i = 0; i < _meshesList.size(); i++)
    {
        cTextura* umbindTex = nullptr;
        unsigned int materialIndex = _meshToTex[i];
        if (materialIndex<_texturesList.size() && _texturesList[materialIndex])
        {
            // en principio no necesitamos el location, lo tenemos a -1, si siempre lo asociamos el texture unit 0: GL_TEXTURE0
            // supongo, que si hubiese mas, ambient o specular, o shadowMap, si me tendría que preocupar, pero por ahora solo en la diffuse.
            // 
            // Lo debería de dar el material asociado a la textura.
            //
            _texturesList[materialIndex]->useTextura(0, textureLocation, 0);
            umbindTex = _texturesList[materialIndex];
        }
        _meshesList[i]->drawMalla();
        // Terminado el mesh, se puede hacer el unbind de textura:
        // si es que ha existido:
        mDo(umbindTex)->unbind();
    }
}


void cModel::ClearModel()
{
    for (size_t i = 0; i < _meshesList.size(); i++)
    {
        delete _meshesList[i];
    }
    _meshesList.clear();

    // Las texturas deben de borrarse en el Gestor de Texturas.
    _texturesList.clear();
}


int cModel::LoadModel(const char* pathModel, const char* pcDirectorio)
{
    int iRes = 0;

    //printf("- Cargando Modelo: %s\n", pathModel);
    //std::string pathFileName;
    //if (pcDirectorio)
    //{
    //    pathFileName = pcDirectorio + std::string("/") + pathModel;
    //}
    //else
    //{
    //    pathFileName = pathModel;
    //}

    //Assimp::Importer importer;
    //const aiScene* scene = importer.ReadFile(pathFileName, 
    //    aiProcess_Triangulate | 
    //    aiProcess_FlipUVs |
    //    aiProcess_GenSmoothNormals |
    //    aiProcess_JoinIdenticalVertices
    //);
    //if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    //{
    //    printf("Error::cModel::LoadModel:: [%s] fallo en la carga: [%s]\n", pathFileName.c_str(), importer.GetErrorString());
    //    iRes = -1;
    //}
    //else
    //{
    //    std::string path = pathFileName;
    //    _directory = path.substr(0, path.find_last_of('/'));
    //    loadNode(scene->mRootNode, scene);
    //    loadMaterials(scene, pcDirectorio);
    //}

    return iRes;
}


//void cModel::loadNode(aiNode* node, const aiScene* scene)
//{
//    for (size_t i = 0; i < node->mNumMeshes; i++)
//    {
//        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//        loadMesh(mesh, scene);
//    }
//
//    for (size_t i = 0; i < node->mNumChildren; i++)
//    {
//        aiNode* nodeHijo = node->mChildren[i];
//        loadNode(nodeHijo, scene);
//    }
//}


//void cModel::loadMesh(aiMesh* mesh, const aiScene* scene)
//{
//    std::vector<GLfloat> vertices;
//    std::vector<unsigned int> indices;
//
//    for (size_t i = 0; i < mesh->mNumVertices; i++)
//    {
//        // Dependiendo de como seran al final los mesh/mallas/buffers se insertan en distintas posiciones:
//        //
//        // Aqui al estructura es: 3 pos, 2 tex, 3 norm.
//        //
//        vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
//        if (mesh->mTextureCoords[0])
//        {
//            vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
//        }
//        else
//        {
//            // Si no hubiera coordenadas de texturas se dejan vacios:
//            vertices.insert(vertices.end(), { 0.0f, 0.0f });
//        }
//        // Normales en negativo
//        vertices.insert(vertices.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
//    }
//
//    // Cada cara son 3 valores de indice al ser Triangulos
//    for (size_t i = 0; i< mesh->mNumFaces; i++)
//    {
//        aiFace face = mesh->mFaces[i];
//        for (size_t j = 0; j < face.mNumIndices; j++)
//        {
//            indices.push_back(face.mIndices[j]);
//        }
//    }
//
//    cMalla* newMalla = new cMalla();
//    //newMalla->createMalla(
//    //    (GLfloat*)&vertices[0], (int)vertices.size() * sizeof(GLfloat),
//    //    (GLuint*)&indices[0], (int)indices.size() * sizeof(GLuint)
//    //);
//    newMalla->createMalla(
//        (GLfloat*)&vertices[0], (int)vertices.size(),
//        (GLuint*)&indices[0], (int)indices.size()
//    );
//    _meshesList.push_back(newMalla);
//
//    _meshToTex.push_back(mesh->mMaterialIndex);
//}


//void cModel::loadMaterials(const aiScene* scene, const char* pcDirectorio)
//{
//    _texturesList.resize(scene->mNumMaterials);
//    for (size_t i = 0; i < scene->mNumMaterials; i++)
//    {
//        aiMaterial* mat = scene->mMaterials[i];
//
//        _texturesList[i] = nullptr;
//
//        if (mat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE))
//        {
//            aiString path;
//            if (mat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
//            {
//                // Tenemos que averiguar si usamos los dos o cual.
//                std::string spath = getCadenaAiString(path);
//                size_t idx1 = spath.rfind("\\");
//                size_t idx2 = spath.rfind("/");
//
//                std::string fileName = "";
//                if (idx1 != std::string::npos)
//                {
//                    fileName = spath.substr((int)idx1 + 1);
//                }
//                else if (idx2 != std::string::npos)
//                {
//                    fileName = spath.substr((int)idx2 + 1);
//                }
//                else
//                {
//                    fileName = spath;
//                }
//                std::string fileNamePath = _directory + "/" + fileName;
//
//                printf("  + Textura: %s\n", fileNamePath.c_str());
//                if (!cGestorTexturas::Instancia()->CargaTextura(fileNamePath.c_str(), nullptr, textureType::TexSimple, true))
//                {
//                    cTextura* tex = cGestorTexturas::Instancia()->GetTextura(fileName.c_str());
//                    _texturesList[i] = tex;
//                }
//                else
//                {
//                    // Si hubiera algun error seria bueno dejar una textura vacia con otra que sepamos que podemos cargar:
//                    if (!cGestorTexturas::Instancia()->CargaTextura("plain.png", pcDirectorio, textureType::TexSimple, true))
//                    {
//                        cTextura* tex = cGestorTexturas::Instancia()->GetTextura("plain.png");
//                        _texturesList[i] = tex;
//                    }
//                }
//            }
//        }
//    }
//}


int cModel::GetLocations(cShader* shader)
{
    return 0;
}
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cModel.cpp
\*========================================================================*/

