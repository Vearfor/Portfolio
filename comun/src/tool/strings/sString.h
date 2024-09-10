#pragma once
/*========================================================================*\
|* sString.h
|*
|*  Created on: 08/09/2024
|*      Author: Enrique Rocafull Ortiz
|*
|* Ya no hace falta, pero lo dejo por si las moscas.
|*
|* A diferencia del sString que controlaba la memoria, este lo estamos
|* haciendo para incorporarlo al cValor
|* por el error del
|*
|* 'UValor::UValor(void)': se está intentando hacer referencia a una función eliminada.
|*
|* en la union UValor de cValor.
\*========================================================================*/


#pragma once
//==========================================================================
// Includes
//==========================================================================
#include "../nComun.h"
#include <string>
//==========================================================================


//--------------------------------------------------------------------------
// Para la sobrecarga del sString::format()
// Tipo para encadenar las va_list y 
// diferenciarlas de const char * pcFormat
// Dos sobrecargas:
// - sString & format(const char * pcFormat, ...);
// - sString & format(TVaList & tList, const char * pcFormat);
//--------------------------------------------------------------------------
struct _mExport TVaList
{
    va_list stList;

};
//--------------------------------------------------------------------------


//==========================================================================
// Primero vemos si nos cuela por herencia
//==========================================================================
struct _mExport sString
    : public std::string
{
    int m_lon{ 0 };

    int lon() { return m_lon; }

    int load(const char* pcBuffer, int longitud);
    int limpia();
    sString& format(const char* pcFormat, ...);
    sString& format(TVaList& stList, const char* pcFormat);
    void trim(char blanco=0);

    static int longitud(va_list pstList, const char* pcFormat);

    //----------------------------------------------------------------------
    // Sobrecarga de operadores
    //----------------------------------------------------------------------
    sString& operator = (const char* pcCad);
    //----------------------------------------------------------------------
};
//==========================================================================


/*========================================================================*\
|* Fin de sString.h
\*========================================================================*/
