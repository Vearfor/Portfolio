/*------------------------------------------------------------------------*\
|* main.cpp
\*------------------------------------------------------------------------*/

#include "cConio.h"
#include "cConsola.h"
#include "cLog.h"
#include "sMyMaze.h"
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
    cLog::print("\n");
    cConsola::PulsaTecla(" Pulsa tecla para terminar ");
    cLog::print("\n");
    cConio::SetColor(eTextColor::eTexNormal);
    //----------------------------------------------------------------------

    delete g_pVista;
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

    cConsola::setNombreProceso("Laberinto");

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


int drawMaze2D()
{
    sVistaConsola* pVistaConsola = new sVistaConsola();

    if (!pVistaConsola)
    {
        cLog::error(" drawMaze2D: Error: new\n");
        return -1;
    }

    g_pVista = pVistaConsola;

    g_pVista->inicia(g_pLaberinto);
    g_pVista->dibuja(g_pLaberinto);

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
    cLog::print(" LABERINTO:     Construimos un laberinto de %d\n", g_iDim);
    cLog::print("                Funcion: ");
    cConio::SetColor(eTextColor::eTexBlanco);
    cLog::print("createMaze2D(int n)");
    cConio::SetColor(eTextColor::eTexCeleste);
    cLog::print("   siendo n la dimension %d\n", g_iDim);
    cLog::print("\n");
    cConio::SetColor(eTextColor::eTexAmarillo);
    cLog::print(" 0 - He desempolvado codigo antiguo que tenia que sigue compilando y funcionando.\n");
    cLog::print("\n");
    cConio::SetColor(eTextColor::eTexCeleste);
    cLog::print(" 1 - Parseamos los parametros necesarios\n");
    cLog::print("     Obligando a que sean mayores que 3 e impar.\n");
    cLog::print("     Y no se si aplicar un limite, se nos saldrian de la pantalla\n");
    cLog::print("     Un solo camino entre A y B. No puede haber mas de uno.\n");
    cLog::print("     'A' estara en en el primer hueco de la esquina superior izquierda. Lo traduzco a la posicion (2.2)\n");
    cLog::print("     'B' al final de ese camino mas largo.\n");
    cLog::print("     Los caminos no pueden ser diagonales\n");
    cLog::print("\n");
    cLog::print(" 2 - Utilizando los parametros generariamos una estructura de datos en tabla para contener\n");
    cLog::print("     el laberinto.\n");
    cLog::print("     Y para visualizarlos mejor generamos otra matriz con caracteres de las lineas.\n");
    cLog::print("\n");
    cLog::print(" 3 - y mostraremos la representacion por pantalla\n");
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
