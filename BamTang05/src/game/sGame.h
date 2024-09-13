/*------------------------------------------------------------------------*\
|*
|* sGame.h
|*
\*------------------------------------------------------------------------*/
#pragma once
/*------------------------------------------------------------------------*\
|* Includes
\*------------------------------------------------------------------------*/
#include "sGlobal.h"
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
    bool m_hayDemo{ false };
    float m_ftimeDemo{ 10.0f * sGlobal::m_fIntervaloDemo };

    int playingDemo(float fDeltaTime);
    void playAction();

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
    void demo(bool hayDemo);
    sRenderSystem* getRender();
    //bool isPlayingDemo() { return m_isPlayingDemo; }
    //void togglePausa() { m_hayPausa = !m_hayPausa; }
    //bool hayPausa() { return m_hayPausa; }
};

/*------------------------------------------------------------------------*\
|* Fin de sGame.h
\*------------------------------------------------------------------------*/
