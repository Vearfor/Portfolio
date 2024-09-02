/*========================================================================*\
|* sMyMaze.cpp
\*========================================================================*/

#include "sMyMaze.h"
#include "../../tool/cLog.h"
#include "../../tool/nComun.h"
#include "../../swat/sRenderObject.h"
#include <cstring>
#include <cctype>


sMyMaze::sMyMaze()
    : sLaberinto("My Maze")
{
    cLog::print(" quien: %s\n", m_quienSoy.c_str());
}

sMyMaze::~sMyMaze()
{
}


/*========================================================================*\
|* Array en el que guardamos las posiciones de direccion:
|* filas y columnas
\*========================================================================*/
sPos vDir[4] =
{
    { -1,  0, eSentido::eTop    , "Arriba"      , 0 , false, 0 },
    {  0,  1, eSentido::eRight  , "Derecha"     , 1 , false, 0 },
    {  1,  0, eSentido::eBot    , "Abajo"       , 2 , false, 0 },
    {  0, -1, eSentido::eLeft   , "Izquierda"   , 3 , false, 0 },
};


//--------------------------------------------------------------------------
// Creacion del laberinto
//--------------------------------------------------------------------------
int sMyMaze::creaLaberinto()
{
    /*====================================================================*\
    |* Prepara el bucle de creacion del laberinto
    \*====================================================================*/
    m_ancho_celda = 1;
    m_alto_celda = 1;

    int sizeTotal = m_size * m_size;
    m_vPos = new sPos[sizeTotal];

    // El offset viene del ancho que hay enttre las celdas:
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

    /*====================================================================*\
    |* Prepara el bucle de busqueda del camino mas largo
    \*====================================================================*/
    // Empezamos en inicio: 'A':
    m_current = { 1, 1, eSentido::eNone, "Inicio", 0, false, 0 };
    m_vecPos.clear();
    m_vecPos.push_back(m_current);
    while (m_vecPos.size() > 0)
    {
        calculaCaminoMasLargo();
    }

    /*====================================================================*\
    |* limpio marcas del calculo  del camino mas largo
    \*====================================================================*/
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

    m_pFin->m_fila = m_last.m_fila;
    m_pFin->m_columna = m_last.m_columna;

    return 0;
}


//--------------------------------------------------------------------------
// Creacion del laberinto
// - guardamos las variables como miembros de la clase para poder ver como
//   creabamos el laberinto frame a frame
// 
// int m_index{ 0 };
// sPos* m_vPos{ nullptr };
// int m_xoffset{ 0 };
// int m_yoffset{ 0 };
// int m_ancho_celda{ 1 };
// int m_alto_celda{ 1 };
//--------------------------------------------------------------------------
int sMyMaze::creaLaberintoFrame()
{
    if (m_index>0)
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

        /*================================================================*\
        |* determina que idx(direccion) alrededor de nosotros es valida
        \*================================================================*/
        for (int k = 0; k < 4; k++)
        {
            int dx = vDir[static_cast<int>(idx[k])].m_fila;
            int dy = vDir[static_cast<int>(idx[k])].m_columna;
            int nx = fila + (m_xoffset * dx);
            int ny = columna + (m_yoffset * dy);

            sPos npos{ nx, ny };

            if (isValidPosition(npos, m_size, m_size) && m_matriz[nx][ny] == kMuro)
            {
                char vcNext[16];
                mInicio(vcNext);
                sprintf_s(vcNext, sizeof(vcNext) - 1, "next %2d", m_index);

                // Guardamos la nueva posicion valida
                m_vPos[m_index++] = { nx, ny,  static_cast<eSentido>(idx[k]), vcNext, m_index-1, false, 0 };
            }
        }
        /*================================================================*/
        return 1;
    }
    return 0;
}


//--------------------------------------------------------------------------
// Recorre el laberinto para contabilizar los camninos, que marcamos con
// kNulo
// - tambien guardamos las variables como miembros de la clase para poder
//  ver como contamos los caminos frame a frame
// 
// std::vector<sPos> m_vecPos;
// sPos m_current{ 1, 1 };
// sPos m_last{ 1, 1 };
//--------------------------------------------------------------------------
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


//--------------------------------------------------------------------------
// como una COCTELERA cambia los valores del array para cambiar el orden
// de los valores
// Decide en que orden(aleatorio) se quedan los siguientes alrededor de
// nosotros:
//--------------------------------------------------------------------------
void sMyMaze::ordenRandom(int* array, int n)
{
    array[0] = static_cast<int>(eSentido::eTop);
    array[1] = static_cast<int>(eSentido::eRight);
    array[2] = static_cast<int>(eSentido::eBot);
    array[3] = static_cast<int>(eSentido::eLeft);

    for (int i = n - 1; i > 0; --i)
    {
        int j = int(m_motor.aleatorio(0, double(i + 1)));
        int tmp = array[i];
        array[i] = array[j];
        array[j] = tmp;
    }
}


//--------------------------------------------------------------------------
// Determina si sPos es una posicion valida
//--------------------------------------------------------------------------
bool sMyMaze::isValidPosition(sPos pos, int alto, int ancho)
{
    return pos.m_fila >= 1 && pos.m_columna >= 1 && pos.m_fila < alto - 1 && pos.m_columna < ancho - 1;
}


//--------------------------------------------------------------------------
// Establece el valor vacio para la pos dada
//--------------------------------------------------------------------------
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
