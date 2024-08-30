/*========================================================================*\
|* sVista3D.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include "sVista.h"
#include "../tool/iMensajes.h"
#include "../swat/sOpenGL.h"
#include "../swat/windows/cMonitor.h"
#include <cstdint>
#include <string>
#include <GLM/gtc/matrix_transform.hpp>
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Constantes
//--------------------------------------------------------------------------
// por ahora asi, no se me ocurre una forma mejor de mejorarlo
//--------------------------------------------------------------------------
const std::string kTexSuelo = "grid.jpg";
const std::string kTexMuro = "wooden_crate.jpg";
const std::string kTexMuro2 = "Bumps.bmp";
const std::string kTexLetraA = "letraA_negro.png";
const std::string kTexLetraB = "letraB_negro.png";
const std::string kTexPunto = "punto_marca_00.png";
const std::string kTexCamara = "marca_circulo.png";
const std::string kDirTexturas = "../comun/textures";
//--------------------------------------------------------------------------
const std::string kNombreShader = "MainShader";
const std::string kVertexFile = "vertex_cubo.glsl";
const std::string kFragmentFile = "fragment_cubo.glsl";
const std::string kDirShaders = "../comun/shaders";
//--------------------------------------------------------------------------
const std::string kModel = "model";
const std::string kView = "view";
const std::string kProjection = "projection";
const std::string kTexSampler1 = "texSampler1";
//--------------------------------------------------------------------------
const uint16_t kWidth = 800;                // 1024;
const uint16_t kHeight = 600;               // 768;
const uint16_t kCelda = 1;                  //
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Struct
//--------------------------------------------------------------------------
struct sWindow;
class cMalla;
class cTextura;
class cShader;
class cCamara;
//--------------------------------------------------------------------------
struct sVista3D
    : public sVista
    , public iMensajes
{
    sVista3D();
    ~sVista3D();

    virtual int inicia(sLaberinto* lab) override;
    virtual int eventos() override;
    virtual int update() override;
    virtual int render() override;

    int creaWindow(int width, int height);
    int renderLaberinto();
    int renderCubo();

    int lanzaLaberintoFrame();
    int rederLaberintoFrame();

    cCamara* m_pCamara{ nullptr };
    sWindow* m_mainWindow{ nullptr };
    cMalla* m_pCubo{ nullptr };
    cMalla* m_pCuadro{ nullptr };
    cMalla* m_pTriangulo{ nullptr };

    // El gestor se encargara de borrar la memoria de estas texturas.
    cTextura* m_pTexSuelo{ nullptr };
    cTextura* m_pTexMuro{ nullptr };
    cTextura* m_pTexMuro2{ nullptr };
    cTextura* m_pTexLetraA{ nullptr };
    cTextura* m_pTexLetraB{ nullptr };
    cTextura* m_pTexPunto{ nullptr };
    cTextura* m_pTexCamara{ nullptr };

    // El gestor se encargara de borrar la memoria de los shaders
    cShader* m_pMainShader{ nullptr };

    GLint m_loc_model{ -1 };
    GLint m_loc_view{ -1 };
    GLint m_loc_projection{ -1 };
    GLint m_loc_texSampler1{ -1 };

    glm::mat4 m_model{ 1.0 };
    glm::mat4 m_view{ 1.0 };
    glm::mat4 m_perspProjection{ 1.0 };
    glm::mat4 m_orthoProjection{ 1.0 };

    // Habra que generar elementos distintos por cada posicion
    // - dependeran del laberinto.

    // Cube and floor positions
    glm::vec3 m_cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 m_floorPos = glm::vec3(0.0f, 0.0f, 0.0f);

    //--------------------------------------------------------------------------
    void calculaRect(int fila, int columna, int size, cRect<float>* pOutRect);
    void renderChar(char car, cRect<float>* pRectDest);
    //----------------------------------------------------------------------
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de sVista3D.h
\*========================================================================*/
