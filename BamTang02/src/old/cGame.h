/*------------------------------------------------------------------------*\
|*
|* cGame.h
|*
\*------------------------------------------------------------------------*/
#pragma once
/*------------------------------------------------------------------------*\
|* Includes
\*------------------------------------------------------------------------*/
#include <SDL3/Sdl.h>
/* ------------------------------------------------------------------------*/

//class cRenderer;
//class cInputHandler;
//class cKeyboard;
//class cTextura;
//class cAnimacion;
//class cRenderObject;
//class cKnight;
class cGame
{
public:

    ~cGame();

    int init();
    bool isRunning();
    int eventos();
    int update(float deltaTime);
    int render();
    int release();

private:

    //cInputHandler* m_pHandler{ nullptr };
    //cKeyboard* m_pKeyboard{ nullptr };

    //SDL_Window* m_window{ nullptr };
    //SDL_Renderer* m_renderer{ nullptr };

    bool m_isRunning = false;
    int64_t m_numFrames{ 0 };

    //int initSDL();
    //int CreateWindow(SDL_Window** window, SDL_Renderer** renderer);
    //int initKeyboard();

};

/*------------------------------------------------------------------------*\
|* Fin de cGame.h
\*------------------------------------------------------------------------*/
#pragma once
