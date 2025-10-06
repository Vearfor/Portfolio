/*========================================================================*\
|* sPos.h
|*
|* Posicion, direccion, celda del laberinto
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
    eTop    = 0,        //                   top                        //
    eRight  = 1,        //   left            +              right       //
    eBot    = 2,        //                   bottom                     //
    eLeft   = 3
};
//--------------------------------------------------------------------------


//
// Mejor Una estructura
//
struct sPos
{
    sPos();
    sPos(int x, int y);
    sPos(int x, int y, eSentido sentido, const char* pcMesg, int index, bool visitada, int pasos, int tecla);

    int m_fila{ 0 };
    int m_columna{ 0 };
    eSentido m_sentido{ eSentido::eTop };
    std::string m_nombre{ "Arriba" };
    int m_index{ 0 };
    bool m_visitada{ false };
    int m_num_pasos{ 0 };
    int m_tecla{ 32 };

    static eSentido getSentidoAnterior(int tecla);
};


/*========================================================================*\
|* Fin de sPos.h
\*========================================================================*/
