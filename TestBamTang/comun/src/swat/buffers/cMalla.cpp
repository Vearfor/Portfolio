/*========================================================================*\
|* cMalla.h
|*
|* Modificado para tener distintas organizaciones de los buffers segun 
|* el eVertexType
\*========================================================================*/

#include "cMalla.h"


std::unordered_map< eVertexType, int> mapVertexStride = {
    { eVertexType::eVer_Pos, 3},
    { eVertexType::eVer_PosTex, 5},
    { eVertexType::eVer_PosTexNor, 8},
};


//--------------------------------------------------------------------------
// Cosntructor & Destructor
//--------------------------------------------------------------------------
cMalla::cMalla()
    : m_iVao(0)
    , m_iVbo(0)
    , m_iEbo(0)
    , m_iNumElementos(0)
{
}


cMalla::~cMalla()
{
	// No podemos hacer deleteBuffer en el destructor, si vamos a utilizar
	// copias en las listas de mallas/mesh/buffers:
    // al hacer delete de las copias eliminariammos los buffers cargados
    // y no tendrian sentido sus copias.
    // 
    // Al final si:
    deleteMalla();
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
/*------------------------------------------------------------------------*\
|* Modificado para tener distintas organizaciones de los buffers segun
|* el eVertexType
\*------------------------------------------------------------------------*/
int cMalla::createMalla(
    GLfloat* vertices, int sizeVertices,
    GLuint* indices, int sizeIndices,
    eVertexType verType)
{
    // No creamos nada si no es necesario
    // por ejemplo: el tipo dado es incorrecto

    if (
            (static_cast<int>(verType) > (static_cast<int>(eVertexType::eVer_NumTypes) - 1)) ||
            !vertices ||
            sizeVertices < 1
        )
    {
        return 0;
    }

    int stride = mapVertexStride[verType];

    m_iNumElementos = (indices && sizeIndices > 0) ?
        sizeIndices / sizeof(GLint) :
        (sizeVertices / sizeof(GLfloat)) / stride;      // Hmmm !! no se !! por el tipo eVertexType::eVer_PosTex: que da un 'stride' de 5 ?? por comprobar !!

    // Quizas deberia dividirse por el stride: 5 o 8 segun la organizacion. Pero sin hacerlo, parece que funciona.

    glGenVertexArrays(1, &m_iVao);
    glBindVertexArray(m_iVao);
    {
        if (indices && sizeIndices > 0)
        {
            glGenBuffers(1, &m_iEbo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEbo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, indices, GL_STATIC_DRAW);
        }

        glGenBuffers(1, &m_iVbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_iVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeVertices, vertices, GL_STATIC_DRAW);
        {
            switch (verType)
            {
                case eVertexType::eVer_Pos:
                    {
                        // Estructura: posiciones : 3
                        //
                        // Atributo Positions
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 3, (GLvoid*)0);
                        glEnableVertexAttribArray(0);
                    }
                    break;

                case eVertexType::eVer_PosTex:
                    {
                        // Estructura: posiciones + texturas : 3 + 2 = 5
                        // 
                        // Atributo Positions
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (GLvoid*)0);
                        glEnableVertexAttribArray(0);

                        // Attribute Texture Coordinates
                        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 5, (GLvoid*)(sizeof(vertices[0]) * 3));
                        glEnableVertexAttribArray(1);
                    }
                    break;

                case eVertexType::eVer_PosTexNor:
                    {
                        // Estructura: posiciones + texturas + normales : 3 + 2 + 3 = 8
                        // 
                        // Atributo Positions
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (GLvoid*)0);
                        glEnableVertexAttribArray(0);

                        // Attribute Texture Coordinates
                        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (GLvoid*)(sizeof(vertices[0]) * 3));
                        glEnableVertexAttribArray(1);

                        // Attribute Normals
                        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (GLvoid*)(sizeof(vertices[0]) * 5));
                        glEnableVertexAttribArray(2);
                    }
                    break;

                case eVertexType::eVer_NumTypes:
                default:
                    break;
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    // unbind to make sure other code doesn't change it
    glBindVertexArray(0);

    return 0;
}


void cMalla::drawMalla()
{
    if (m_iVao > 0 && m_iVbo > 0)
    {
        glBindVertexArray(m_iVao);
        if (m_iEbo > 0)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iEbo);
            glDrawElements(GL_TRIANGLES, m_iNumElementos, GL_UNSIGNED_INT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, m_iNumElementos);
        }
        glBindVertexArray(0);
    }
}


void cMalla::deleteMalla()
{
    if (m_iEbo > 0)
    {
        glDeleteBuffers(1, &m_iEbo);
        m_iEbo = 0;
    }
    if (m_iVbo > 0)
    {
        glDeleteBuffers(1, &m_iVbo);
        m_iVbo = 0;
    }
    if (m_iVao > 0)
    {
        glDeleteVertexArrays(1, &m_iVao);
        m_iVao = 0;
    }
    m_iNumElementos = 0;
}
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cMalla.h
\*========================================================================*/
