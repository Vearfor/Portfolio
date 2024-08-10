/*========================================================================*\
|* sLaberinto_2.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include "sLaberinto.h"
#include "sPos.h"
//#include <unordered_map>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Struct
//--------------------------------------------------------------------------
// Una estructura en c++ es simplemente una clase con sus elementos publicos
// Así que meteremos en la parte privada aquello que no queremos que se
// tenga acceso.
//--------------------------------------------------------------------------
struct sLaberinto_2
    : public sLaberinto
{
    sLaberinto_2();
    ~sLaberinto_2();

    virtual int creaLaberinto() override;
    virtual int calculaCaminoMasLargo() override;

    virtual int creaLaberintoFrame() override;

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

    // Para el calculo del camino mas largo:
    std::vector<sPos> m_vecPos;
    sPos m_current{ 1, 1 };
    sPos m_last{ 1, 1 };
};


/*========================================================================*\
|* Fin de sLaberinto_2.h
\*========================================================================*/
