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
struct sRenderSystem;
/*------------------------------------------------------------------------*/
struct sGame
{
private:

    bool m_isRunning = false;
    int64_t m_numFrames{ 0 };

    sMyMaze* m_pLaberinto{ nullptr };

    sRenderSystem* m_pRender{ nullptr };

    //cInputHandler* m_pHandler{ nullptr };
    //cKeyboard* m_pKeyboard{ nullptr };
    //int initKeyboard();

public:

    sGame();
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
