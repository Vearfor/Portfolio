/*========================================================================*\
|* Buffer.cpp
\*========================================================================*/


#include "Buffer.h"


//--------------------------------------------------------------------------
// Create Buffers
//--------------------------------------------------------------------------
#pragma region Create
int createSimpleBuffer(GLfloat* aVertices, int numElementVertices, GLuint& iVaoFinal)
{
    GLuint iVao = 0;
    GLuint iVbo = 0;

    glGenVertexArrays(1, &iVao);
    glGenBuffers(1, &iVbo);

    glBindVertexArray(iVao);
    glBindBuffer(GL_ARRAY_BUFFER, iVbo);
    {
        glBufferData(GL_ARRAY_BUFFER, numElementVertices, aVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    iVaoFinal = iVao;
    //iVboFinal = iVbo;

    return 0;
}


int createColorBuffer(GLfloat* aVertices, int numElementVertices, GLuint& iVaoFinal)
{
    GLuint iVao = 0;
    GLuint iVbo = 0;

    glGenVertexArrays(1, &iVao);
    glGenBuffers(1, &iVbo);

    glBindVertexArray(iVao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, iVbo);
        {
            glBufferData(GL_ARRAY_BUFFER, numElementVertices, aVertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    iVaoFinal = iVao;
    //iVboFinal = iVbo;

    return 0;
}


int createTextureBuffer(GLfloat* aVertices, int numElementVertices, GLuint& iVaoFinal)
{
    GLuint iVao = 0;
    GLuint iVbo = 0;

    glGenVertexArrays(1, &iVao);
    glGenBuffers(1, &iVbo);

    glBindVertexArray(iVao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, iVbo);
        glBufferData(GL_ARRAY_BUFFER, numElementVertices, aVertices, GL_STATIC_DRAW);

        // Atributo Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Attribute Texture Coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    iVaoFinal = iVao;
    //iVboFinal = iVbo;

    return 0;
}


int createIndexBuffer(
    GLfloat* aVertices, int numVertices,
    GLuint* aIndices, int numIndices,
    GLuint& iVaoFinal,
    //GLuint& iVboFinal,
    GLuint& iEboFinal)
{
    GLuint iVao = 0;
    GLuint iVbo = 0;
    GLuint iEbo = 0;

    glGenVertexArrays(1, &iVao);
    glGenBuffers(1, &iVbo);
    glGenBuffers(1, &iEbo);

    glBindVertexArray(iVao);
    {
        // Asociamos vertices a un array general de datos: posiciones, colores, coordenadas textura
        glBindBuffer(GL_ARRAY_BUFFER, iVbo);
        glBufferData(GL_ARRAY_BUFFER, numVertices, aVertices, GL_STATIC_DRAW);

        // Asociamos indices a un array de elementos
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices, aIndices, GL_STATIC_DRAW);

        // Positions atributo
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Colors atributo
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // Text. Coordinates atributo
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    iVaoFinal = iVao;
    //iVboFinal = iVbo;
    iEboFinal = iEbo;

    return 0;
}


int createModelBuffer(
    GLfloat* aVertices, int numVertices,
    GLuint* aIndices, int numIndices,
    GLuint& iVaoFinal,
    //GLuint& iVboFinal,
    GLuint& iEboFinal)
{
    GLuint iVao = 0;
    GLuint iVbo = 0;
    GLuint iEbo = 0;

    glGenVertexArrays(1, &iVao);
    glGenBuffers(1, &iVbo);
    glGenBuffers(1, &iEbo);

    glBindVertexArray(iVao);
    {
        // Asociamos vertices a un array general de datos: posiciones, colores, coordenadas textura
        glBindBuffer(GL_ARRAY_BUFFER, iVbo);
        glBufferData(GL_ARRAY_BUFFER, numVertices, aVertices, GL_STATIC_DRAW);

        // Asociamos indices a un array de elementos
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices, aIndices, GL_STATIC_DRAW);

        // Positions atributo
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Colors atributo
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // Text. Coordinates atributo
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        // Normals atributo
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
        glEnableVertexAttribArray(3);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    iVaoFinal = iVao;
    //iVboFinal = iVbo;
    iEboFinal = iEbo;

    return 0;
}
#pragma endregion


//--------------------------------------------------------------------------
// Use Buffers
//--------------------------------------------------------------------------
#pragma region Use
//--------------------------------------------------------------------------
// :: glDrawArrays
//--------------------------------------------------------------------------
int useSimpleBuffer(GLuint iVao, int sizeofVerticesTriangles)
{
    if (iVao > 0)
    {
        glBindVertexArray(iVao);
        glDrawArrays(GL_TRIANGLES, 0, sizeofVerticesTriangles);
        glBindVertexArray(0);
    }

    return 0;
}


//--------------------------------------------------------------------------
// Utilizamos Drawelements y no DrawArrays
// :: glDrawElements
//--------------------------------------------------------------------------
int useIndexBuffer(GLuint iVao, int sizeofIndices)
{
    if (iVao > 0)
    {
        glBindVertexArray(iVao);
        glDrawElements(GL_TRIANGLES, sizeofIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    return 0;
}
#pragma endregion


//--------------------------------------------------------------------------
// Delete Buffers
//--------------------------------------------------------------------------
#pragma region Delete
int deleteSimpleBuffer(GLuint& iVao)
{
    glDeleteVertexArrays(1, &iVao);
    //glDeleteBuffers(1, &iVbo);

    return 0;
}


int deleteIndexBuffer(GLuint& iVao, GLuint& iEbo)
{
    glDeleteVertexArrays(1, &iVao);
    //glDeleteBuffers(1, &iVbo);
    glDeleteBuffers(1, &iEbo);

    return 0;
}
#pragma endregion



/*========================================================================*\
|* Fin de Buffer.cpp
\*========================================================================*/
