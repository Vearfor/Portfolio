/*========================================================================*\
|* sLaberinto.cpp
\*========================================================================*/


#include "sLaberinto.h"
#include "cLog.h"
#include "nComun.h"


sLaberinto::sLaberinto(const char* pcSoy)
    : m_quienSoy(pcSoy)
    , m_size(kMin + 2)
    , m_matriz(nullptr)
    , m_motor{}
{
    // Iniciamos el motor aleatorio:
    m_motor.inicia();
}

sLaberinto::~sLaberinto()
{
    destruyeMatriz(m_matriz, m_size);
}


int sLaberinto::creaMatriz(int size)
{
    m_size = size;
    char car = kMuro;
    // primero la matriz que apunta al array de filas
    m_matriz = new char* [size];
    miNulo(m_matriz);

    // Y ahora con un for completamos las columnas:
    for (int fila = 0; fila < size; fila++)
    {
        // Creamos las columnas de cada fila:
        m_matriz[fila] = new char[size];
        miNulo(m_matriz[fila]);
        // tengo que multiplicar por sizeof int, ya que son caracteres, no ints
        std::memset(m_matriz[fila], car, size * sizeof(char));
    }

    return 0;
}

void sLaberinto::destruyeMatriz(char ** matriz, int size)
{
    if (matriz)
    {
        for (int fila = 0; fila < size; fila++)
        {
            if (matriz[fila])
            {
                delete[] matriz[fila];
                matriz[fila] = nullptr;
            }
        }
        delete[] matriz;
        matriz = nullptr;
    }
}


int sLaberinto::createMaze2D(int size)
{
    if (creaMatriz(size))
    {
        cLog::error(" sLaberinto::createMaze2D Error: creaMatriz: en la creación de la matriz del laberinto");
        return -1;
    }
    if (creaLaberinto())
    {
        cLog::error(" sLaberinto::createMaze2D Error: creaLaberinto: en la construccion del laberinto");
        return -1;
    }
    return 0;
}


char** sLaberinto::getCopiaMatriz(char** src_matriz, int size)
{
    char** src_copia = new char* [size];
    for (int i = 0; i < size; i++)
    {
        src_copia[i] = new char[size];
        for (int j = 0; j < size; j++)
        {
            src_copia[i][j] = src_matriz[i][j];
        }
    }
    return src_copia;
}


/*========================================================================*\
|* Fin de sLaberinto.h
\*========================================================================*/

