/*========================================================================*\
|* sLaberinto_1.cpp
\*========================================================================*/

#include "sLaberinto_1.h"
#include "cLog.h"
#include "nComun.h"
#include <cstring>
#include <cctype>


sLaberinto_1::sLaberinto_1()
    : sLaberinto("Laberinto 1")
{
    cLog::print(" quien: %s\n", m_quienSoy.c_str());
}

sLaberinto_1::~sLaberinto_1()
{
}


int sLaberinto_1::creaLaberinto()
{
    return 0;
}


int sLaberinto_1::creaLaberintoFrame()
{
    return 0;
}


int sLaberinto_1::calculaCaminoMasLargo()
{
    return 0;
}


void sLaberinto_1::ramdomize(int* array, int n)
{
    for (int i = n - 1; i > 0; --i)
    {
        int j = int(m_motor.aleatorio(0, static_cast<double>(i + 1)));
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

bool sLaberinto_1::isValidPosition(sPos pos, int alto, int ancho)
{
    return pos.m_fila >= 1 && pos.m_columna >= 1 && pos.m_fila < alto - 1 && pos.m_columna < ancho - 1;
}


void sLaberinto_1::setHole(sPos pos)
{
    for (int i = 0; i < m_alto_celda; i++)
    {
        for (int j = 0; j < m_ancho_celda; j++)
        {
            m_matriz[pos.m_fila + i][pos.m_columna + j] = kVacio;
        }
    }
}


/*========================================================================*\
|* Fin de sLaberinto_1.cpp
\*========================================================================*/
