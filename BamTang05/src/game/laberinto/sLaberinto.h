/*========================================================================*\
|* sLaberinto.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include "sPos.h"
#include "../../tool/cRandom.hpp"
#include <vector>
#include <string>
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Constantes
//--------------------------------------------------------------------------
const int kMin = 3;
#ifdef _MYWINDOWS_
const int kLim = 41;
#else
const int kLim = 25;
#endif // _WINDOWS_
//--------------------------------------------------------------------------
// Valores definidos para marcar:
//--------------------------------------------------------------------------
const char kInicio = 'A';
const char kFin = 'B';
const char kMuro = '#';
const char kVacio = 32;     // El blanco ' '.
const char kNulo = 0;       // Pendiente de definir.
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Struct
//--------------------------------------------------------------------------
// Una estructura en c++ es simplemente una clase con sus elementos publicos
// Así que meteremos en la parte privada aquello que no queremos que se
// tenga acceso.
//--------------------------------------------------------------------------
struct sRenderObject;
//--------------------------------------------------------------------------
struct sLaberinto
{
    sLaberinto(const char * pcSoy);
    virtual ~sLaberinto();

    int createMaze2D(int size);

    char** getMatriz() const { return m_matriz; }
    int getSize() { return m_size; }
    sRenderObject* getObjectPunto() { return m_pPunto; }
    sRenderObject* getObjectSuelo() { return m_pSuelo; }
    sRenderObject* getObjectMuro() { return m_pMuro; }
    sRenderObject* getObjectInicio() { return m_pInicio; }
    sRenderObject* getObjectFin() { return m_pFin; }
    sRenderObject* getObjectCamara() { return m_pObjCamara; }

    virtual int creaLaberinto() = 0;
    virtual int creaLaberintoFrame() = 0;
    virtual int calculaCaminoMasLargo() = 0;

    int arriba();
    int derecha();
    int abajo();
    int izquierda();

    static void destruyeMatriz(char** matriz, int size);
    static char** getCopiaMatriz(char** src_matriz, int size);

protected:

    std::string m_quienSoy{ "" };
    int m_size{ kMin + 2 };

    // Y para el laberinto, ya sabemos que son de dos dimensiones, lo que
    // siempre hemos llamado una matriz.
    // En este caso, cuadrada, num.filas = num.columnas, siendo el 'size'
    // variable, no fijo.
    // Es un array de 2 dimensiones de tamaño dinamico:
    // - un puntero que apuntará a un array de punteros:
    //   que se puede traducir en un doble puntero a enteros.
    // - de enteros o un enumerado que indique si estamos en muro o en vacio.
    //   ... lo dejaremos como entero, para en pruebas ver que nos funcionan
    //   bien los contenidos de filas y columnas ... por ahora.
    char** m_matriz{ nullptr };
    cRandom m_motor;

    // Hay que tener en cuenta que lo estamos haciendo mal, hay que separar de las vistas
    // los contenidos/datos centrales del Laberinto
    sRenderObject* m_pPunto{ nullptr };
    // El render object sera UN personaje que esta dentro del laberinto y debería estar fuera de la vista !!!

    // Y el laberinto se compondra del resto de RenderObjects:
    sRenderObject* m_pSuelo{ nullptr };
    sRenderObject* m_pMuro{ nullptr };
    sRenderObject* m_pInicio{ nullptr };
    sRenderObject* m_pFin{ nullptr };

    sRenderObject* m_pObjCamara{ nullptr };

    int creaMatriz(int size);
};


/*========================================================================*\
|* Fin de sLaberinto.h
\*========================================================================*/

