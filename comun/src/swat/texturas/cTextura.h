/*========================================================================*\
|* cTextura.h
\*========================================================================*/


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "../sOpenGL.h"
#include <vector>
#include <string>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enumerado
//--------------------------------------------------------------------------
enum class textureType
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
    static sResultText genTexturaSimple(int width, int height, int componentes, unsigned char* image, bool generateMipMaps = true);
    static sResultText genTexturaMap(int width, int height, int componentes, unsigned char* image, bool generateMipMaps = true);
    static sResultText genTexturaModel(int width, int height, int componentes, unsigned char* image, bool generateMipMaps = true);
    static int invertImage(unsigned char* imageData, int width, int height, int components);

    void setTextura(std::string nombre, GLuint idTex, textureType type);

    std::string m_nombreTextura;
    GLuint m_iTex = 0;
    textureType m_mytype = textureType::TexSimple;
    cMaterial* m_material;

    std::string m_nameLoc = "";
    GLint m_texLoc = -2;
    
    size_t m_sizeTextura{ 0 };
    int m_widthTextura{ 0 };
    int m_heightTextura{ 0 };
    int m_bytesPorPixelTextura{ 0 };

public:

    cTextura(std::string nombre = "", GLuint idTex = 0, textureType type = textureType::TexSimple);
    ~cTextura();

    static sResultText cargaTextura(textureType type, const char* pcFichero, const char* directory = nullptr, bool generateMipMaps = true);

    int genCubeMap(std::vector<const GLchar*> faces);

    void deleteTextura();
    int useTextura(int texUnit, GLint samplerLoc = 0, int valorSampler = 0);
    int useTextura(int texUnit, cShader* pShader, const char* nameUniform, int valorSampler);
    void getLocations(cShader*);
    void unbind(GLuint texUnit = 0);

    void setMaterial(cMaterial*);
    cMaterial* getMaterial();

    int getIdTex() { return m_iTex; }
    const char* getNombre() { return m_nombreTextura.c_str(); }

    size_t getSize() const { return m_sizeTextura; } 
    int getWidth() const { return m_widthTextura; }
    int getHeight() const { return m_heightTextura; }
    int getBytesPorPixel() const { return m_bytesPorPixelTextura; }

    void setSize(size_t valor) { m_sizeTextura = valor;}
    void setWidth(int valor) { m_widthTextura = valor; }
    void setHeight(int valor) { m_heightTextura = valor; }
    void setBytesPorPixel(int valor) { m_bytesPorPixelTextura = valor; }

    void setNameLoc(std::string& sNameLoc) { m_nameLoc = sNameLoc; }
    std::string getNameLoc() { return m_nameLoc; }
    GLint getTexLoc() { return m_texLoc; }
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cTextura.h
\*========================================================================*/