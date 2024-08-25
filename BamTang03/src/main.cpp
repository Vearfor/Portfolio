/*------------------------------------------------------------------------*\
|* main.cpp
\*------------------------------------------------------------------------*/

#include "cConio.h"
#include "cConsola.h"
#include "cLog.h"
#include "sMyMaze.h"
#include "sVistaConsola.h"
#include "sVistaSDL.h"
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
int preinicio();
int presentacion();
//--------------------------------------------------------------------------
// Las funciones que realmente nos piden
//--------------------------------------------------------------------------
int createMaze2D(int n);
int drawMaze2D();
//--------------------------------------------------------------------------
void getCaminoFichero(const char* pcFichero, char* pcSalida, int iSizeSalida);
void getNombreFichero(const char* pcPathFichero, char* pcSalida, int iSizeSalida);
void trim(char* pcCadena, char cBlancos);
bool isBlank(char pCar);
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Globales
//--------------------------------------------------------------------------
int g_iDim = 5;
sLaberinto* g_pLaberinto = nullptr;
sVista* g_pVista = nullptr;
sVistaSDL* g_pVistaSDL = nullptr;
char vcNombrePrograma[LON_BUFF / 8];
//--------------------------------------------------------------------------


int main(int iArgc, char * vcArgv[])
{
    cLog log;
    preinicio();

    miError(parametros(iArgc, vcArgv));

    //----------------------------------------------------------------------
    presentacion();
    //----------------------------------------------------------------------
    createMaze2D(g_iDim);
    drawMaze2D();
    //----------------------------------------------------------------------
    cConio::SetColor(eTextColor::eTexNormal);
    //----------------------------------------------------------------------

    delete g_pVistaSDL;
    delete g_pVista;        // En Realidad es g_pVistaConsola
    delete g_pLaberinto;

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
    cConsola::setNombreProceso("Laberinto 2D");
    return 0;
}


//--------------------------------------------------------------------------
// Control de los parametros:
//--------------------------------------------------------------------------
int parametros(int iArgc, char* vcArgv[])
{
    getNombreFichero(vcArgv[0], vcNombrePrograma, sizeof(vcNombrePrograma));

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
    printf("      %s  <size>  mayor que %d.\n", vcNombrePrograma, kMin);
    printf("                             no superior que %d\n", kLim);
    printf("                             (el enunciado no dice que haya limite, tampoco lo contrario)\n");
    printf("                             y debe ser impar\n");
    printf("                             (Superior a %d hacen que sean visualmente no manejables en la Consola)\n", kLim);
    printf("\n");
    cConsola::PulsaTecla(" Pulsa tecla para terminar ");
    printf("\n");
    return -1;
}


//==========================================================================
// Las funciones que debemos construir:
//==========================================================================
//--------------------------------------------------------------------------
// Creamos el laberinto
//--------------------------------------------------------------------------
int createMaze2D(int n)
{
    sMyMaze* pLaberinto = new sMyMaze();

    if (!pLaberinto)
    {
        cLog::error(" createMaze2D: Error: new\n");
        return -1;
    }

    g_pLaberinto = pLaberinto;
    g_pLaberinto->createMaze2D(n);

    return 0;
}


//--------------------------------------------------------------------------
// Dibujamos el laberinto
//--------------------------------------------------------------------------
int drawMaze2D()
{
    sVistaConsola* pVistaConsola = new sVistaConsola();

    if (!pVistaConsola)
    {
        cLog::error(" drawMaze2D: Error: new sVistaConsola\n");
        return -1;
    }

    // Mientras aqui utilizamos la variable global de vista
    g_pVista = pVistaConsola;

    miError(
        g_pVista->inicia(g_pLaberinto) ||
        g_pVista->update() ||
        g_pVista->dibuja(g_pLaberinto)
    );

    // Aqui podemos compartir las dos vistas, ejecutando la de SDL:
    // Y para poder borrarlas luego sin problemas utilizo la vista de SDL
    // en variable global
    g_pVistaSDL = new sVistaSDL();

    if (!g_pVistaSDL)
    {
        cLog::error(" drawMaze2D: Error: new sVistaSDL\n");
        return -1;
    }

    miError(g_pVistaSDL->mainLoop(g_pLaberinto));

    return 0;
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
    cLog::print(" LABERINTO 2D:     Construimos un laberinto de %d\n", g_iDim);
    cLog::print("                Funcion: ");
    cConio::SetColor(eTextColor::eTexBlanco);
    cLog::print("drawMaze2D()");
    cConio::SetColor(eTextColor::eTexCeleste);
    cLog::print("\n");
    cLog::print("                (utilizamos la funcion anterior:  ");
    cConio::SetColor(eTextColor::eTexBlanco);
    cLog::print("createMaze2D(int n)");
    cConio::SetColor(eTextColor::eTexCeleste);
    cLog::print("\n");
    cConio::SetColor(eTextColor::eTexCeleste);
    cLog::print(" 1 - Partimos de lo que se hizo en el anterior\n");
    cLog::print("     En esta nos centraremos en el dibujado, en principio con SDL, dejaré entonces a mano\n");
    cLog::print("     el acceso a la libreria y a los includes en la solucion final\n");
    cLog::print("     Haremos una clase sVista abstracta y de ahí se diferenciaran las distintas vistas\n");
    cLog::print("     sea para consola, o sea para libreria grafica\n");
    cLog::print("\n");
    cLog::print(" 2 - Al menos empezaremos probando que podemos iniciar SDL.\n");
    cLog::print("     Empezando por distinguir entre Vista de Cosola y Vista de SDL\n");
    cLog::print("     Estamos compilando en Debug, si luego la libreria no nos cabe, habria que hacerlo en Release\n");
    cLog::print("\n");
    cLog::print(" 2 - En este caso para SDL necesitamos tener un 'main' 'loop' para la gestión de eventos\n");
    cLog::print("     y el dibujado de los cambios\n");
    cLog::print("\n");
    cConsola::PulsaTecla(" Pulsa tecla para continuar ");
    cConio::Cls();
    return 0;
}


void getCaminoFichero(const char* pcFichero, char* pcSalida, int iSizeSalida)
{
    char* pcCad, * pcRes, * pcTok;
    char	vcCadena[LON_BUFF];
    char    vcDir[LON_BUFF];

    pcTok = nullptr;

    memset(pcSalida, 0, iSizeSalida);
    //---------------------------------------------------
    // El nombre del fichero es la Ultima cadena
    // despues del '\' o del '/'.
    //---------------------------------------------------
    // mCopia(vcCadena, pcFichero);
    strncpy_s(vcCadena, sizeof(vcCadena), pcFichero, sizeof(vcCadena) - 1);
    // uTrim(vcCadena, '"');
    // mInicio(vcDir);
    memset(vcDir, 0, sizeof(vcDir));
    for (pcRes = NULL,
        pcCad = strtok_s(vcCadena, "/\\", &pcTok);
        pcCad;
        pcCad = strtok_s(NULL, "/\\", &pcTok)
        )
    {
        if (pcRes)
        {
            strcat_s(vcDir, sizeof(vcDir), pcRes);
            strcat_s(vcDir, sizeof(vcDir), "/");
        }

        pcRes = pcCad;
    }
    //---------------------------------------------------
    strncpy_s(pcSalida, iSizeSalida, (const char*)vcDir, iSizeSalida - 1);
    //---------------------------------------------------
}


void getNombreFichero(const char* pcPathFichero, char* pcSalida, int iSizeSalida)
{
    char* pcCad, * pcRes, * pcTok;
    char	vcCadena[LON_BUFF];


    memset(pcSalida, 0, iSizeSalida);
    //---------------------------------------------------
    // El nombre del fichero es la Ultima cadena
    // despues del '\' o del '/'.
    //---------------------------------------------------
    mCopia(vcCadena, pcPathFichero);
    trim(vcCadena, '"');
    for (pcRes = NULL, pcCad = strtok_s(vcCadena, "/\\", &pcTok); pcCad; pcCad = strtok_s(NULL, "/\\", &pcTok))
    {
        pcRes = pcCad;
    }
    if (pcRes)
    {
        strncpy_s(pcSalida, iSizeSalida, pcRes, iSizeSalida - 1);
    }
    //---------------------------------------------------
}


void trim(char* pcCadena, char cBlancos)
{
    if (mNoVacia(pcCadena))
    {
        // Por detras.
        //------------------------------------------------------------------
        int i, k, j, iLen, iLen1 = (int)strlen(pcCadena);

        for (
            i = 1;
            (
                (cBlancos == 0 && isBlank(pcCadena[iLen1 - i]))
                || (cBlancos != 0 && cBlancos == pcCadena[iLen1 - i])
                ) && i < iLen1;
            i++
            )
        {
            pcCadena[iLen1 - i] = 0;
        }
        // Nueva longitud
        iLen = (int)strlen(pcCadena);

        // Por delante
        //------------------------------------------------------------------
        // char * pcCopia = (char *) mNew(char,iLen+1,"Copia uTrim");
        char* pcCopia = new char[iLen + 1];
        if (pcCopia)
        {
            memset(pcCopia, 0, iLen + 1);
            for (
                j = 0;
                j < iLen &&
                (
                    (cBlancos == 0 && isBlank(pcCadena[j]))
                    || (cBlancos != 0 && cBlancos == pcCadena[j])
                    );
                j++
                )
                ;
            for (k = 0, i = j; k < (iLen - j) && i < iLen1; k++, i++)
            {
                pcCopia[k] = pcCadena[i];
            }
            iLen = (int)strlen(pcCopia);
            strncpy_s(pcCadena, iLen + 1, pcCopia, iLen);
            delete[] pcCopia;
        }
        //------------------------------------------------------------------
    }
}


bool isBlank(char pCar)
{
    int iCar = (int)pCar;

    // Hacemos que todos los que nos den valores negativos sean tambien blancos
    // a eliminar:   iCar < 0 ;

    return (iCar == ' ' || iCar == '\t' || iCar == '\n' || iCar == '\r' || iCar < 0);
}


/*------------------------------------------------------------------------*\
|* Fin de main.cpp
\*------------------------------------------------------------------------*/
