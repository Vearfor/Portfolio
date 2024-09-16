/*========================================================================*\
|* cTool.cpp
|*
|*  Created on: 23/08/2024
|*      Author: Enrique Rocafull Ortiz
\*========================================================================*/


#include "cTool.h"
//#include <codecvt>


//==========================================================================
// static members
//==========================================================================
bool cTool::m_bDisplay = false;
//==========================================================================


//--------------------------------------------------------------------------
// Habra funciones para copiar Unicode, pero por ahora nos las vamos
// fabricando. En base a 'la cuenta de la vieja
// Primero hago a este vacio (no vamos a hacer appends ocultos)
//--------------------------------------------------------------------------
int cTool::copiaUnicodeToMultibyte(const WCHAR * unicodeSrc, std::string & multibyteDest)
{
    //std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;

    multibyteDest = "";

    char vChar[2] = { 0, 0 };
    WCHAR* ptrInd = (WCHAR *) unicodeSrc;

    short car = 0;
    int lon = 0;
    while (*ptrInd)
    {
        lon++;
        car = *ptrInd;
        vChar[0] = (char)car;
        multibyteDest.append(vChar);
        // Es un puntero UNICODE, esperemos que vaya de dos en dos bytes:
        ptrInd++;
    }

    return 0;
}


//--------------------------------------------------------------------------
// Suponemos, para ir resolviendo problemas iniciales
// que en destino el espacio es suficiente.
// Con el sizeUnicodeDest corregimos el control de tamaño en destino
//--------------------------------------------------------------------------
int cTool::copiaMultibyteToUnicode(const std::string& multibyteSrc, WCHAR* unicodeDest, size_t sizeUnicodeDest)
{
    WCHAR* ptrInd = unicodeDest;
    short car = 0;
    for (size_t i = 0; i < multibyteSrc.size() &&  i < sizeUnicodeDest; i++)
    {
        car = static_cast<short>(multibyteSrc[i]);
        *ptrInd = static_cast<WCHAR>(car);
        ptrInd++;
    }
    // Hemos terminado: suponemos espacio suficiente en unicodeDest, le
    // ponemos el nulo final.
    *ptrInd = static_cast<WCHAR>(0);

    return 0;
}


//====================================================================================
//  Mostramos mensajes tipo printf
//====================================================================================
// Hay que cambiar esta mecanica. Deberiamos de poder mostrar mensajes tanto en 
// modo Consola como en Windows.
//====================================================================================
int cTool::displayMensaje(HWND hWnd, uint uiTipe, const char* szTitulo, const char* pcFormato, ...)
{
    if (pcFormato == NULL)
        return 0;

    int iRes = 0;
    if (m_bDisplay)
    {
        char	vcMensaje[LON_BUFF * 2];
        va_list stLista;			/* Argument pointer */

        mInicio(vcMensaje);

        va_start(stLista, pcFormato);
        vsprintf_s(vcMensaje, sizeof(vcMensaje), pcFormato, stLista);
        va_end(stLista);

        WCHAR wcMensaje[LON_BUFF * 2];
        cTool::copiaMultibyteToUnicode(vcMensaje, wcMensaje, sizeof(wcMensaje));

        WCHAR wcTitulo[LON_BUFF / 8];
        cTool::copiaMultibyteToUnicode(szTitulo, wcTitulo, sizeof(wcTitulo));

        iRes = MessageBox(hWnd, wcMensaje, wcTitulo, uiTipe);
    }

    return iRes;
}


//====================================================================================
// Activa o desactiva los mensajes de DisplayMensaje
//====================================================================================
void cTool::setDisplay(bool bDis)
{
    m_bDisplay = bDis;
}


bool cTool::getDisplay(void)
{
    return m_bDisplay;
}


void cTool::NoDisplay(void)
{
    m_bDisplay = false;
}


void cTool::Display(void)
{
    m_bDisplay = true;
}


//--------------------------------------------------------------------------
// Extraemos las partes distintas de un path 
//--------------------------------------------------------------------------
void cTool::getCaminoFichero(const char* pcFichero, char* pcSalida, int iSizeSalida)
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
    // strncpy_s(pcSalida, iSizeSalida, (const char*)vcDir, iSizeSalida - 1);
    int iSizemenos1 = iSizeSalida - 1;
    strncpy_s(pcSalida, iSizeSalida, vcDir, iSizemenos1);
    //---------------------------------------------------
}


void cTool::getNombreFichero(const char* pcPathFichero, char* pcSalida, int iSizeSalida)
{
    char* pcCad, * pcRes, * pcTok;
    char	vcCadena[LON_BUFF];

    pcTok = nullptr;
    memset(pcSalida, 0, iSizeSalida);
    //---------------------------------------------------
    // El nombre del fichero es la Ultima cadena
    // despues del '\' o del '/'.
    //---------------------------------------------------
    mCopia(vcCadena, pcPathFichero);
    trim(vcCadena, '"');
    for (pcRes = nullptr, pcCad = strtok_s(vcCadena, "/\\", &pcTok); pcCad; pcCad = strtok_s(NULL, "/\\", &pcTok))
    {
        pcRes = pcCad;
    }
    if (pcRes)
    {
        int sizemenos1 = iSizeSalida - 1;
        strncpy_s(pcSalida, iSizeSalida, pcRes, sizemenos1);
    }
    //---------------------------------------------------
}


//--------------------------------------------------------------------------
// Cadena libre de blancos a derecha y a izquierda.
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Quitamos blancos por delante y por detras.
// Tabuladores, retornos de carro, y saltos de linea.
//
// cBlancos = 0 por defecto, entonces compara con cString::IsBlank
//
// cBlancos != 0, entonces se compara con este valor, para quitar otros
// elementos.
//
//--------------------------------------------------------------------------
void cTool::trim(char* pcCadena, char cBlancos)
{
    if (mNoVacia(pcCadena))
    {
        // Por detras.
        //------------------------------------------------------------------
        int i, k, j; 
        size_t iLen, iLen1 = (int)strlen(pcCadena);

        for (
            i = 1;
            (
                (cBlancos == 0 && cTool::isBlank(pcCadena[iLen1 - i]))
                || (cBlancos != 0 && cBlancos == pcCadena[iLen1 - i])
                ) && i < iLen1;
            i++
            )
        {
            pcCadena[iLen1 - i] = 0;
        }
        // Nueva longitud
        iLen = strlen(pcCadena);

        // Por delante
        //------------------------------------------------------------------
        // char * pcCopia = (char *) mNew(char,iLen+1,"Copia uTrim");
        int lonmas1 = (int)iLen + 1;
        char* pcCopia = new char[lonmas1];
        if (pcCopia)
        {
            memset(pcCopia, 0, lonmas1);
            for (
                j = 0;
                j < iLen &&
                (
                    (cBlancos == 0 && cTool::isBlank(pcCadena[j]))
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
            strncpy_s(pcCadena, lonmas1, pcCopia, iLen);
            delete[] pcCopia;
        }
        //------------------------------------------------------------------
    }
}


//--------------------------------------------------------------------------
// Comprobacion de caracteres
//--------------------------------------------------------------------------
bool cTool::isBlank(char pCar)
{
    int iCar = (int)pCar;

    // Hacemos que todos los que nos den valores negativos sean tambien blancos
    // a eliminar:   iCar < 0 ;

    return (iCar == ' ' || iCar == '\t' || iCar == '\n' || iCar == '\r' || iCar < 0);
}


//--------------------------------------------------------------------------
// Comprobacion de numeros
//--------------------------------------------------------------------------
// Nos responde con true si todos sus elementos son digitos, y +/- el
// primer caracter: solo numeros enteros.
// No hay puntos decimales ya que no es float.
//--------------------------------------------------------------------------
// Natural y entero (con signo).
//--------------------------------------------------------------------------
bool cTool::esEntero(const char* p_pcEntrada)
{
    if (mNoVacia(p_pcEntrada))
    {
        int iLon = (int)strlen(p_pcEntrada);

        if (iLon)
        {
            bool	bPrimero;		// Indica que es el primer caracter.
            bool	bUltimo;
            char* pcCadena;

            //--------------------------------------------------------------
            // Quitamos blancos por delante.
            //--------------------------------------------------------------
            pcCadena = (char*)p_pcEntrada;
            for (bPrimero = false; *pcCadena != 0 && !bPrimero;)
            {
                if (*pcCadena != ' ' && *pcCadena != '\n' && *pcCadena != '\t')
                {
                    bPrimero = true;
                }
                else
                    pcCadena++;
            }
            //--------------------------------------------------------------

            if (bPrimero)
            {
                //----------------------------------------------------------
                // Si el primer caracter no es digito, solo se permite que
                // sea '+' o '-'.
                //----------------------------------------------------------
                if (
                    !isdigit(*pcCadena)
                    && (*pcCadena != '-')
                    && (*pcCadena != '+')
                    )
                    return false;
                pcCadena++;

                for (bUltimo = false; *pcCadena != 0; pcCadena++)
                {
                    if (!bUltimo)
                    {
                        if (!isdigit(*pcCadena))
                        {
                            //----------------------------------------------
                            // Cualquier otro caracter solo puede ser el punto,
                            // si no es digito.
                            // (No esta controlado que haya mas de un punto)
                            //----------------------------------------------
                            if (*pcCadena == ' ' || *pcCadena == '\n' || *pcCadena == '\r' || *pcCadena == '\t')
                            {
                                bUltimo = true;
                            }
                            else
                                return false;
                        }
                    }
                    else
                    {
                        if (*pcCadena != ' ' && *pcCadena != '\n' && *pcCadena != '\r' && *pcCadena != '\t')
                            return false;
                    }
                }
                return true;
            }
        }
    }
    return false;
}


//--------------------------------------------------------------------------
// Nos responde con true si todos sus elementos son digitos.
// Hemos intentdo que acepte ya los signos y los puntos decimales
// (Al menos uno)
//--------------------------------------------------------------------------
// Natural, entero, y real
//--------------------------------------------------------------------------
bool cTool::esNumero(const char* p_pcCadena)
{
    return esFloat(p_pcCadena);
}


bool cTool::esFloat(const char* p_pcCadena)
{
    if (mNoVacia(p_pcCadena))
    {
        int iLon = (int)strlen(p_pcCadena);

        if (iLon)
        {
            bool	bPrimero;		// Indica que es el primer caracter.
            bool	bUltimo;
            bool	bPrimerPunto;
            char* pcCadena;

            //------------------------------------------------------------------
            // Quitamos blancos por delante.
            //------------------------------------------------------------------
            pcCadena = (char*)p_pcCadena;
            for (bPrimero = false; *pcCadena != 0 && !bPrimero;)
            {
                if (*pcCadena != ' ' && *pcCadena != '\n' && *pcCadena != '\t')
                {
                    bPrimero = true;
                }
                else
                    pcCadena++;
            }
            //------------------------------------------------------------------

            if (bPrimero)
            {
                bPrimerPunto = false;
                //--------------------------------------------------------------
                // Si el primer caracter no es digito, solo se permite que
                // sea '+' o '-'.
                //--------------------------------------------------------------
                if (
                    (*pcCadena == '¿')
                    || (!isdigit(*pcCadena) && (*pcCadena != '-') && (*pcCadena != '+'))
                    )
                    return false;
                pcCadena++;

                for (bUltimo = false; *pcCadena != 0; pcCadena++)
                {
                    if (!bUltimo)
                    {
                        if (!isdigit(*pcCadena))
                        {
                            //------------------------------------------------------
                            // Cualquier otro caracter solo puede ser el punto,
                            // si no es digito.
                            // (No esta controlado que haya mas de un punto)
                            //------------------------------------------------------
                            if (*pcCadena != '.')
                            {
                                if (*pcCadena == ' ' || *pcCadena == '\n' || *pcCadena == '\r' || *pcCadena == '\t')
                                {
                                    bUltimo = true;
                                }
                                else
                                    return false;
                            }
                            else
                            {
                                if (bPrimerPunto)
                                    return false;
                                bPrimerPunto = true;
                            }
                        }
                    }
                    else
                    {
                        if (*pcCadena != ' ' && *pcCadena != '\n' && *pcCadena != '\r' && *pcCadena != '\t')
                            return false;
                    }
                }
                return true;
            }
        }
    }
    return false;
}


//--------------------------------------------------------------------------
//  Convierte toda una cadena a mayusculas,
//--------------------------------------------------------------------------
cstatic int cTool::mayusculas(char* p_pcCadena)
{
    if (mNoVacia(p_pcCadena))
    {
        int i, iLon = (int)strlen(p_pcCadena);

        for (i = 0; i < iLon; i++)
        {
            p_pcCadena[i] = toupper(p_pcCadena[i]);
        }
    }
    return 0;
}


cstatic int cTool::mayusculas(char* p_pcCadena, int iLon)
{
    if (mNoVacia(p_pcCadena))
    {
        int i;

        for (i = 0; i < iLon; i++)
        {
            p_pcCadena[i] = toupper(p_pcCadena[i]);
        }
    }
    return 0;
}


//--------------------------------------------------------------------------
//  Convierte toda una cadena a mayusculas,
//--------------------------------------------------------------------------
cstatic int cTool::minusculas(char* p_pcCadena)
{
    if (mNoVacia(p_pcCadena))
    {
        int i, c, iLon = (int)strlen(p_pcCadena);

        for (i = 0; i < iLon; i++)
        {
            c = tolower(p_pcCadena[i]);
            p_pcCadena[i] = c;
        }
    }
    return 0;
}


cstatic int cTool::minusculas(char* p_pcCadena, int iLon)
{
    if (mNoVacia(p_pcCadena))
    {
        int i;

        for (i = 0; i < iLon; i++)
        {
            p_pcCadena[i] = tolower(p_pcCadena[i]);
        }
    }
    return 0;
}


/*========================================================================*\
|* Fin de cTool.cpp
\*========================================================================*/
