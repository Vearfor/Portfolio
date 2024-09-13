/*========================================================================*\
|* sMyMaze.cpp
\*========================================================================*/

#include "sMyMaze.h"
#include "sPos.h"
#include "../sGame.h"
#include "../sRenderSystem.h"
#include "../vistas/sVista3D.h"
#include "../sGameWindow.h"
#include <tool/cLog.h>
#include <tool/nComun.h>
#include <swat/sRenderObject.h>
#include <cstring>
#include <cctype>


/*------------------------------------------------------------------------*\
|* Constructor & Destructor
\*------------------------------------------------------------------------*/
sMyMaze::sMyMaze(sGame* pGame)
    : sLaberinto(pGame, "My Maze")
{
    cLog::print(" quien: %s\n", m_quienSoy.c_str());
    //m_posAnterior = { 1, 1, eSentido::eNone, "Anterior", 0, false, 0, 32 };
}

sMyMaze::~sMyMaze()
{
}
/*------------------------------------------------------------------------*/


/*========================================================================*\
|* Array en el que guardamos las posiciones de direccion:
|* filas y columnas
\*========================================================================*/
sPos vDir[4] =
{
    { -1,  0, eSentido::eTop    , "Arriba"      , 0 , false, 0, 'W' },
    {  0,  1, eSentido::eRight  , "Derecha"     , 1 , false, 0, 'D'},
    {  1,  0, eSentido::eBot    , "Abajo"       , 2 , false, 0, 'S'},
    {  0, -1, eSentido::eLeft   , "Izquierda"   , 3 , false, 0, 'A'},
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
    sPos start = { 1, 1, eSentido::eNone, "Inicio", 0, false, 0, 32 };
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
    m_current = { 1, 1, eSentido::eNone, "Inicio", 0, false, 0, 32 };
    m_vecPos.clear();
    m_vecPos.push_back(m_current);
    while (m_vecPos.size() > 0)
    {
        calculaCaminoMasLargo();
    }

    /*====================================================================*\
    |* limpio marcas del calculo  del camino mas largo
    \*====================================================================*/
    limpiaMarcas();
    /*====================================================================*/

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
                m_vPos[m_index++] = { nx, ny,  static_cast<eSentido>(idx[k]), vcNext, m_index-1, false, 0, 32 };
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
        }
    }

    return 0;
}


//--------------------------------------------------------------------------
// Busca la tecla mas apropiada para llegar a la celda final del
// laberinto
//--------------------------------------------------------------------------
int sMyMaze::decideTecla()
{
    int tecla = 32;
    int fila = m_pPunto->m_fila;
    int columna = m_pPunto->m_columna;
    std::vector<sPos> locVectorPos{};

    for (int i = 0; i < 4; i++)
    {
        int ifila = vDir[i].m_fila;
        int icolumna = vDir[i].m_columna;

        if (m_matriz[fila + ifila][columna + icolumna] == kFin)
        {
            // Estupendo hemos llegado: esta es la tecla
            tecla = vDir[i].m_tecla;
            break;
        }
        else
        if (m_matriz[fila + ifila][columna + icolumna] == kVacio)
        {
            sPos pos =
            {
                fila + ifila, 
                columna + icolumna, 
                vDir[i].m_sentido, 
                vDir[i].m_nombre.c_str(), 
                vDir[i].m_index, 
                vDir[i].m_visitada, 
                vDir[i].m_num_pasos,
                vDir[i].m_tecla
            };
            locVectorPos.push_back(pos);
        }
    }

    // Todavia tenemos que decidir
    if (tecla == 32)
    {
        if (locVectorPos.size() > 0)
        {
            if (locVectorPos.size() > 1)
            {
                sPos masCercano = locVectorPos[0];
                int diffila_cercano = m_size;
                int difcolumna_cercano = m_size;
                for (auto pos : locVectorPos)
                {
                    int diffila = abs(m_last.m_fila - pos.m_fila);
                    int difcolumna = abs(m_last.m_columna - pos.m_columna);
                    if (
                            (diffila_cercano > diffila) ||
                            (difcolumna_cercano > difcolumna)
                       )
                    {
                        masCercano = pos;
                        diffila_cercano = diffila;
                        difcolumna_cercano = difcolumna;
                    }
                }
                tecla = masCercano.m_tecla;
                // Al haber mas de uno marcamos la vifurcacion:
                // Y en sentido la decision de direccion que se tomo:
                m_vecPos.push_back(sPos(fila, columna, masCercano.m_sentido, "Vifurcacion", 0, true, 0, 32));
            }
            else
            {
                tecla = locVectorPos[0].m_tecla;
            }
        }
        else
        {
            // no hay salida nos vamos a la primera vifurcacion anterior
            if (m_vecPos.size() > 0)
            {
                sPos lastVif = m_vecPos.back();
                m_vecPos.pop_back();
                m_pPunto->m_fila = lastVif.m_fila;
                m_pPunto->m_columna = lastVif.m_columna;
            }
            else
            {
                // Si ya no hay mas vifurcaciones guradadas, hemos terminado,
                // y parece que no hemos conseguido llegar al final
                stopDemo(dynamic_cast<sGameWindow*>(m_pGame->getRender()->m_pVista3D->m_mainWindow));
            }
        }
    }

    if (tecla != 32)
    {
        eSentido sentAnterior = sPos::getSentidoAnterior(tecla);
        if (sentAnterior != eSentido::eNone)
        {
            // De donde venimos ha siso visitada:
            vDir[static_cast<int>(sentAnterior)].m_visitada = true;
            // Marco el camino visitado
            m_matriz[fila][columna] = (m_matriz[fila][columna] ==kVacio)? kNulo: m_matriz[fila][columna];
        }
    }

    return tecla;
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
