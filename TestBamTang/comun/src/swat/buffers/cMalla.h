/*========================================================================*\
|* cMalla.h
|* 
|* Calse cMesh o cBuffer de "Game Institute" (Steve jones)
|* 
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <unordered_map>
#include <dll/nExport.h>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enumerados
//--------------------------------------------------------------------------
enum class eVertexType : int
{
    eVer_Pos = 0,
    eVer_PosTex,            // 3 de position, 2 de texturas
    eVer_PosTexNor,         // 3 de position, 2 de texturas, 3 de normales
    eVer_NumTypes           // Nuumero de tipos
};

//--------------------------------------------------------------------------
// structs de vertex segun los eVertexType 
//--------------------------------------------------------------------------
struct sVertex_posTex
{
    glm::vec3 _position;
    glm::vec2 _texCoords;
};
//--------------------------------------------------------------------------
struct sVertex_posTexNor
{
    glm::vec3 _position;
    glm::vec2 _texCoords;
    glm::vec3 _normal;
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class _mExport cMalla
{
public:

    cMalla();
    ~cMalla();

    int createMalla(GLfloat* vertices, int sizeVertices, GLuint* indices, int sizeIndices, eVertexType verType = eVertexType::eVer_PosTexNor);
    void deleteMalla();
    void drawMalla();

private:

    GLuint m_iVao;
    GLuint m_iVbo;
    GLuint m_iEbo;
    int m_iNumElementos;
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cMalla.h
\*========================================================================*/
