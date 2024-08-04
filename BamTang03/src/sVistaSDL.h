/*========================================================================*\
|* sVistaSDL.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include "sVista.h"
#include <cstdint>
#include <string>
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Constantes
//--------------------------------------------------------------------------
const uint16_t kWidth = 800;                // 1024;
const uint16_t kHeight = 600;               // 768;
const uint16_t kCelda = 25;                 //
//--------------------------------------------------------------------------
const std::string kPathLetraA = "../BamTang03/recursos/letraA_negro.png";
const std::string kPathLetraB = "../BamTang03/recursos/letraB_negro.png";
const std::string kPathMuro = "../BamTang03/recursos/muro.png";
const std::string kPathVacio = "../BamTang03/recursos/negro.png";
const std::string kPathPunto = "../BamTang03/recursos/punto_negro.png";
const std::string kPathPuntoAzul = "../BamTang03/recursos/punto_marca.png";
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
// Struct
//--------------------------------------------------------------------------
struct SDL_Window;
struct SDL_Renderer;
class cTextura;
struct SDL_FRect;
struct sRenderObject;
struct sLaberinto;
//--------------------------------------------------------------------------
struct sVistaSDL
    : public sVista
{
    SDL_Window* m_pWindow{ nullptr };
    SDL_Renderer* m_pRenderer{ nullptr };

    sLaberinto* m_sLaberinto{ nullptr };

    // Hay que tener en cuenta que lo estamos haciendo mal, hay que separar de las vistas
    // los contenidos/datos centrales del Laberinto
    sRenderObject* m_pPunto{ nullptr };
    // El render object sera el personaje que esta dentro del laberinto y debería estar fuera de la vista !!!

    cTextura* m_pLetraA{ nullptr };
    cTextura* m_pLetraB{ nullptr };
    cTextura* m_pMuro{ nullptr };
    cTextura* m_pVacio{ nullptr };
    cTextura* m_pMarca{ nullptr };

    sVistaSDL();
    ~sVistaSDL();

    virtual int inicia(sLaberinto* lab) override;
    virtual int update() override;
    virtual int dibuja(sLaberinto* lab) override;
    virtual int mainLoop(sLaberinto* lab) override;

    int initSDL();
    int createWindow(SDL_Window** pWindow, SDL_Renderer** pRenderer);
    int libera();

    bool eventos();
    void dibujaChar(char car, SDL_FRect* pRectDest);
    void calculaRect(int fila, int columna, int size, SDL_FRect* pOutRect);

    void laVentanaGanaFoco();
    void laVentanaPierdeFoco();
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de sVistaSDL.h
\*========================================================================*/
