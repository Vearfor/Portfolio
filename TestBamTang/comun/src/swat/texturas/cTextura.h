/*========================================================================*\
|* cTextura.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "../sOpenGL.h"
#include <dll/nExport.h>
#include <vector>
#include <string>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enumerados
//--------------------------------------------------------------------------
// El enumerado de eTipoTextura esta funcionando como un identificador
// de la tabla de tipos de textura que construimos en el cpp:
//  sTipoTextura cListaTipoTexturas::s_vtTipoTextura[] =
//  Las posiciones del Array (deberia ser mejor un vector), deben 
//  coincidir con los valores del enumerado
//--------------------------------------------------------------------------
enum class eOldTipoTextura
{
    eAT0 = 0,   //  0, AT0 Tex Bmp sin alpha
    eAT1,       //  1, AT1 Tex Bmp sin alpha
    eAT2,       //  2, AT2 Tex Bmp sin alpha
    eAT3,       //  3, AT3 Tex Bmp ?
    eAT4,       //  4 ,  Cook
    eFT0,       //  5, FT0 Tex Tga con alpha
    eFT1,       //  6, FT1 Tex Bmp sin alpha
    eFT2,       //  7, FT2 Tex Tga con alpha
    eSH0,       //  8, SH0
    eSH1,       //  9, SH1
    eFLR,       // 10, eFLR   Flare
    eENG,       // 11, eENG:  Particle Engine
    eLOG,       // 12, eLOG:  Logo de Inmize.
    eCOL0,      // 13,        Texturas de color.
    eTNoReg,    // Textura No Registrada: por defecto AT3, lo que se especifique
    NUM_TIPOS_TEXTURA
};
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
class _mExport cTextura
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

    //-------------------------------------------------------------------------
    // De Activa o Desactiva
    //-------------------------------------------------------------------------
    bool m_bActiveTex{ false };         // Esta ya activa la textura
    bool m_bActiveBlend{ false };       // Si textura color transparente, indica si esta activa para, activar o desactivar.
    //-------------------------------------------------------------------------

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

    //----------------------------------------------------------------------
    void	activa(bool activeSampler = false, GLenum unitTextura = GL_TEXTURE0);
    void	desActiva(void);
    bool	estaActivaTex2D(void);
    //----------------------------------------------------------------------

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