/*------------------------------------------------------------------------*\
|* cTime.cpp
\*------------------------------------------------------------------------*/

#include "cTime.h"
#include <Windows.h>

cTime::cTime(float fps) : m_fps(fps)
{
    m_milPerFrame = int64_t((1 / m_fps) * 1000);
    m_lastTime = getTime();
}

#ifdef _CRONO_
uTimePoint cTime::getTime()
{
    // Con este 'now' la diferencia esta en segundos:
    // auto myTime = std::chrono::system_clock::now();
    // y con este en nanos
    auto myTime = uClk::now();
#else
int64_t cTime::getTime()
{
    auto myTime = SDL_GetTicks();
#endif
    return myTime;
}

// Forzamos que devuelva en segundos
// Es mas directo luego el calculo
float cTime::getDeltaTime()
{
    float deltaTime = (float)m_deltaTime / 1000;

    return deltaTime;
}


void cTime::espera(bool delayed)
{
    m_currentTime = getTime();
    auto elapsed = m_currentTime - m_lastTime;
#ifdef _CRONO_
    m_deltaTime = (int64_t) fromNanoToMiliSeconds((double)elapsed.count());
#else
    m_deltaTime = elapsed;
#endif
    if (delayed)
    {
        // Hay que esperar mas:
        int64_t nelapsed = m_milPerFrame - m_deltaTime;
        if (nelapsed > 0)
            Sleep((DWORD)nelapsed);
    }
    m_lastTime = m_currentTime;
}


void cTime::sin_espera()
{
    m_currentTime = getTime();
    auto elapsed = m_currentTime - m_lastTime;
#ifdef _CRONO_
    m_deltaTime = (int64_t)fromNanoToMiliSeconds((double)elapsed.count());
#else
    m_deltaTime = elapsed;
#endif
    m_lastTime = m_currentTime;
}


/*------------------------------------------------------------------------*\
|* Fin de cTime.h
\*------------------------------------------------------------------------*/
