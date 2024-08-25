/*------------------------------------------------------------------------*\
|* main.cpp
\*------------------------------------------------------------------------*/


#include "sGame.h"
#include "sGlobal.h"
#include "tool/cTime.h"
#include "laberinto/sMyMaze.h"
#include "vistas/sVistaConsola.h"
#include "vistas/sVista3D.h"
#include "tool/cLog.h"
#include "tool/nComun.h"
#include "tool/consola/cConio.h"
#include "tool/consola/cConsola.h"
#include "swat/cMonitor.h"


//--------------------------------------------------------------------------
// Prototipos
//--------------------------------------------------------------------------
int parametros(int argc, char* argv[]);
int ayuda(const char* pcFormat, ...);
int preinicio();
int presentacion();
//--------------------------------------------------------------------------


int main(int iArgc, char* vcArgv[])
{
    preinicio();

    cLog log;
    sGame juego;

    miError(parametros(iArgc, vcArgv));
    presentacion();

    cTime time(60);
    for (juego.init(); juego.isRunning(); juego.eventos())
    {
        juego.update(time.getDeltaTime());
        juego.render();

        time.espera(false);
    }

    cConsola::PulsaTecla(" Pulsa tecla para terminar");

    cMonitor::release();
    return 0;
}


//--------------------------------------------------------------------------
// preinicio: elementos preparatorios
//--------------------------------------------------------------------------
int preinicio()
{
#ifdef _MYWINDOWS_
    cConsola::setModo(eModConsola::eMOD_WINDOWS);
    cConsola::SetActiva(false);
#else
    cConsola::setModo(eModConsola::eMOD_CONSOLA);
    cConsola::SetActiva(true);
#endif // _MYWINDOWS_
    cConsola::setNombreProceso("LABERINTO 3D");
    cConsola::setInstance(GetModuleHandle(NULL));
    cLog::print("\n");

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

    sGlobal::m_iDim = std::atoi(vcArgv[1]);
    if (sGlobal::m_iDim==0)
        return ayuda("el parametro del 'size' debe ser numerico");

    if (sGlobal::m_iDim<(kMin + 1) || sGlobal::m_iDim>kLim)
        return ayuda("el 'size' debe de ser mayor que %d y no superior a %d", kMin, kLim);

    if (sGlobal::m_iDim % 2 == 0)
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
    printf("                        (Superior a %d hacen que sean visualmente no menajables)\n", kLim);
    printf("\n");
    cConsola::PulsaTecla(" Pulsa tecla para terminar ");
    printf("\n");
    return -1;
}


//--------------------------------------------------------------------------
// Presentacion de lo que vamos a hacer
//--------------------------------------------------------------------------
int presentacion()
{
    // word normalColor = cConio::GetNormalColor();
    cConio::SetColor(eTextColor::eTexCeleste);
    cLog::print(" ");
    cConio::Cls();
    cLog::print("\n");
    cLog::print(" %s:     Construimos un laberinto de %d\n", cConsola::getNombreProceso(), sGlobal::m_iDim);
    cLog::print("\n");
    cLog::print(" 1 - Partimos de lo que se hizo en el anterior\n");
    cLog::print("     La vista de consola, no se si dejar la vista de SDL\n");
    cLog::print("\n");
    cLog::print(" 2 - Y ahora crearemos una Window, cuyo contexto utilizaremos para dibujar 3D con openGL.\n");
    cLog::print("     Hemos desempolvado codigo de creacion de Ventanas, para no utilizar SDL o Glfw\n");
    cLog::print("\n");
    cLog::print(" 3 - Dejaremos el 'main' 'loop' de sGame para la gestion de eventos, para la ventana OpenGL\n");
    cLog::print("\n");
    cConsola::PulsaTecla(" Pulsa tecla para continuar ");
    cConio::Cls();
    return 0;
}


/*------------------------------------------------------------------------*\
|* Fin de main.cpp
\*------------------------------------------------------------------------*/
