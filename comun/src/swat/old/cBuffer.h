/*========================================================================*\
|* Buffer.h
|* Pondremos aqui las funciones para los buffers
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <GL/glew.h>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enum
//--------------------------------------------------------------------------
enum bufferType
{
    // Tipos/Formatos interleave
    tbufferSimple = 0,      // Solo tres coordenadas positions
    tbufferColor,           // positions + color(3)
    tbufferTexture,         // positions + tex(2)
    tbufferNormal,          // positions + norm(3)
    tbufferIndex,           // positions + color(3) + tex(2)                y glBufferData para indices
    tbufferColorIndex,      // positions + color(3)                         y glBufferData para indices
    tbufferModel,           // positions + color(3) + tex(2) + normals(3)   y glBufferData para indices
    tbufferNormalTex,       // positions + norm(3) + tex(2)
    tbufferNormalIndex,     // positions + norm(3) + tex(2)                 y glBufferData para indices
    tbufferColorTexIndex,   // positions + color(3) + tex(2)                y glBufferData para indices
    tbufferCube,
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cBuffer
{
    GLuint _iVao;
    GLuint _iVbo;
    GLuint _iEbo;

    bufferType _type;
    int _iNumElementos;

public:

    cBuffer(bufferType type = bufferType::tbufferSimple);
    ~cBuffer();

    int createBuffer(GLfloat* vertices, int sizeArray, int iSizeReg, int iInicioReg, int iNumUtiles, int iAtributo);
    int createBuffer(bufferType type, GLfloat* vertices, int sizeArray);
    int createBuffer(bufferType type, GLfloat* vertices, int sizeArray, GLuint* aIndices, int numIndices);
    int createSimpleBuffer(GLfloat* vertices, int sizeArray, GLuint& iVaoFinal);
    int createColorBuffer(GLfloat* vertices, int sizeArray, GLuint& iVaoFinal);
    int createTextureBuffer(GLfloat* vertices, int sizeArray, GLuint& iVaoFinal);
    int createNormalBuffer(GLfloat* vertices, int sizeArray, GLuint& iVaoFinal);
    int createNormalTexBuffer(GLfloat* vertices, int sizeArray, GLuint& iVaoFinal);
    int createColorIndexBuffer(GLfloat* aVertices, int sizeArray, GLuint* aIndices, int numIndices, GLuint& iVaoFinal, GLuint& iEboFinal);
    int createIndexBuffer(GLfloat* aVertices, int sizeArray, GLuint* aIndices, int numIndices, GLuint& iVaoFinal, GLuint& iEboFinal);
    int createModelBuffer(GLfloat* aVertices, int sizeArray, GLuint* aIndices, int numIndices, GLuint& iVaoFinal, GLuint& iEboFinal);
    int createNormalIndexBuffer(GLfloat* aVertices, int sizeArray, GLuint* aIndices, int numIndices, GLuint& iVaoFinal, GLuint& iEboFinal);
    int createColorTexIndexBuffer(GLfloat* aVertices, int sizeArray, GLuint* aIndices, int numIndices, GLuint& iVaoFinal, GLuint& iEboFinal);
    int createCubeBuffer();
    int useSimpleBuffer(GLuint iVao, int sizeofVertices);
    int useIndexBuffer(GLuint iVao, int sizeofIndices);
    int deleteSimpleBuffer(GLuint& iVao);
    int deleteIndexBuffer(GLuint& iVao, GLuint& iEbo);

    void setType(bufferType type);
    bufferType getType();
    void setNumElementos(int iNum);

    void use();
    void draw();
    void deleteBuffer();
    void bind();

    static void unBind();
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de Buffer.h
\*========================================================================*/
