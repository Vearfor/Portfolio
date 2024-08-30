/*========================================================================*\
|* sVistaConsola.cpp
\*========================================================================*/

#include "sVistaConsola.h"
#include "../laberinto/sLaberinto.h"
#include "../tool/consola/cConio.h"
#include "../tool/consola/cConsola.h"


//--------------------------------------------------------------------------
sVistaConsola::sVistaConsola()
{
}

sVistaConsola::~sVistaConsola()
{
    sLaberinto::destruyeMatriz(m_matrizMuros, m_size);
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// inicio de la Vista Consola
//--------------------------------------------------------------------------
int sVistaConsola::inicia(sLaberinto* lab)
{
    m_pLaberinto = lab;
    m_size = m_pLaberinto->getSize();
    m_matrizMuros = transformarMuros(m_pLaberinto->getMatriz(), m_size);

    return 0;
}


int sVistaConsola::eventos()
{
    // Complicado hacer aqui el shell: ... por ahora nada.
    int running = 1;
    return running;
}


int sVistaConsola::update()
{
    int iRes = (m_renderizado) ? 1 : 0;
    return iRes;
}


int sVistaConsola::render()
{
    if (!m_renderizado)
    {
        cConio::Cls();
        mostrar_1(m_pLaberinto->getMatriz(), m_pLaberinto->getSize());
        cConsola::PulsaTecla(" Pulsa tecla para continuar ");

        cConio::Cls();
        mostrar_2(m_pLaberinto->getMatriz(), m_matrizMuros, m_size);
        cConsola::PulsaTecla(" Pulsa tecla para continuar ");

        cConio::Cls();
        cLog::print("\n");

        // Con la consola lo hacemos una sola vez: modificamos el flag.
        m_renderizado = true;
    }

    return 0;
}



void sVistaConsola::mostrar_1(char** matriz, int size)
{
    cLog::print("\n");
    cLog::print(" La que nos han pedido:\n");
    cLog::print("\n");
    for (int fila = 0; fila < size; fila++)
    {
        cLog::print(" ");
        for (int columna = 0; columna < size; columna++)
        {
            int iValor = matriz[fila][columna];
            char valor = char(iValor);
            cLog::print("%c", valor);
        }
        // Llegamos al final de la fila, saltamos de linea:
        cLog::print("\n");
    }
    cLog::print("\n");
}


void sVistaConsola::mostrar_2(char** matriz1, char** matriz2, int size)
{
    char vcLinea1[LON_BUFF / 8];
    char vcLinea2[LON_BUFF / 8];

    cLog::print("\n");
    cLog::print(" Para visualizarla mejor:\n");
    cLog::print("\n");
    for (int fila = 0; fila < size; fila++)
    {
        char* linea1 = matriz1[fila];
        char* linea2 = matriz2[fila];

        copia(vcLinea1, linea1, size);
        copia(vcLinea2, linea2, size);

        cLog::print(" ");
        cLog::print("%s", vcLinea1);
        cLog::print("      ");
        cLog::print("%s", vcLinea2);
        cLog::print("\n");
    }
    cLog::print("\n");
}


// Nos aseguramos la copia con el nulo final para imprimir como linea
// Nos hemos asegurado/sabemos que en vcCopia cabe src+size
void sVistaConsola::copia(char* vcCopia, char* src, int size)
{
    std::memset(vcCopia, 0, size + 1);
    std::memcpy(vcCopia, src, size);
}


char** sVistaConsola::transformarMuros(char** src_matriz, int size)
{
    char** src_copia = new char* [size];
    for (int i = 0; i < size; i++)
    {
        src_copia[i] = new char[size];
        for (int j = 0; j < size; j++)
        {
            src_copia[i][j] = getMuro(i, j, src_matriz, size);
        }
    }
    return src_copia;
}


int sVistaConsola::getMuro(int fila, int columna, char** src_matriz, int size)
{
    char src = src_matriz[fila][columna];
    int car = src;

    if (src == kMuro)
    {
        if (fila == 0 && columna == 0)
            car = kSSupIzq;
        else if (fila == size - 1 && columna == 0)
            car = kSInfIzq;
        else if (fila == 0 && columna == size - 1)
            car = kSSupDer;
        else if (fila == size - 1 && columna == size - 1)
            car = kSInfDer;
        else if (fila == 0)
        {
            char src1 = src_matriz[1][columna];
            if (src1 == kMuro)
                car = kMTop;
            else
                car = kLHorizontal;
        }
        else if (fila == size - 1)
        {
            char srcM = src_matriz[size - 2][columna];
            if (srcM == kMuro)
                car = kMBot;
            else
                car = kLHorizontal;
        }
        else if (columna == 0)
        {
            char src1 = src_matriz[fila][1];
            if (src1 == kMuro)
                car = kMIzq;
            else
                car = kLVertical;
        }
        else if (columna == size - 1)
        {
            char srcM = src_matriz[fila][size-2];
            if (srcM == kMuro)
                car = kMDer;
            else
                car = kLVertical;
        }
        else
        {
            char srcL = src_matriz[fila][columna - 1];    // left
            char srcR = src_matriz[fila][columna + 1];    // right
            char srcT = src_matriz[fila - 1][columna];    // top, arriba
            char srcB = src_matriz[fila + 1][columna];    // bot, abajo

            // 4
            if (srcL == kMuro && srcR == kMuro && srcT == kMuro && srcB == kMuro)
                car = kCruz;
            // 3
            else if (srcL == kMuro && srcR == kMuro && srcT == kMuro)
                car = kMBot;
            else if (srcL == kMuro && srcR == kMuro && srcB == kMuro)
                car = kMTop;
            else if (srcT == kMuro && srcB == kMuro && srcL == kMuro)
                car = kMDer;
            else if (srcT == kMuro && srcB == kMuro && srcR == kMuro)
                car = kMIzq;
            // 2 Esquinas
            else if (srcB == kMuro && srcR == kMuro)
                car = kSSupIzq;
            else if (srcB == kMuro && srcL == kMuro)
                car = kSSupDer;
            else if (srcT == kMuro && srcL == kMuro)
                car = kSInfDer;
            else if (srcT == kMuro && srcR == kMuro)
                car = kSInfIzq;
            // Horizontales
            else if (srcL == kMuro || srcR == kMuro)
                car = kLHorizontal;
            // Verticales
            else if (srcT == kMuro || srcB == kMuro)
                car = kLVertical;
        }
    }
    return car;
}


/*========================================================================*\
|* Fin de sVistaConsola.h
\*========================================================================*/
