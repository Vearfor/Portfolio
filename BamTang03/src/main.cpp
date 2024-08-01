/*------------------------------------------------------------------------*\
|* main.cpp
\*------------------------------------------------------------------------*/

#include "cConio.h"
#include "cConsola.h"
#include "cLog.h"
#include "sLaberinto_1.h"
#include "sLaberinto_2.h"
#include "sVistaConsola.h"
#include <stdio.h>
#include <string>
#include <conio.h>
#include <cctype>
#include <vector>
#include <map>


//--------------------------------------------------------------------------
// Prototipos
//--------------------------------------------------------------------------
int parametros(int argc, char* argv[]);
int ayuda(const char* pcFormat, ...);
//--------------------------------------------------------------------------
// Las funciones que realmente nos piden
//--------------------------------------------------------------------------
int createMaze2D(int n);
int drawMaze2D();
//--------------------------------------------------------------------------

int g_iDim = 5;

sLaberinto* g_pLaberinto = nullptr;
sVista* g_pVista = nullptr;


int main(int iArgc, char * vcArgv[])
{
#ifdef _MYWINDOWS_
    cConsola::setModo(eModConsola::eMOD_WINDOWS);
    cConsola::SetActiva(false);
#else
    cConsola::setModo(eModConsola::eMOD_CONSOLA);
    cConsola::SetActiva(true);
#endif // _WINDOWS_

    cConsola::setNombreProceso("Laberinto 2D");
    cLog log;

    miError(parametros(iArgc, vcArgv));

    //----------------------------------------------------------------------
    word normalColor = cConio::GetNormalColor();
    cConio::SetColor(eTextColor::eTexCeleste);
    cConio::Cls();
    cLog::print("\n");
    cLog::print(" LABERINTO 2D:  Construimos un laberinto de %d\n", g_iDim);
    cLog::print("                Funcion:  ");
    cConio::SetColor(eTextColor::eTexBlanco);
    cLog::print("drawMaze2D()");
    cConio::SetColor(eTextColor::eTexCeleste);
    cLog::print("\n");
    cLog::print("                (utilizamos la funcion anterior:  ");
    cConio::SetColor(eTextColor::eTexBlanco);
    cLog::print("createMaze2D(int n)");
    cConio::SetColor(eTextColor::eTexCeleste);
    cLog::print("\n");
    cLog::print("\n");
    cConio::SetColor(eTextColor::eTexCeleste);
    cLog::print(" 1 - Partimos de lo que se hizo en el anterior\n");
    cLog::print("     En esta nos centraremos en el dibujado, en principio con SDL, dejaré entonces a mano\n");
    cLog::print("     el acceso a la libreria y a los includes en la solucion final\n");
    cLog::print("     Haremos una clase sVista abstracta y de ahí se diferenciaran las distintas vistas\n");
    cLog::print("     sea para consola, o sea para libreria grafica\n");
    cLog::print("\n");
    cConsola::PulsaTecla(" Pulsa tecla para continuar ");
    //----------------------------------------------------------------------
    cConio::Cls();
    createMaze2D(g_iDim);
    drawMaze2D();
    cLog::print("\n");
    cConsola::PulsaTecla(" Pulsa tecla para terminar ");
    //----------------------------------------------------------------------
    cLog::print("\n");
    cConio::SetColor(eTextColor::eTexNormal);

    delete g_pVista;
    delete g_pLaberinto;

    return 0;
}


//--------------------------------------------------------------------------
// Control de los parametros:
//--------------------------------------------------------------------------
int parametros(int iArgc, char* vcArgv[])
{
    if (iArgc < 2)
    {
        return ayuda("falta el parametro del 'size'");
    }

    g_iDim = std::atoi(vcArgv[1]);
    if (g_iDim==0)
        return ayuda("el parametro del 'size' debe ser numerico");

    if (g_iDim<(kMin + 1) || g_iDim>kLim)
        return ayuda("el 'size' debe de ser mayor que %d y no superior a %d", kMin, kLim);

    if (g_iDim % 2 == 0)
        return ayuda("el 'size' debe ser impar");

    return 0;
}


//--------------------------------------------------------------------------
// Lo que debemos hacer
//--------------------------------------------------------------------------
int ayuda(const char * pcFormat, ...)
{
    va_list stList;

    char vcMensaje[LON_BUFF / 8];
    mInicio(vcMensaje);

    va_start(stList, pcFormat);
    vsnprintf_s(vcMensaje, sizeof(vcMensaje), sizeof(vcMensaje) - 1, pcFormat, stList);
    va_end(stList);

    printf("\n");
    printf(" Error: %s\n", vcMensaje);
    printf("\n");
    printf(" Uso:\n");
    printf("      prog  <size>      mayor que %d.\n", kMin);
    printf("                        no superior que %d (el enunciado no dice que haya limite, tampoco lo contrario)\n", kLim);
    printf("                        y debe ser impar\n");
    printf("\n");
    cConsola::PulsaTecla(" Pulsa tecla para terminar ");
    printf("\n");
    return -1;
}


//==========================================================================
// Las funciones que debemos construir:
//==========================================================================
int createMaze2D(int n)
{
    // sLaberinto_1* pLaberinto = new sLaberinto_1();
    sLaberinto_2* pLaberinto = new sLaberinto_2();

    if (!pLaberinto)
    {
        cLog::error(" createMaze2D: Error: new\n");
        return -1;
    }

    g_pLaberinto = pLaberinto;
    g_pLaberinto->createMaze2D(n);

    return 0;
}


int drawMaze2D()
{
    sVistaConsola* pVistaConsola = new sVistaConsola();

    if (!pVistaConsola)
    {
        cLog::error(" drawMaze2D: Error: new\n");
        return -1;
    }

    g_pVista = pVistaConsola;

    g_pVista->inicia();
    g_pVista->dibuja(g_pLaberinto);

    //cLog::print("\n");
    //for (int i = 128; i < 255; i++)
    //{
    //    char car = char(i);
    //    cLog::print(" %3d [%c]\n", i, i);
    //    //if (std::isprint(i))
    //    //    cLog::print(" %3d [%c]\n", i, i);
    //    //else
    //    //    cLog::print(" %3d [...]\n", i);
    //}
    //cLog::print("\n");

    return 0;
}


/*------------------------------------------------------------------------*\
|* Fin de main.cpp
\*------------------------------------------------------------------------*/
