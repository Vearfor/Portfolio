/*========================================================================*\
|* cTextura.h
\*========================================================================*/


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <GL/glew.h>
#include <assimp/Importer.hpp>
#include <vector>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enum
//--------------------------------------------------------------------------
enum textureType
{
    TexSimple = 0,
    TexMap,
    TexModel,
    TexCube,
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// struct para devolver en las funciones de static que devuelven
// la textura
//--------------------------------------------------------------------------
struct sResultText
{
    GLint _iTex{ 0 };
    size_t _sizeTextura{ 0 };
    int _widthTextura{ 0 };
    int _heightTextura{ 0 };
    int _bytesPorPixelTextura{ 0 };
};


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cShader;
class cMaterial;
//--------------------------------------------------------------------------
class cTextura
{
    textureType _mytype = textureType::TexSimple;

    static sResultText genTexturaSimple(int width, int height, int componentes, unsigned char* image, bool generateMipMaps = true);
    static sResultText genTexturaMap(int width, int height, int componentes, unsigned char* image, bool generateMipMaps = true);
    static sResultText genTexturaModel(int width, int height, int componentes, unsigned char* image, bool generateMipMaps = true);
    static int invertImage(unsigned char* imageData, int width, int height, int components);

    void setTextura(std::string nombre, GLuint idTex, textureType type);

    std::string _nombreTextura;
    GLuint _iTex = 0;
    std::string _type = "";

    cMaterial* _material;

    // Debería de borrarlas ?
    aiString _path;
    std::string _nameLoc = "";
    GLint _texLoc = -2;
    
    size_t _sizeTextura{ 0 };
    int _widthTextura{ 0 };
    int _heightTextura{ 0 };
    int _bytesPorPixelTextura{ 0 };

public:

    cTextura(std::string nombre = "", GLuint idTex = 0, textureType type = textureType::TexSimple);
    ~cTextura();

    static sResultText cargaTextura(textureType type, const char* pcFichero, const char* directory = nullptr, bool generateMipMaps = true);

    int genCubeMap(std::vector<const GLchar*> faces);

    void deleteTextura();
    int useTextura(int texUnit, GLint samplerLoc, int valorSampler);
    int useTextura(int texUnit, cShader* pShader, const char* nameUniform, int valorSampler);
    void getLocations(cShader*);
    void unbind(GLuint texUnit = 0);

    void setMaterial(cMaterial*);
    cMaterial* getMaterial();

    int getIdTex() { return _iTex; }
    const char* getNombre() { return _nombreTextura.c_str(); }

    size_t getSize() const { return _sizeTextura; }
    int getWidth() const { return _widthTextura; }
    int getHeight() const { return _heightTextura; }
    int getBytesPorPixel() const { return _bytesPorPixelTextura; }

    void setSize(size_t valor) { _sizeTextura = valor;}
    void setWidth(int valor) { _widthTextura = valor; }
    void setHeight(int valor) { _heightTextura = valor; }
    void setBytesPorPixel(int valor) { _bytesPorPixelTextura = valor; }
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cTextura.h
\*========================================================================*/