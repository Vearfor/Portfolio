/*========================================================================*\
|* Buffer.cpp
\*========================================================================*/


#include "cBuffer.h"


//--------------------------------------------------------------------------
// Valores Skybox
//--------------------------------------------------------------------------
GLfloat skyboxVertices[] = {
    // Positions
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cBuffer::cBuffer(bufferType type)
    : _type(type)
    , _iVao(-1)
    , _iVbo(-1)
    , _iEbo(-1)
    , _iNumElementos(0)
{
}

cBuffer::~cBuffer()
{
    deleteBuffer();
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
void cBuffer::setType(bufferType type)
{
    _type = type;
}


bufferType cBuffer::getType()
{
    return _type;
}


void cBuffer::setNumElementos(int iNum)
{
    _iNumElementos = iNum;
}


//--------------------------------------------------------------------------
// Create Buffers
//--------------------------------------------------------------------------
#pragma region Create
/// <summary>
/// Creacion de un buffer VAO simple , pero variable
/// </summary>
/// <param name="vertices">array de datos/vertices</param>
/// <param name="sizeArray">tamaño en bytes del array</param>
/// <param name="iSizeReg">numero/size del registro/tupla de datos</param>
/// <param name="iInicioReg">en que posicion empieza los datos utiles</param>
/// <param name="iNumUtiles">numero de datos utiles</param>
/// <param name="iAtributo">Atributo del shader/layout</param>
/// <returns></returns>
int cBuffer::createBuffer(GLfloat* vertices, int sizeArray, int iSizeReg, int iInicioReg, int iNumUtiles, int iAtributo)
{
    // Se ejecuta como glDrawArrays
    _type = bufferType::tbufferSimple;

    _iVao = 0;
    _iVbo = 0;

    _iNumElementos = (sizeArray / sizeof(GLfloat)) / iSizeReg;

    glGenVertexArrays(1, &_iVao);
    glGenBuffers(1, &_iVbo);

    glBindVertexArray(_iVao);
    glBindBuffer(GL_ARRAY_BUFFER, _iVbo);
    {
        glBufferData(GL_ARRAY_BUFFER, sizeArray, vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(iAtributo, iNumUtiles, GL_FLOAT, GL_FALSE, iSizeReg * sizeof(GLfloat), (GLvoid*)(iInicioReg * sizeof(GLfloat)));
        glEnableVertexAttribArray(iAtributo);

        // Por ejemplo, esta entrada definiría el glVertexAttribPointer con estos datos:
        // 
        // createBuffer(vertices, sizeof(vertices), 6, 0, 3, 0)
        // SizeReg    = 6
        // InicioReg  = 0
        // iNumUtiles = 3
        // Atributo   = 0
        // 
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
        //
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return 0;
}


int cBuffer::createBuffer(bufferType type, GLfloat* vertices, int sizeArray)
{
    int iRes = 0;
    switch (type)
    {
        case tbufferSimple:
            iRes = createSimpleBuffer(vertices, sizeArray, _iVao);
            break;
        case tbufferColor:
            iRes = createColorBuffer(vertices, sizeArray, _iVao);
            break;
        case tbufferTexture:
            iRes = createTextureBuffer(vertices, sizeArray, _iVao);
            break;
        case tbufferNormal:
            iRes = createNormalBuffer(vertices, sizeArray, _iVao);
            break;
        case tbufferNormalTex:
            iRes = createNormalTexBuffer(vertices, sizeArray, _iVao);
            break;

        case tbufferIndex:
        case tbufferModel:
        case tbufferNormalIndex:
        case tbufferColorIndex:
        case tbufferColorTexIndex:
        default:
            break;
    }
    return iRes;
}


int cBuffer::createBuffer(bufferType type, GLfloat* vertices, int sizeArray, GLuint* aIndices, int sizeArrayIndices)
{
    int iRes = 0;
    switch (type)
    {
        case tbufferIndex:
            iRes = createIndexBuffer(vertices, sizeArray, aIndices, sizeArrayIndices, _iVao, _iEbo);
            break;

        case tbufferModel:
            iRes = createModelBuffer(vertices, sizeArray, aIndices, sizeArrayIndices, _iVao, _iEbo);
            break;

        case tbufferNormalIndex:
            iRes = createNormalIndexBuffer(vertices, sizeArray, aIndices, sizeArrayIndices, _iVao, _iEbo);
            break;

        case tbufferColorIndex:
            iRes = createColorIndexBuffer(vertices, sizeArray, aIndices, sizeArrayIndices, _iVao, _iEbo);
            break;

        case tbufferColorTexIndex:
            iRes = createColorTexIndexBuffer(vertices, sizeArray, aIndices, sizeArrayIndices, _iVao, _iEbo);
            break;

        case tbufferSimple:
        case tbufferColor:
        case tbufferTexture:
        case tbufferNormal:
        case tbufferNormalTex:
        default:
            break;
    }
    return iRes;
}


int cBuffer::createSimpleBuffer(GLfloat* aVertices, int sizeArray, GLuint& iVaoFinal)
{
    _iVao = 0;
    _iVbo = 0;

    _type = bufferType::tbufferSimple;

    _iNumElementos = (sizeArray / sizeof(GLfloat)) / 3;

    glGenVertexArrays(1, &_iVao);
    glGenBuffers(1, &_iVbo);

    glBindVertexArray(_iVao);
    glBindBuffer(GL_ARRAY_BUFFER, _iVbo);
    {
        glBufferData(GL_ARRAY_BUFFER, sizeArray, aVertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    iVaoFinal = _iVao;

    return 0;
}


int cBuffer::createColorBuffer(GLfloat* aVertices, int sizeArray, GLuint& iVaoFinal)
{
    _iVao = 0;
    _iVbo = 0;

    _type = bufferType::tbufferColor;

    _iNumElementos = (sizeArray / sizeof(GLfloat)) / 6;

    glGenVertexArrays(1, &_iVao);
    glGenBuffers(1, &_iVbo);

    glBindVertexArray(_iVao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, _iVbo);
        {
            glBufferData(GL_ARRAY_BUFFER, sizeArray, aVertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    iVaoFinal = _iVao;
    //iVboFinal = _iVbo;

    return 0;
}


int cBuffer::createTextureBuffer(GLfloat* aVertices, int sizeArray, GLuint& iVaoFinal)
{
    _iVao = 0;
    _iVbo = 0;

    _type = bufferType::tbufferTexture;

    _iNumElementos = (sizeArray / sizeof(GLfloat)) / 5;

    glGenVertexArrays(1, &_iVao);
    glGenBuffers(1, &_iVbo);

    glBindVertexArray(_iVao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, _iVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeArray, aVertices, GL_STATIC_DRAW);

        // Atributo Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Attribute Texture Coordinates
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    iVaoFinal = _iVao;

    return 0;
}


int cBuffer::createNormalBuffer(GLfloat* aVertices, int sizeArray, GLuint& iVaoFinal)
{
    _iVao = 0;
    _iVbo = 0;

    _type = bufferType::tbufferNormal;

    _iNumElementos = (sizeArray / sizeof(GLfloat)) / 6;

    glGenVertexArrays(1, &_iVao);
    glGenBuffers(1, &_iVbo);

    glBindVertexArray(_iVao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, _iVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeArray, aVertices, GL_STATIC_DRAW);

        // Atributo Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Attribute Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    iVaoFinal = _iVao;
    //iVboFinal = _iVbo;

    return 0;
}


int cBuffer::createNormalTexBuffer(GLfloat* vertices, int sizeArray, GLuint& iVaoFinal)
{
    _iVao = 0;
    _iVbo = 0;

    _type = bufferType::tbufferNormalTex;

    _iNumElementos = (sizeArray / sizeof(GLfloat)) / 8;

    glGenVertexArrays(1, &_iVao);
    glGenBuffers(1, &_iVbo);

    glBindVertexArray(_iVao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, _iVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeArray, vertices, GL_STATIC_DRAW);

        // Attribute Positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Attribute Normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // Attribute Texture Coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    iVaoFinal = _iVao;

    return 0;
}


int cBuffer::createColorIndexBuffer(
    GLfloat* aVertices,
    int sizeArrayVertices,
    GLuint* aIndices,
    int sizeArrayIndices,
    GLuint& iVaoFinal,
    GLuint& iEboFinal)
{
    _iVao = 0;
    _iVbo = 0;
    _iEbo = 0;

    _type = bufferType::tbufferIndex;

    _iNumElementos = (sizeArrayIndices / sizeof(GLuint));

    glGenVertexArrays(1, &_iVao);
    glGenBuffers(1, &_iVbo);
    glGenBuffers(1, &_iEbo);

    glBindVertexArray(_iVao);
    {
        // Asociamos vertices a un array general de datos: posiciones, colores, coordenadas textura
        glBindBuffer(GL_ARRAY_BUFFER, _iVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeArrayVertices, aVertices, GL_STATIC_DRAW);

        // Asociamos indices a un array de elementos
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeArrayIndices, aIndices, GL_STATIC_DRAW);

        // Positions atributo
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Colors atributo
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    iVaoFinal = _iVao;
    iEboFinal = _iEbo;

    return 0;
}


int cBuffer::createIndexBuffer(
    GLfloat* aVertices, int sizeArrayVertices,
    GLuint* aIndices, int sizeArrayIndices,
    GLuint& iVaoFinal,
    GLuint& iEboFinal)
{
    _iVao = 0;
    _iVbo = 0;
    _iEbo = 0;

    _type = bufferType::tbufferIndex;

    // _iNumElementos = (sizeArrayVertices / sizeof(GLfloat)) / 8;
    _iNumElementos = (sizeArrayIndices / sizeof(GLuint));

    glGenVertexArrays(1, &_iVao);
    glGenBuffers(1, &_iVbo);
    glGenBuffers(1, &_iEbo);

    glBindVertexArray(_iVao);
    {
        // Asociamos vertices a un array general de datos: posiciones, colores, coordenadas textura
        glBindBuffer(GL_ARRAY_BUFFER, _iVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeArrayVertices, aVertices, GL_STATIC_DRAW);

        // Asociamos indices a un array de elementos
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeArrayIndices, aIndices, GL_STATIC_DRAW);

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

    iVaoFinal = _iVao;
    iEboFinal = _iEbo;

    return 0;
}


int cBuffer::createModelBuffer(
    GLfloat* aVertices, int sizeArrayVertices,
    GLuint* aIndices, int sizeArrayIndices,
    GLuint& iVaoFinal,
    //GLuint& iVboFinal,
    GLuint& iEboFinal)
{
    _iVao = 0;
    _iVbo = 0;
    _iEbo = 0;

    _type = bufferType::tbufferModel;

    // _iNumElementos = (sizeArrayVertices / sizeof(GLfloat)) / 11;
    _iNumElementos = (sizeArrayIndices / sizeof(GLuint));

    glGenVertexArrays(1, &_iVao);
    glGenBuffers(1, &_iVbo);
    glGenBuffers(1, &_iEbo);

    glBindVertexArray(_iVao);
    {
        // Asociamos vertices a un array general de datos: posiciones, colores, coordenadas textura
        glBindBuffer(GL_ARRAY_BUFFER, _iVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeArrayVertices, aVertices, GL_STATIC_DRAW);

        // Asociamos indices a un array de elementos
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeArrayIndices, aIndices, GL_STATIC_DRAW);

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

    iVaoFinal = _iVao;
    //iVboFinal = _iVbo;
    iEboFinal = _iEbo;

    return 0;
}


int cBuffer::createNormalIndexBuffer(
    GLfloat* aVertices, int sizeArrayVertices,
    GLuint* aIndices, int sizeArrayIndices,
    GLuint& iVaoFinal,
    //GLuint& iVboFinal,
    GLuint& iEboFinal)
{
    _iVao = 0;
    _iVbo = 0;
    _iEbo = 0;

    _type = bufferType::tbufferNormalIndex;

    // _iNumElementos = (sizeArrayVertices / sizeof(GLfloat)) / 8;
    _iNumElementos = (sizeArrayIndices / sizeof(GLuint));

    glGenVertexArrays(1, &_iVao);
    glGenBuffers(1, &_iVbo);
    glGenBuffers(1, &_iEbo);

    glBindVertexArray(_iVao);
    {
        // Asociamos vertices a un array general de datos: posiciones, colores, coordenadas textura
        glBindBuffer(GL_ARRAY_BUFFER, _iVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeArrayVertices, aVertices, GL_STATIC_DRAW);

        // Asociamos indices a un array de elementos
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeArrayIndices, aIndices, GL_STATIC_DRAW);

        // Positions atributo
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Normals atributo
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // Text. Coordinates atributo
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    iVaoFinal = _iVao;
    iEboFinal = _iEbo;

    return 0;
}


int cBuffer::createColorTexIndexBuffer(
    GLfloat* aVertices, int sizeArrayVertices,
    GLuint* aIndices, int sizeArrayIndices,
    GLuint& iVaoFinal,
    GLuint& iEboFinal)
{
    _iVao = 0;
    _iVbo = 0;
    _iEbo = 0;

    _type = bufferType::tbufferNormalIndex;

    _iNumElementos = (sizeArrayIndices / sizeof(GLuint));

    glGenVertexArrays(1, &_iVao);
    glGenBuffers(1, &_iVbo);
    glGenBuffers(1, &_iEbo);

    glBindVertexArray(_iVao);
    {
        // Asociamos vertices a un array general de datos: posiciones, colores, coordenadas textura
        glBindBuffer(GL_ARRAY_BUFFER, _iVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeArrayVertices, aVertices, GL_STATIC_DRAW);

        // Asociamos indices a un array de elementos
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iEbo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeArrayIndices, aIndices, GL_STATIC_DRAW);

        // Atributo Posiciones de 3
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Atributo colores de 3
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // Atributo coordenadas texturas de 2
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    iVaoFinal = _iVao;
    iEboFinal = _iEbo;

    return 0;
}


int cBuffer::createCubeBuffer()
{
    _iVao = 0;
    _iVbo = 0;
    _iEbo = 0;

    _type = bufferType::tbufferCube;

    // Este tiene que ser fijo 36.
    // Pero no esta mal saber el origen del calculo final:
    _iNumElementos = (sizeof(skyboxVertices) / sizeof(GLfloat)) / 3;

    glGenVertexArrays(1, &_iVao);
    glGenBuffers(1, &_iVbo);

    glBindVertexArray(_iVao);
    {
        // Asociamos vertices a un array general de datos: posiciones, colores, coordenadas textura
        glBindBuffer(GL_ARRAY_BUFFER, _iVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

        // Positions atributo
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    return 0;
}
#pragma endregion


//--------------------------------------------------------------------------
// Use Buffers
//--------------------------------------------------------------------------
#pragma region use
//--------------------------------------------------------------------------
// :: glDrawArrays
//--------------------------------------------------------------------------
void cBuffer::use()
{
    if (_iVao > 0)
    {
        glBindVertexArray(_iVao);
        if (
            _type == bufferType::tbufferIndex ||
            _type == bufferType::tbufferModel ||
            _type == bufferType::tbufferNormalIndex ||
            _type == bufferType::tbufferColorIndex ||
            _type == bufferType::tbufferColorTexIndex
            )
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _iEbo);
            glDrawElements(GL_TRIANGLES, _iNumElementos, GL_UNSIGNED_INT, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, _iNumElementos);
        }
        glBindVertexArray(0);
    }
}


void cBuffer::draw()
{
    if (_iVao > 0)
    {
        if (
                _type == bufferType::tbufferIndex ||
                _type == bufferType::tbufferModel ||
                _type == bufferType::tbufferNormalIndex ||
                _type == bufferType::tbufferColorIndex ||
                _type == bufferType::tbufferColorTexIndex
            )
        {
            glDrawElements(GL_TRIANGLES, _iNumElementos, GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLES, 0, _iNumElementos);
        }
    }
}


void cBuffer::bind()
{
    if (_iVao > 0)
    {
        glBindVertexArray(_iVao);
    }
}


void cBuffer::unBind()
{
    glBindVertexArray(0);
}

//--------------------------------------------------------------------------
// :: glDrawArrays
//--------------------------------------------------------------------------
int cBuffer::useSimpleBuffer(GLuint iVao, int sizeofVerticesTriangles)
{
    if (iVao > 0 &&
        !(
            _type == bufferType::tbufferIndex ||
            _type == bufferType::tbufferModel ||
            _type == bufferType::tbufferNormalIndex ||
            _type == bufferType::tbufferColorIndex ||
            _type == bufferType::tbufferColorTexIndex)
        )
    {
        _iNumElementos = sizeofVerticesTriangles;
        glBindVertexArray(iVao);
        glDrawArrays(GL_TRIANGLES, 0, _iNumElementos);
        glBindVertexArray(0);
    }

    return 0;
}


//--------------------------------------------------------------------------
// Utilizamos Drawelements y no DrawArrays
// :: glDrawElements
//--------------------------------------------------------------------------
int cBuffer::useIndexBuffer(GLuint iVao, int sizeofIndices)
{
    if (iVao > 0 && 
        (
            _type == bufferType::tbufferIndex || 
            _type == bufferType::tbufferModel ||
            _type == bufferType::tbufferNormalIndex ||
            _type == bufferType::tbufferColorIndex ||
            _type == bufferType::tbufferColorTexIndex)
        )
    {
        _iNumElementos = sizeofIndices;
        glBindVertexArray(iVao);
        glDrawElements(GL_TRIANGLES, _iNumElementos, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    return 0;
}
#pragma endregion


//--------------------------------------------------------------------------
// Delete Buffers
//--------------------------------------------------------------------------
#pragma region Delete
int cBuffer::deleteSimpleBuffer(GLuint& iVao)
{
    if (iVao > 0)
    {
        glDeleteVertexArrays(1, &iVao);
    }

    return 0;
}


int cBuffer::deleteIndexBuffer(GLuint& iVao, GLuint& iEbo)
{
    glDeleteVertexArrays(1, &iVao);
    glDeleteBuffers(1, &iEbo);

    return 0;
}


void cBuffer::deleteBuffer()
{
    if (_iVao > 0)
    {
        glDeleteVertexArrays(1, &_iVao);
    }
    if (_iVbo > 0)
    {
        glDeleteBuffers(1, &_iVbo);
    }
    if (_iEbo > 0)
    {
        glDeleteBuffers(1, &_iEbo);
    }
}
#pragma endregion



/*========================================================================*\
|* Fin de Buffer.cpp
\*========================================================================*/
