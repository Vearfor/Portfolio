/*========================================================================*\
|* cTextura.cpp
\*========================================================================*/


#include "cTextura.h"
#include "cShader.h"
#include "../tool/cTool.h"
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>
#include <iostream>


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cTextura::cTextura(std::string nombre, GLuint idTex, textureType type)
    : _nombreTextura(nombre)
    , _iTex(idTex)
    , _mytype(type)
    , _material(nullptr)
{
}


cTextura::~cTextura()
{
    // No podemos hacer deleteTexturas en el destructor, si vamos a utilizar
    // copias en las listas de texturas:
    // 
    // Al final si:
    deleteTextura();
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto de Funciones
//--------------------------------------------------------------------------
void cTextura::deleteTextura()
{
    if (_iTex > 0)
    {
        glDeleteTextures(1, &_iTex);
    }
}


void cTextura::setTextura(std::string nombre, GLuint idTex, textureType type)
{
    _nombreTextura = nombre;
    _iTex = idTex;
    _type = type;
}


cstatic sResultText cTextura::genTexturaSimple(int width, int height, int componentes, unsigned char* image, bool generateMipMaps)
{
    sResultText sTex;
    GLuint iTex;

    glGenTextures(1, &iTex);
    glBindTexture(GL_TEXTURE_2D, iTex);
    // Wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    if (generateMipMaps)
        glGenerateMipmap(GL_TEXTURE_2D);
    //SOIL_free_image_data(image);
    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    sTex._iTex = iTex;
    sTex._bytesPorPixelTextura = componentes;
    sTex._widthTextura = width;
    sTex._heightTextura = height;
    sTex._sizeTextura = sTex._widthTextura * sTex._heightTextura * sTex._bytesPorPixelTextura;

    return sTex;
}


cstatic sResultText cTextura::genTexturaMap(int width, int height, int componentes, unsigned char* image, bool generateMipMaps)
{
    sResultText sTex;
    GLuint iTex;

    glGenTextures(1, &iTex);
    glBindTexture(GL_TEXTURE_2D, iTex);
    // Wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if (generateMipMaps)
        glGenerateMipmap(GL_TEXTURE_2D);
    //SOIL_free_image_data(image);
    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    sTex._iTex = iTex;
    sTex._bytesPorPixelTextura = 3;
    sTex._widthTextura = width;
    sTex._heightTextura = height;
    sTex._sizeTextura = sTex._widthTextura * sTex._heightTextura * sTex._bytesPorPixelTextura;

    return sTex;
}


cstatic sResultText cTextura::genTexturaModel(int width, int height, int componentes, unsigned char* image, bool generateMipMaps)
{
    sResultText sTex;
    GLuint iTex;

    glGenTextures(1, &iTex);
    glBindTexture(GL_TEXTURE_2D, iTex);
    // Wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if (generateMipMaps)
        glGenerateMipmap(GL_TEXTURE_2D);
    //SOIL_free_image_data(image);
    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    sTex._iTex = iTex;
    sTex._bytesPorPixelTextura = 3;
    sTex._widthTextura = width;
    sTex._heightTextura = height;
    sTex._sizeTextura = sTex._widthTextura * sTex._heightTextura * sTex._bytesPorPixelTextura;

    return sTex;
}


int cTextura::genCubeMap(std::vector<const GLchar*> faces)
{
    struct stImagen
    {
        int width;
        int height;
        int componentes;
        unsigned char* image;
    };
    int iRes = 0;
    //int forceCanals = SOIL_LOAD_RGB;
    int forceCanals = STBI_rgb;
    std::vector<struct stImagen> imagenes;

    GLuint i;
    for (i = 0; i < faces.size() && !iRes; i++)
    {
        struct stImagen imagen;
        // imagen.image = SOIL_load_image(faces[i], &imagen.width, &imagen.height, &imagen.componentes, forceCanals);
        imagen.image = stbi_load(faces[i], &imagen.width, &imagen.height, &imagen.componentes, forceCanals);
        if (!imagen.image)
        {
            iRes = -1;
            std::cout << "\nError::genCubeMap::SOIL_load_image: [" << faces[i] << "]" << std::endl;
        }
        else
        {
            imagenes.push_back(imagen);
        }
    }

    if (!iRes)
    {
        if (i != 6)
        {
            iRes = -1;
            std::cout << "\nError::genCubeMap: El vecor de texturas debe ser de 6 para un Cube Map." << std::endl;
        }
        else
        {
            glGenTextures(1, &_iTex);
            glBindTexture(GL_TEXTURE_CUBE_MAP, _iTex);

            // Wrap
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            // Filter
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            for (i = 0; i < imagenes.size(); i++)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imagenes[i].width, imagenes[i].height, 0, GL_RGB, GL_UNSIGNED_BYTE, imagenes[i].image);
                // SOIL_free_image_data(imagenes[i].image);
                stbi_image_free(imagenes[i].image);
            }
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

            _mytype = textureType::TexCube;
        }

        imagenes.clear();
    }

    return iRes;
}


cstatic sResultText cTextura::cargaTextura(textureType type, const char* pcFichero, const char* directory, bool generateMipMaps)
{
    char texFile[LON_BUFF];

    mInicio(texFile);
    if (directory)
    {
        sprintf_s(texFile, sizeof(texFile), "%s/%s", directory, pcFichero);
    }
    else
    {
        strcpy_s(texFile, sizeof(texFile), pcFichero);
    }

    sResultText sTex;

    sTex._iTex = -1;
    int forceCanals;
    switch (type)
    {
        case TexSimple:
            // forceCanals = (int)SOIL_LOAD_RGBA;
            forceCanals = (int)STBI_rgb_alpha;
            break;
        case TexMap:
        case TexModel:
            // forceCanals = (int)SOIL_LOAD_RGB;
            forceCanals = (int)STBI_rgb;
            break;
        case TexCube:
            std::cout << "\nError::cargaTextura:: El TexCube se hace con cTextura::genCubeMap" << std::endl;
            return sTex;
        default:
            std::cout << "\nError::cargaTextura:: unknown type" << std::endl;
            return sTex;
    }

    int width, height, componentes;
    // unsigned char* image = SOIL_load_image(texFile, &width, &height, &componentes, forceCanals);
    unsigned char* image = stbi_load(texFile, &width, &height, &componentes, forceCanals);

    //invertImage(image, width, height, componentes);

    if (image)
    {
        switch (type)
        {
            case TexSimple:
                sTex = genTexturaSimple(width, height, componentes, image);
                break;
            case TexMap:
                sTex = genTexturaMap(width, height, componentes, image);
                break;
            case TexModel:
                sTex = genTexturaModel(width, height, componentes, image);
                break;
            default:
                std::cout << "\nError::cargaTextura:: unknown type" << std::endl;
                break;
        }
    }
    else
    {
        std::cout << "Error::cargaTextura::SOIL_load_image: [" << pcFichero << "]" << std::endl;
    }

    return sTex;
}


cstatic int cTextura::invertImage(unsigned char* imageData, int width, int height, int components)
{
    //int widthInBytes = width * components;      // 4
    int widthInBytes = width * 4;      // 4
    unsigned char* top = nullptr;
    unsigned char* bottom = nullptr;
    unsigned char tmp = 0;
    int halfHeight = height / 2;
    for (int iRow = 0; iRow < halfHeight; iRow++)
    {
        top = imageData + (iRow * widthInBytes);
        bottom = imageData + ((height - iRow - 1) * widthInBytes);
        for (int iCol = 0; iCol < widthInBytes; iCol++)
        {
            tmp = *top;
            *top = *bottom;
            *bottom = tmp;
            top++;
            bottom++;
        }
    }

    return 0;
}


int cTextura::useTextura(int texUnit, GLint samplerLoc, int valorSampler)
{
    if (_iTex > 0)
    {
        if (texUnit > -1 && texUnit < 32)
        {
            glActiveTexture(GL_TEXTURE0 + texUnit);
            if (_mytype == textureType::TexCube)
            {
                glBindTexture(GL_TEXTURE_CUBE_MAP, _iTex);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, _iTex);
            }
            cShader::SetUniform(samplerLoc, valorSampler);
        }
    }
    return 0;
}


int  cTextura::useTextura(int texUnit, cShader* pShader, const char* nameUniform, int valorSampler)
{
    if (_iTex > 0)
    {
        if (texUnit > -1 && texUnit < 32)
        {
            glActiveTexture(GL_TEXTURE0 + texUnit);
            if (_mytype == textureType::TexCube)
            {
                glBindTexture(GL_TEXTURE_CUBE_MAP, _iTex);
            }
            else
            {
                glBindTexture(GL_TEXTURE_2D, _iTex);
            }
            GLint iLoc = pShader->getLocation(nameUniform);
            cShader::SetUniform(iLoc, valorSampler);
        }
    }
    return 0;
}


void cTextura::getLocations(cShader* shader)
{
    if (!_nameLoc.empty())
    {
        _texLoc = shader->getLocation(_nameLoc.c_str());
    }
}


void cTextura::unbind(GLuint texUnit)
{
    glActiveTexture(GL_TEXTURE0 + texUnit);
    if (_mytype == textureType::TexCube)
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

//--------------------------------------------------------------------------
void cTextura::setMaterial(cMaterial* material)
{
    _material = material;
}


cMaterial* cTextura::getMaterial()
{
    return _material;
}
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cTextura.cpp
\*========================================================================*/
