/*------------------------------------------------------------------------*\
|* main.cpp
\*------------------------------------------------------------------------*/

#include "game/sGame.h"
#include "game/sGlobal.h"
#include "game/laberinto/sLaberinto.h"
#include "tool/cTime.h"
#include "tool/cTool.h"
#include "tool/cLog.h"
#include "tool/consola/cConio.h"
#include "tool/consola/cConsola.h"
#include "swat/windows/cMonitor.h"


//--------------------------------------------------------------------------
// Prototipos
//--------------------------------------------------------------------------
int parametros(int argc, char* argv[]);
int ayuda(const char* pcFormat, ...);
int preinicio();
int presentacion();
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Globales
//--------------------------------------------------------------------------
bool hayDemo = false;
//--------------------------------------------------------------------------


int main(int iArgc, char* vcArgv[])
{
    preinicio();

    cLog log;
    sGame juego;

    miError(parametros(iArgc, vcArgv));
    presentacion();

    juego.demo(hayDemo);

    cTime time(60);
    for (juego.init(); juego.isRunning(); juego.eventos())
    {
        juego.update(time.getDeltaTime());
        juego.render();

        time.espera(true);
    }

    cConio::SetColor(eTextColor::eTexNormal);
    cConsola::PulsaTecla(" Pulsa tecla para terminar ");
    cLog::print("\n");
    // Pendiente de mejorar: quedan mas recursos pendientes de liberar ?
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
    cConsola::setNombreProceso("LABERINTO 3D " TITLE);
    cConsola::setInstance(GetModuleHandle(NULL));
    cLog::print("\n");

    return 0;
}


//--------------------------------------------------------------------------
// Control de los parametros:
//--------------------------------------------------------------------------
int parametros(int iArgc, char* vcArgv[])
{
    cTool::getNombreFichero(vcArgv[0], sGlobal::vcNombrePrograma, sizeof(sGlobal::vcNombrePrograma));

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

    if (iArgc > 2)
    {
        if (!strcmp(vcArgv[2], "Demo"))
        {
            hayDemo = true;
        }
        else
        {
            return ayuda("el segundo parametro es 'Demo', no '%s'", vcArgv[2]);
        }
    }

    return 0;
}


//--------------------------------------------------------------------------
// Lo que debemos hacer
//--------------------------------------------------------------------------
int ayuda(const char* pcFormat, ...)
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
    printf("      %s  <size>  mayor que %d.\n", sGlobal::vcNombrePrograma, kMin);
    printf("                             no superior que %d\n", kLim);
    printf("                             (el enunciado no dice que haya limite, tampoco lo contrario)\n");
    printf("                             y debe ser impar\n");
    printf("                             (Superior a %d hacen que sean visualmente no manejables en la Consola)\n", kLim);
    printf("\n");
    printf("      %s  <size>  Demo       Ejecutamos una Demo.\n", sGlobal::vcNombrePrograma);
    printf("                                        Cada % 3.1f segundos ejecutamos una tecla para mover nuestro cubo\n", sGlobal::m_fIntervaloDemo);
    printf("                                        dentro del laberinto.\n");
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
    cLog::print("     La vista de consola, no se si dejar la vista de SDL, pero quiero demostrar que puedo generar\n");
    cLog::print("     la ventana 3D sin tener que utlizar SDL o GLFW, etc ..\n");
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
