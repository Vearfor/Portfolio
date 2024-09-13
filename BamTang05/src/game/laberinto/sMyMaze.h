/*========================================================================*\
|* sMyMaze.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include "sLaberinto.h"
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Struct
//--------------------------------------------------------------------------
// Una estructura en c++ es simplemente una clase con sus elementos publicos
// Así que meteremos en la parte privada aquello que no queremos que se
// tenga acceso.
//--------------------------------------------------------------------------
struct sMyMaze
    : public sLaberinto
{
    sMyMaze(sGame* pGame);
    ~sMyMaze();

    virtual int creaLaberinto() override;
    virtual int calculaCaminoMasLargo() override;
    virtual int creaLaberintoFrame() override;
    virtual int decideTecla() override;

private:

    void setHole(sPos pos);
    void ordenRandom(int* array, int n);
    bool isValidPosition(sPos pos, int alto, int ancho);

    // Para la creacion del laberinto:
    int m_index{ 0 };
    sPos* m_vPos{ nullptr };
    int m_xoffset{ 0 };
    int m_yoffset{ 0 };
    int m_ancho_celda{ 1 };
    int m_alto_celda{ 1 };
};


/*========================================================================*\
|* Fin de sMyMaze.h
\*========================================================================*/
