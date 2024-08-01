/*========================================================================*\
|* sVistaConsola.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include "sVista.h"
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Struct
//--------------------------------------------------------------------------
struct sVistaConsola
    : public sVista
{
    sVistaConsola();
    ~sVistaConsola();

    virtual int inicia() override;
    virtual int dibuja(sLaberinto* lab) override;

    char** transformarMuros(char**, int size);

    int getMuro(int fila, int columna, char** src_matriz, int size);
    void mostrar_1(char** matriz, int size);
    void mostrar_2(char** matriz1, char** matriz2, int size);
    void copia(char* vcCopia, char* src, int size);

    const int kLVertical = 179;
    const int kLHorizontal = 196;
    const int kSSupDer = 191;
    const int kSInfIzq = 192;

    const int kSSupIzq = 218;
    const int kSInfDer = 217;

    const int kCruz = 197;

    const int kMIzq = 195;
    const int kMTop = 194;
    const int kMDer = 180;
    const int kMBot = 193;
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de sVistaConsola.h
\*========================================================================*/
