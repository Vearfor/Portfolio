/*========================================================================*\
|* sLaberinto.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include "sPos.h"
#include "cRandom.hpp"
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
struct sVistaSDL;
//--------------------------------------------------------------------------
struct sLaberinto
{
    sLaberinto(const char * pcSoy);
    virtual ~sLaberinto();

    int createMaze2D(int size);

    char** getMatriz() const { return m_matriz; }
    int getSize() { return m_size; }
    sRenderObject* getPunto() { return m_pPunto; }

    virtual int creaLaberinto() = 0;
    virtual int calculaCaminoMasLargo() = 0;
    virtual int creaLaberintoFrame() = 0;
    virtual int decideTecla() = 0;

    void checkHemosLlegado();
    void stopDemo();
    void limpiaMarcas();

    void setPlayingDemo(bool playingDemo) { m_isPlayingDemo = playingDemo; }
    bool estaEnElFin() { return m_bEstaEnElFin; }
    bool isPlayingDemo() { return m_isPlayingDemo; }
    bool hayPausa() { return m_hayPausa; }
    void togglePausa() { m_hayPausa = !m_hayPausa; }

    void setVistaSDL(sVistaSDL* pVista) { m_pVistaSDL = pVista; }

    static void destruyeMatriz(char** matriz, int size);
    static char** getCopiaMatriz(char** src_matriz, int size);

protected:

    std::string m_quienSoy{ "" };
    int m_size{ kMin + 2 };

    // Y para el laberinto, ya sabeomos que son de dos dimensiones, lo que
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

    // El objeto que se iba a utilizar para marcar el fin:
    sRenderObject* m_pFin{ nullptr };

    // Hay que tener en cuenta que lo estamos haciendo mal, hay que separar de las vistas
    // los contenidos/datos centrales del Laberinto
    sRenderObject* m_pPunto{ nullptr };
    // El render object sera el personaje que esta dentro del laberinto y debería estar fuera de la vista !!!

    int creaMatriz(int size);

    bool m_bEstaEnElFin{ false };
    bool m_hayPausa{ false };
    bool m_isPlayingDemo{ false };

    // Para el calculo del camino mas largo:
    std::vector<sPos> m_vecPos;
    sPos m_current{ 1, 1 };
    sPos m_last{ 1, 1 };

    // Para las llamadas finales a la Vista SDL
    sVistaSDL* m_pVistaSDL{ nullptr };
};


/*========================================================================*\
|* Fin de sLaberinto.h
\*========================================================================*/

