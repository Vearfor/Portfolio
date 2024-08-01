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


void sLaberinto_1::ramdomize(int* array, int n) 
{
    for (int i = n - 1; i > 0; --i)
    {
        // int j = rand() % (i + 1);
        int j = int(m_motor.aleatorio(0, i + 1));
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

bool sLaberinto_1::isValidPosition(sCoord cur, int alto, int ancho)
{
    // no se consideran loa bordes
    return cur.fila >= 1 && cur.columna >= 1 && cur.fila < alto - 1 && cur.columna < ancho - 1;
}


void sLaberinto_1::removeWall(sCoord cur, int ancho_celda, int alto_celda)
{
    for (int i = 0; i < alto_celda; i++)
    {
        for (int j = 0; j < ancho_celda; j++)
        {
            m_matriz[cur.fila + i][cur.columna + j] = kVacio;
        }
    }
}


void sLaberinto_1::copyRow(int row_src, int row_dest, int ancho)
{
    for (int j = 0; j < ancho; j++)
    {
        m_matriz[row_dest][j] = m_matriz[row_src][j];
    }
}


void sLaberinto_1::copyCol(int col_src, int col_dest, int alto)
{
    for (int i = 0; i < alto; i++)
    {
        m_matriz[i][col_dest] = m_matriz[i][col_src];
    }
}

void sLaberinto_1::copyValidRowsAndCols(int alto, int ancho)
{
    int i = alto - 2;
    while (i >= 0 && m_matriz[i][1] == kMuro) --i;
    while (i < alto - 2)
    {
        copyRow(i, i + 1, ancho);
        i++;
    }
    int j = ancho - 2;
    while (j >= 0 && m_matriz[1][j] == kMuro) --j;
    while (j<ancho-2)
    {
        copyCol(j, j + 1, alto);
        j++;
    }
}


/*========================================================================*\
|* Fin de sLaberinto_1.cpp
\*========================================================================*/
