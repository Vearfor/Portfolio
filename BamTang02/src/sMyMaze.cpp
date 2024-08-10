/*========================================================================*\
|* sMyMaze.cpp
\*========================================================================*/

#include "sMyMaze.h"
#include "cLog.h"
#include "nComun.h"
#include <cstring>
#include <cctype>


sMyMaze::sMyMaze()
    : sLaberinto("Laberinto 2")
{
    cLog::print(" quien: %s\n", m_quienSoy.c_str());
}

sMyMaze::~sMyMaze()
{
}


sPos vDir[4] =
{
    { -1,  0, eSentido::eTop    , "Arriba"      , 0 , false, 0 },
    {  0,  1, eSentido::eRight  , "Derecha"     , 1 , false, 0 },
    {  1,  0, eSentido::eBot    , "Abajo"       , 2 , false, 0 },
    {  0, -1, eSentido::eLeft   , "Izquierda"   , 3 , false, 0 },
};


int sMyMaze::creaLaberinto()
{
    sPos posInicial(1, 1);
    sPos posFinal(m_size - 2, m_size - 2);

    m_ancho_celda = 1;
    m_alto_celda = 1;

    int sizeTotal = m_size * m_size;
    m_vPos = new sPos[sizeTotal];

    // El offset viene del ancho que hay enttre las celdas:
    m_xoffset = 1 + m_ancho_celda;
    m_yoffset = 1 + m_alto_celda;

    m_index = 0;
    sPos start = { 1, 1, eSentido::eNone, "Inicio", 0, false, 0 };
    m_vecPos.clear();
    m_vecPos.push_back(start);

    m_vPos[m_index++] = start;

    while (m_index)
    {
        creaLaberintoFrame();
    }

    delete[] m_vPos;

    // Empezamos en inicio: 'A':
    m_current = { 1, 1, eSentido::eNone, "Inicio", 0, false, 0 };
    m_vecPos.clear();
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
    // El ultimo se marca como inicio 'B'
    m_matriz[m_last.m_fila][m_last.m_columna] = kFin;

    return 0;
}


int sMyMaze::creaLaberintoFrame()
{
    if (m_index)
    {
        int idx[4];
        sPos cur = m_vPos[--m_index];
        int fila = cur.m_fila;
        int columna = cur.m_columna;
        if (m_matriz[fila][columna] != kMuro)
        {
            return 0;
        }

        // Elmina la pared entre dos posiciones validas
        if (cur.m_sentido != eSentido::eNone)
        {
            int dx = vDir[static_cast<int>(cur.m_sentido)].m_fila;
            int dy = vDir[static_cast<int>(cur.m_sentido)].m_columna;
            setHole({ fila - dx, columna - dy });
        }
        setHole(cur);

        ordenRandom(idx, 4);

        for (int k = 0; k < 4; k++)
        {
            std::string sentido = vDir[static_cast<int>(idx[k])].m_nombre;
            int dx = vDir[idx[k]].m_fila;
            int dy = vDir[idx[k]].m_columna;
            int nx = fila + (m_xoffset * dx);
            int ny = columna + (m_yoffset * dy);

            sPos npos{ nx, ny };

            if (isValidPosition(npos, m_size, m_size) && m_matriz[nx][ny] == kMuro)
            {
                char vcNext[16];
                mInicio(vcNext);
                sprintf_s(vcNext, sizeof(vcNext) - 1, "next %2d", m_index);

                m_vPos[m_index++] = { nx, ny,  static_cast<eSentido>(idx[k]), vcNext, m_index-1, false, 0 };
            }
        }
        return 1;
    }
    return 0;
}


int sMyMaze::calculaCaminoMasLargo()
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

            if (m_vecPos.size() > 0)
            {
                // Hay que quitar el primero:
                m_vecPos.erase(m_vecPos.begin());
                if (m_vecPos.size() > 0)
                {
                    // Y quedarnos con el siguiente
                    m_current = m_vecPos.front();
                }
                else
                {
                    m_matriz[m_last.m_fila][m_last.m_columna] = kFin;
                }
            }
            //else
            //{
            //    //cLog::print(" Hemos terminado: Este print no sale\n");
            //}
        }
    }

    return 0;
}


// Decide en que orden(aleatorio) se quedan los siguientes alrededor de nosotros:
void sMyMaze::ordenRandom(int* array, int n)
{
    array[0] = static_cast<int>(eSentido::eTop);
    array[1] = static_cast<int>(eSentido::eRight);
    array[2] = static_cast<int>(eSentido::eBot);
    array[3] = static_cast<int>(eSentido::eLeft);

    for (int i = n - 1; i > 0; --i)
    {
        int j = rand() % (i + 1);
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}


bool sMyMaze::isValidPosition(sPos pos, int alto, int ancho)
{
    // no se consideran loa bordes
    return pos.m_fila >= 1 && pos.m_columna >= 1 && pos.m_fila < alto - 1 && pos.m_columna < ancho - 1;
}


void sMyMaze::setHole(sPos pos)
{
    //----------------------------------------------------------------------
    // Si la celda/pos del laberinto es mayor de 1, mejor con un for
    // con uno, nos valdria con una unica sentencia:
    // m_matriz[pos.m_fila + 0][pos.m_columna + 0] = kVacio;
    //----------------------------------------------------------------------
    for (int i = 0; i < m_alto_celda; i++)
    {
        for (int j = 0; j < m_ancho_celda; j++)
        {
            m_matriz[pos.m_fila + i][pos.m_columna + j] = kVacio;
        }
    }
}


/*========================================================================*\
|* Fin de sMyMaze.cpp
\*========================================================================*/
