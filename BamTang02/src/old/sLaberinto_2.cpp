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
    //cLog::print(" quien: %s\n", m_quienSoy.c_str());
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

    m_ancho_celda = 1;
    m_alto_celda = 1;

    int sizeTotal = m_size * m_size;
    m_vPos = new sPos[sizeTotal];

    // El offset viene del ancho que hay enttre las celdas:
    // minimo: 1 + 1 = 2.
    m_xoffset = 1 + m_ancho_celda;
    m_yoffset = 1 + m_alto_celda;

    m_index = 0;
    sPos start = { 1, 1, eSentido::eNone, "Inicio", 0, false, 0 };

    m_vPos[m_index++] = start;

    while (m_index)
    {
        creaLaberintoFrame();
    }

    delete[] m_vPos;

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


int sLaberinto_2::creaLaberintoFrame()
{
    if (m_index)
    {
        sPos cur = m_vPos[--m_index];
        int fila = cur.m_fila;
        int columna = cur.m_columna;
        if (m_matriz[fila][columna] != kMuro)
            return 0;

        // Elmina la pared entre dos posiciones validas
        if (cur.m_sentido != eSentido::eNone)
        {
            int dx = vDir[static_cast<int>(cur.m_sentido)].m_fila;
            int dy = vDir[static_cast<int>(cur.m_sentido)].m_columna;
            if (
                cur.m_sentido == eSentido::eLeft ||     // 3
                cur.m_sentido == eSentido::eTop         // 0
               )
            {
                dx *= (m_xoffset - 1);
                dy *= (m_yoffset - 1);
            }
            setHole({ fila - dx, columna - dy });
        }
        setHole(cur);

        int idx[4] = { int(eSentido::eTop), int(eSentido::eRight), int(eSentido::eBot), int(eSentido::eLeft) };
        ramdomize(idx, 4);

        for (int k = 0; k < 4; k++)
        {
            int dx = vDir[idx[k]].m_fila;
            int dy = vDir[idx[k]].m_columna;
            int nx = fila + m_xoffset * dx;
            int ny = columna + m_yoffset * dy;

            sPos ul, lr;
            ul = { min(fila,nx), min(columna, ny) };
            lr =
            {
                max(fila,nx) + m_alto_celda - 1,
                min(columna, ny) + m_ancho_celda - 1
            };

            if (
                isValidPosition(ul, m_size, m_size) &&
                isValidPosition(lr, m_size, m_size) &&
                m_matriz[nx][ny] == kMuro
                )
            {
                m_vPos[m_index++] = { nx, ny,  static_cast<eSentido>(idx[k]), "next", m_index - 1, false, 0 };
            }
        }
    }
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

bool sLaberinto_2::isValidPosition(sPos pos, int alto, int ancho)
{
    return pos.m_fila >= 1 && pos.m_columna >= 1 && pos.m_fila < alto - 1 && pos.m_columna < ancho - 1;
}


void sLaberinto_2::setHole(sPos pos)
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
|* Fin de sLaberinto_2.cpp
\*========================================================================*/
