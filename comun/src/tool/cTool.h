/*========================================================================*\
|* cTool.h
|*
|*  Created on: 23/08/2024
|*      Author: Enrique Rocafull Ortiz
\*========================================================================*/


#pragma once
//==========================================================================
// Includes
//==========================================================================
#include "nComun.h"
#include <string>
//==========================================================================


//==========================================================================
// static class
//==========================================================================
class cTool
{
    static bool m_bDisplay;

public:

    //----------------------------------------------------------------------
    static int copiaUnicodeToMultibyte(const WCHAR * unicodeSrc, std::string & multibyteDest);
    static int copiaMultibyteToUnicode(const std::string& multibyteSrc, WCHAR* unicodeDest, size_t sizeUnicodeDest);
    //----------------------------------------------------------------------
    static int displayMensaje(HWND hWnd, uint uiTipe, const char* szTitulo, const char* pcFormato, ...);
    static void setDisplay(bool bDis);
    static bool getDisplay(void);
    static void NoDisplay(void);
    static void Display(void);
    //----------------------------------------------------------------------
    static void getCaminoFichero(const char* pcFichero, char* pcSalida, int iSizeSalida);
    static void getNombreFichero(const char* pcPathFichero, char* pcSalida, int iSizeSalida);
    static void trim(char* pcCadena, char cBlancos = 0);
    static bool isBlank(char pcCar);
    //--------------------------------------------------
    static bool esEntero(const char* pcCadena);
    static bool esNumero(const char* pcCadena);
    static bool esFloat(const char* pcCadena);
    //----------------------------------------------------------------------
};
//==========================================================================


/*========================================================================*\
|* Fin de cTool.h
\*========================================================================*/
