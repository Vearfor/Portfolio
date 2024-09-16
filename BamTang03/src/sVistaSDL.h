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
const float kIntervaloDemo = 0.25f;         //
//--------------------------------------------------------------------------
// const std::string kPathLetraA = "../BamTang03/recursos/letraA_negro.png";
//--------------------------------------------------------------------------
const std::string kPathLetraA = "../BamTang03/recursos/letraA_negro.png";
const std::string kPathLetraB = "../BamTang03/recursos/letraB_negro.png";
const std::string kPathMuro = "../BamTang03/recursos/muro.png";
const std::string kPathVacio = "../BamTang03/recursos/negro.png";
const std::string kPathPunto = "../BamTang03/recursos/punto_negro.png";
//--------------------------------------------------------------------------
const std::string kPathPuntoAmarillo = "../BamTang03/recursos/marca_circulo.png";
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

    cTextura* m_pLetraA{ nullptr };
    cTextura* m_pLetraB{ nullptr };
    cTextura* m_pMuro{ nullptr };
    cTextura* m_pVacio{ nullptr };
    cTextura* m_pMarca{ nullptr };

    bool m_hayDemo{ false };
    float m_ftimeDemo{ 10.0f * kIntervaloDemo };

    bool m_hemosLlegado{ false };

    sVistaSDL();
    ~sVistaSDL();

    virtual int inicia(sLaberinto* lab) override;
    virtual int update(float fDeltaTime) override;
    virtual int dibuja(sLaberinto* lab) override;
    virtual int mainLoop(sLaberinto* lab) override;

    int initSDL();
    int createWindow(SDL_Window** pWindow, SDL_Renderer** pRenderer);
    int libera();

    bool eventos();
    void dibujaChar(char car, SDL_FRect* pRectDest);
    void calculaRect(int fila, int columna, int size, SDL_FRect* pOutRect);

    void OnSetFocus();
    void OnKillFocus();

    int playingDemo(float fDeltaTime);
    void playAction();
    int demo(bool hayDemo);
    void controlFin();
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de sVistaSDL.h
\*========================================================================*/
