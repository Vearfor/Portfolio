/*------------------------------------------------------------------------*\
|*
|* sGame.h
|*
\*------------------------------------------------------------------------*/
#pragma once
/*------------------------------------------------------------------------*\
|* Includes
\*------------------------------------------------------------------------*/
#include <cstdint>
/*------------------------------------------------------------------------*/

/*------------------------------------------------------------------------*/
struct sMyMaze;
struct sVistaConsola;
//struct sVistaSDL;
struct sVista3D;
struct sRenderSystem;
/*------------------------------------------------------------------------*/
struct sGame
{
private:

    bool m_isRunning = false;
    int64_t m_numFrames{ 0 };

    sMyMaze* m_pLaberinto{ nullptr };
    sVistaConsola* m_pVistaConsola{ nullptr };
    //sVistaSDL* m_pVistaSDL{ nullptr };
    sVista3D* m_pVista3D{ nullptr };

    sRenderSystem* m_pRender{ nullptr };
    //cInputHandler* m_pHandler{ nullptr };
    //cKeyboard* m_pKeyboard{ nullptr };

    //SDL_Window* m_window{ nullptr };
    //SDL_Renderer* m_renderer{ nullptr };

    //cTextura* m_texPruebaLetra{ nullptr };
    //SDL_FRect m_desLetra;

    //cTextura* m_texLogo{ nullptr };
    //SDL_FRect m_desLogo;

    //cTextura* m_texKnight{ nullptr };
    //SDL_FRect m_desKnight;

    //cAnimacion* m_animKnight{ nullptr };
    //SDL_FRect m_desAnim;

    //SDL_FRect m_desRotate;

    //cKnight* m_hero{ nullptr };

    //int initSDL();
    //int CreateWindow(/*SDL_Window** window, SDL_Renderer** renderer*/);
    //int initKeyboard();

public:

    ~sGame();

    int init();
    bool isRunning();
    int eventos();
    int update(float deltaTime);
    int render();
};

/*------------------------------------------------------------------------*\
|* Fin de sGame.h
\*------------------------------------------------------------------------*/
