/*========================================================================*\
|* sPos.cpp
|*
|* Posicion, direccion, celda del laberinto
\*========================================================================*/

#include "sPos.h"
#include "nComun.h"


/*------------------------------------------------------------------------*\
|* Constructores
\*------------------------------------------------------------------------*/
sPos::sPos()
{
}

sPos::sPos(int x, int y)
    : m_fila(x)
    , m_columna(y)
{
}

sPos::sPos(int x, int y, eSentido sentido, const char* pcMesg, int index, bool visitada, int pasos, int tecla)
    : m_fila(x)
    , m_columna(y)
    , m_sentido(sentido)
    , m_nombre(pcMesg)
    , m_index(index)
    , m_visitada(visitada)
    , m_num_pasos(pasos)
    , m_tecla(tecla)
{
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Metodos
\*------------------------------------------------------------------------*/
cstatic eSentido sPos::getSentidoAnterior(int tecla)
{
    eSentido sentAnterior = eSentido::eNone;
    switch (tecla)
    {
    case 'W': sentAnterior = eSentido::eBot;    break;
    case 'D': sentAnterior = eSentido::eLeft;   break;
    case 'S': sentAnterior = eSentido::eTop;    break;
    case 'A': sentAnterior = eSentido::eRight;  break;
    default:    break;
    }
    return sentAnterior;
}


/*========================================================================*\
|* Fin de sPos.h
\*========================================================================*/
