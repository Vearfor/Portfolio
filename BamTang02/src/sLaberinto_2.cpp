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
    { -1,  0, eSentido::eTop    , "Arriba"      , 0 , false },
    {  0,  1, eSentido::eRight  , "Derecha"     , 1 , false },
    {  1,  0, eSentido::eBot    , "Abajo"       , 2 , false },
    {  0, -1, eSentido::eLeft   , "Izquierda"   , 3 , false },
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

        // Elmina la pared entre dos posiciones validad
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

    // El primero se marca como inicio A.
    m_matriz[1][1] = kInicio;
    // El camino mas largo sera la salida/final del laberinto y se marca con kFin
    // Nos recorremos el 'lab' para calcular cual es la celda con el camino mas largo desde kInicio:
    sPos last = calculaCaminoMasLargo();
    m_matriz[last.m_fila][last.m_columna] = kFin;

    return 0;
}


sPos sLaberinto_2::calculaCaminoMasLargo()
{
    sPos last(m_size - 2, m_size - 2);
    return last;

    //sCoord cur{ 1, 1, -1 };
    std::unordered_map<int, sPos> mapPos;

    int index = 0, key = 0;
    sPos cur(1, 1);
    cur.m_index = 0;
    mapPos[0] = cur;

    for (size_t size = mapPos.size(); size > 0; size = mapPos.size())
    {
        do
        {
            cur = mapPos[index];
            index = cur.m_index;
            cLog::print(
                " [%3d] %3d   x: %2d y: %2d  [%s]\n", 
                size, index, cur.m_fila, cur.m_columna,
                (cur.m_visitada ? "visitada. Cambio de camino" : "no visitada"));
            if (cur.m_visitada)
            {
                index++;
            }
        } while (cur.m_visitada);

        bool hayNext = false;
        sPos primerCur;
        int len = sizeof(vDir)/sizeof(sPos);
        key = index + 1;
        for (int i = 0; i < len; i++)
        {
            // Miro si hay hueco en:
            int fila = cur.m_fila + vDir[i].m_fila;
            int columna = cur.m_columna + vDir[i].m_columna;
            int car = m_matriz[fila][columna];
            if (car == kVacio)
            {
                // Insertamos en la lista de vacios
                sPos nextPos(fila, columna);
                nextPos.m_sentido = vDir[i].m_sentido;
                nextPos.m_nombre = vDir[i].m_nombre;
                nextPos.m_index = key;
                mapPos[nextPos.m_index] = nextPos;
                if (!hayNext)
                {
                    primerCur = nextPos;
                }
                hayNext = true;
            }
        }
        if (hayNext)
        {
            cur.m_visitada = true;
            cur = primerCur;
            index = cur.m_index;
        }
        else
        {
            quitarElemento(mapPos, cur.m_index);
            index++;
        }
    }
    return last;
}


void sLaberinto_2::quitarElemento(std::unordered_map<int, sPos>& mapPos, int index)
{
    auto it = mapPos.find(index);
    if (it != mapPos.end())
    {
        mapPos.erase(it);
    }
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
