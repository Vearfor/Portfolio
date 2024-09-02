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
// Prototipos
//--------------------------------------------------------------------------
int createSimpleBuffer(GLfloat* vertices, int numElementVertices, GLuint& iVaoFinal);
int createColorBuffer(GLfloat* vertices, int numElementVertices, GLuint& iVaoFinal);
int createTextureBuffer(GLfloat* vertices, int numElementVertices, GLuint& iVaoFinal);
int createIndexBuffer(GLfloat* aVertices, int numVertices, GLuint* aIndices, int numIndices, GLuint& iVaoFinal, GLuint& iEboFinal);
int createModelBuffer(GLfloat* aVertices, int numVertices, GLuint* aIndices, int numIndices, GLuint& iVaoFinal, GLuint& iEboFinal);
int useSimpleBuffer(GLuint iVao, int sizeofVertices);
int useIndexBuffer(GLuint iVao, int sizeofIndices);
int deleteSimpleBuffer(GLuint& iVao);
int deleteIndexBuffer(GLuint& iVao, GLuint& iEbo);
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de Buffer.h
\*========================================================================*/
