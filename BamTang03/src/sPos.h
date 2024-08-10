/*========================================================================*\
|* sPos.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include <string>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enumerados
//--------------------------------------------------------------------------
enum class eSentido : int8_t
{
    eNone   = -1,
    eTop    = 0,
    eRight  = 1,
    eBot    = 2,
    eLeft   = 3
};
//--------------------------------------------------------------------------


//
// Mejor Una estructura
//
struct sPos
{
    sPos() {}

    sPos(int x, int y)
        : m_fila(x)
        , m_columna(y)
    {
    }

    sPos(int x, int y, eSentido sentido, const char * pcMesg, int index, bool visitada, int pasos)
        : m_fila(x)
        , m_columna(y)
        , m_sentido(sentido)
        , m_nombre(pcMesg)
        , m_index(index)
        , m_visitada(visitada)
        , m_num_pasos(pasos)
    {
    }

    int m_fila{ 0 };
    int m_columna{ 0 };
    eSentido m_sentido{ eSentido::eTop };
    std::string m_nombre{"Arriba"};
    int m_index{ 0 };
    bool m_visitada{ false };
    int m_num_pasos{ 0 };
};


/*========================================================================*\
|* Fin de sPos.h
\*========================================================================*/
