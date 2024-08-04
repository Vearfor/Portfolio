/*========================================================================*\
|* sLaberinto_2.cpp
\*========================================================================*/

#include "sLaberinto_2.h"
#include "cLog.h"
#include "nComun.h"
#include <cstring>
#include <cctype>
#include <unordered_map>


sLaberinto_2::sLaberinto_2()
    : sLaberinto("Laberinto 2")
{
    cLog::print(" quien: %s\n", m_quienSoy.c_str());
}

sLaberinto_2::~sLaberinto_2()
{
}


sPos vDir[4] =
{
    { -1,  0, eSentido::eTop    , "Arriba"      , 0 , false, 0 },
    {  0,  1, eSentido::eRight  , "Derecha"     , 1 , false, 0 },
    {  1,  0, eSentido::eBot    , "Abajo"       , 2 , false, 0 },
    {  0, -1, eSentido::eLeft   , "Izquierda"   , 3 , false, 0 },
};


int sLaberinto_2::creaLaberinto()
{
    sPos posInicial(1, 1);
    sPos posFinal(m_size - 2, m_size - 2);

    int ancho_celda = 1;
    int alto_celda = 1;

    int sizeTotal = m_size * m_size;

    sCoord* lab = new sCoord[sizeTotal];
    int xoffset = 1 + ancho_celda;
    int yoffset = 1 + alto_celda;

    int index = 0;
    sCoord start = { 1, 1, eSentido::eNone };

    lab[index++] = start;

    while (index)
    {
        sCoord cur = lab[--index];
        int fila = cur.fila;
        int columna = cur.columna;
        if (m_matriz[fila][columna] != kMuro)
            continue;

        // Elmina la pared entre dos posiciones validas
        if (cur.sentido != eSentido::eNone)
        {
            int dx = vDir[static_cast<int>(cur.sentido)].m_fila;
            int dy = vDir[static_cast<int>(cur.sentido)].m_columna;
            if (
                    cur.sentido == eSentido::eLeft ||     // 3
                    cur.sentido == eSentido::eTop         // 0
               )
            {
                dx *= (xoffset - 1);
                dy *= (yoffset - 1);
            }
            removeWall({ fila - dx, columna - dy }, 1, 1);
        }
        removeWall(cur, 1, 1);

        int idx[4] = { int(eSentido::eTop), int(eSentido::eRight), int(eSentido::eBot), int(eSentido::eLeft) };
        ramdomize(idx, 4);

        for (int k = 0; k < 4; k++)
        {
            int dx = vDir[idx[k]].m_fila;
            int dy = vDir[idx[k]].m_columna;
            int nx = fila + xoffset * dx;
            int ny = columna + yoffset * dy;

            sCoord ul, lr;
            ul = { min(fila,nx), min(columna, ny) };
            lr =
            {
                max(fila,nx) + alto_celda - 1,
                min(columna, ny) + ancho_celda -1
            };

            if (
                    isValidPosition(ul, m_size, m_size) &&
                    isValidPosition(lr, m_size, m_size) &&
                    m_matriz[nx][ny] == kMuro
               )
            {
                lab[index++] = { nx, ny,  static_cast<eSentido>(idx[k]) };
            }
        }
    }

    delete [] lab;

    // Empezamos en inicio: 'A':
    m_current = { 1, 1, eSentido::eNone, "Inicio", 0, false, 0 };
    m_vecPos.push_back(m_current);
    while (m_vecPos.size() > 0)
    {
        calculaCaminoMasLargo();
    }

    for (int fila = 0; fila < m_size; fila++)
    {
        for (int columna = 0; columna < m_size; columna++)
        {
            if (m_matriz[fila][columna] != kMuro)
                m_matriz[fila][columna] = kVacio;
        }
    }

    // El primero se marca como inicio 'A'.
    m_matriz[1][1] = kInicio;

    //m_last.m_fila = m_size - 2;
    //m_last.m_columna = m_size - 2;
    m_matriz[m_last.m_fila][m_last.m_columna] = kFin;

    return 0;
}


int sLaberinto_2::calculaCaminoMasLargo()
{
    if (m_vecPos.size() > 0)
    {
        int len = sizeof(vDir) / sizeof(sPos);
        int hayCamino = 0;
        sPos cur = m_current;
        for (int dir = 0; dir < len; dir++)
        {
            int fila = cur.m_fila + vDir[dir].m_fila;
            int columna = cur.m_columna + vDir[dir].m_columna;
            char valor = m_matriz[fila][columna];
            if (valor == kVacio || valor == kFin)
            {
                hayCamino++;

                // Siguiente camino vacio lo marcamos:
                m_matriz[fila][columna] = kNulo;

                if (hayCamino > 1)
                {
                    // Hay mas de un camino, lo reservamos para mirar cuando se nos agote el current.
                    sPos next(fila, columna);
                    next.m_index = static_cast<int>(m_vecPos.size());
                    next.m_num_pasos = m_current.m_num_pasos + 1;
                    m_vecPos.push_back(next);

                    //cLog::print(" Bifurcacion:   [ %2d, %2d ]   pasos: %d\n",
                    //    next.m_fila, next.m_columna,
                    //    next.m_num_pasos);

                    m_matriz[fila][columna] = kInicio;
                }
                else
                {
                    // Todavia solo es un solo camino, continuamos en este.
                    m_current.m_fila = fila;
                    m_current.m_columna = columna;
                    m_current.m_num_pasos++;
                }
            }
        }

        if (!hayCamino)
        {
            if (m_last.m_num_pasos < m_current.m_num_pasos)
                m_last = m_current;

            //cLog::print(" Fin de camino:   %d pasos  [ %2d, %2d ]\n",
            //    m_current.m_num_pasos,
            //    m_current.m_fila, m_current.m_columna
            //);

            if (m_vecPos.size() > 0)
            {
                // Hay que quitar el primero:
                m_vecPos.erase(m_vecPos.begin());
                if (m_vecPos.size() > 0)
                {
                    // Y quedarnos con el siguiente
                    m_current = m_vecPos.front();
                    //cLog::print("    %d [ %2d, %2d ]\n", m_current.m_index, m_current.m_fila, m_current.m_columna);
                }
                else
                {
                    //cLog::print(" Hemos terminado: last [ %2d, %2d ]  pasos: %d\n", m_last.m_fila, m_last.m_columna, m_last.m_num_pasos);
                    m_matriz[m_last.m_fila][m_last.m_columna] = kFin;
                }
            }
            else
            {
                //cLog::print(" Hemos terminado: Este print no sale\n");
            }
        }
    }

    return 0;
}


void sLaberinto_2::ramdomize(int* array, int n)
{
    for (int i = n - 1; i > 0; --i)
    {
        int j = rand() % (i + 1);
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}

bool sLaberinto_2::isValidPosition(sCoord cur, int alto, int ancho)
{
    // no se consideran loa bordes
    return cur.fila >= 1 && cur.columna >= 1 && cur.fila < alto - 1 && cur.columna < ancho - 1;
}


void sLaberinto_2::removeWall(sCoord cur, int ancho_celda, int alto_celda)
{
    for (int i = 0; i < alto_celda; i++)
    {
        for (int j = 0; j < ancho_celda; j++)
        {
            m_matriz[cur.fila + i][cur.columna + j] = kVacio;
        }
    }
}


/*========================================================================*\
|* Fin de sLaberinto_2.cpp
\*========================================================================*/
