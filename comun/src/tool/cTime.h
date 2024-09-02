/*------------------------------------------------------------------------*\
|* cTime.h
\*------------------------------------------------------------------------*/

#pragma once
/*------------------------------------------------------------------------*\
|* Includes
\*------------------------------------------------------------------------*/
#include <stdint.h>
#include <chrono>
/* -----------------------------------------------------------------------*/

//--------------------------------------------------------------------------
// Defines
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
 

//--------------------------------------------------------------------------
// using
//--------------------------------------------------------------------------
using uNano = std::chrono::duration<uint64_t, std::nano>;
using uMicro = std::chrono::duration<uint64_t, std::micro>;
using uMilis = std::chrono::duration<uint64_t, std::milli>;

#ifdef _NORMAL_TIME
using uClk = std::chrono::steady_clock;
// using uClk = std::chrono::system_clock;
#else
using uClk = std::chrono::high_resolution_clock;
#endif  // _NORMAL_TIME

using uTimePoint = std::chrono::time_point<uClk, uNano>;
//--------------------------------------------------------------------------
// using uTimePoint = std::chrono::system_clock::time_point;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Lambdas de conversiones directas:
// inline c++17.
//--------------------------------------------------------------------------
inline auto fromNanoToMiliSeconds = [](double nanoSegundos)
{
    double nano = nanoSegundos;
    double milis = nano / 1000000.0;
    return milis;
};

inline auto fromMiliToNano = [](int64_t miliSegundos)
{
    int64_t milis = miliSegundos;
    int64_t nanos = milis * 1000000;
    return nanos;
};
//--------------------------------------------------------------------------


class cTime
{
    float m_fps{ 30 };
    int64_t m_milPerFrame{ int64_t((1 / m_fps) * 1000) };

#ifdef _CRONO_
    uTimePoint m_lastTime{};
    uTimePoint m_currentTime{};
#else
    int64_t m_lastTime{0};
    int64_t m_currentTime{0};
#endif // CRONO

    int64_t m_deltaTime{ 0 };

public:

    cTime(float m_fps);

#ifdef _CRONO_
    static uTimePoint getTime();
#else
    static int64_t getTime();
#endif
    void espera(bool espera = true);
    void sin_espera();
    float getDeltaTime();
    int64_t getMilPerFrame() { return m_milPerFrame; }
};

/*------------------------------------------------------------------------*\
|* Fin de cTime.h
\*------------------------------------------------------------------------*/
