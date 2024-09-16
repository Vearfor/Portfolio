/*------------------------------------------------------------------------*\
|* sString.h
|*
|*  Created on: 07/12/2015
|*      Author: Enrique Rocafull Ortiz
\*-----------------------------------------------------------------------*/


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "../nComun.h"
#include <vector>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------
// #define mInicio(vcad)		memset(static_cast<void*>(&vcad), 0, sizeof(vcad));
//--------------------------------------------------------------------------
// #define mInicio(vcad)		memset(vcad, 0, sizeof(vcad));
//--------------------------------------------------------------------------
//#define mCopia(dest,src)	\
//    sString::strncpy(dest,sizeof(dest),src,sizeof(dest)-1)
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Para la sobrecarga del sString::format()
// Tipo para encadenar las va_list y 
// diferenciarlas de const char * pcFormat
// Dos sobrecargas:
// - sString & format(const char * pcFormat, ...);
// - sString & format(TVaList & tList, const char * pcFormat);
//--------------------------------------------------------------------------
typedef struct _mExport stVaList
{
    va_list stList;

} TVaList;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// class
//--------------------------------------------------------------------------
// template<class T> class _mExport cLista;
//--------------------------------------------------------------------------
struct _mExport sString
{
private:

    char  * m_pcCad;    // Se reserva Longitud+1 para el nulo.
    int     m_iLon;     // Longitud sin el nulo
    bool    m_bMeTienesQueBorrar;

    void    Del ( void ) const;

public:

    sString ( bool bMarcaOperador = false );
    sString ( const char * pcCad, int lon=-1, bool bMarcaOperador = false );
    sString ( const sString & aCopiar );
    virtual ~sString ( void );

    static char Empty[LON_BUFF];
    // static char * Empty;

    //----------------------------------------------------------------------
    // Statics
    //----------------------------------------------------------------------
    static int longitud(va_list pstList, const char * pcFormat);
    static int strcpy(char * pcDest, int sizeDest, const char * pcSrc);
    static int strncpy(char * pcDest, int sizeDest, const char * pcSrc, int count);
    static int strncat(char * pcDest, int sizeDest, const char * pcSrc, int count);
    static int strcmp(const char * pcCad1, const char * pcCad2);
    static int stricmp(const char * pcCad1, const char * pcCad2);
    static int strlen(const char * pCad);
    static void replace(char * pcCad, char cSrc, char cDest);

    static void formatLista(char * pcDestino, int iLonDestino, va_list pstList, const char * pcFormat);
    static void formatCad(char * pcDestino, int iLonDestino, const char * pcFormat, ...);

    static bool sortString(const sString & sNew, const sString & sOld);
    static bool sortString(const char * pcNew, const char * pcOld);
    //----------------------------------------------------------------------

    char* cad(void) const { return m_pcCad; }
    char car(int pos) const;
    int  lon(void) const { return m_iLon; }
    void limpia ( void );
    void reset (void);
    void vacia (void);
    int load (int lon);
    int load ( const char * pcCad, int lon=-1 );
    int concat ( const char * pcCad, int lon=-1 );
    const char* substr(int numPos);

    // sString & format(va_list pstList, const char * pcFormat);
    sString & format(const char * pcFormat, ...);
    sString & format(TVaList & stList, const char * pcFormat);

    bool esVacia(void);
    void trim(char cBlancos = 0);

    //----------------------------------------------------------------------
    void minusculas(void);
    void mayusculas(void);
    //--------------------------------------------------------------------------
    long  Numero  (void);
    float  Float  (void);
    double  Double  (void);
    //----------------------------------------------------------------------
    bool esNumero(void);
    bool esFloat(void);
    bool esDouble(void);
    //----------------------------------------------------------------------

    std::vector<sString> * split(const char ** ppcSep, int iNumSep, bool bAdmiteCadenaVacia = false);

    //----------------------------------------------------------------------
    // Sobrecarga de operadores
    //----------------------------------------------------------------------
    sString & operator = ( const sString & aCopiar );
    sString & operator = ( const char * pcCad );
    sString & operator = (int iNum);

    void operator += (const sString & aCopiar);
	void operator += (const char * pcCad);
    void operator += (int iNum);

    sString & operator + ( const sString & aCopiar );
    sString & operator + (const char * pcCad);
    sString & operator + (int iNum);

    bool operator < (const sString & s) const;

    bool operator == (const sString & s) const;
    bool operator == (const char * cad) const;
    bool operator == (int iNum) const;

    bool operator != (const sString & s) const;
    bool operator != (const char * cad) const;
    bool operator != (int iNum) const;

    //----------------------------------------------------------------------
    char* operator* () const;
    //----------------------------------------------------------------------
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Tipo Cadenas identificadas con identificador unico
//--------------------------------------------------------------------------
struct _mExport TString
{
    int iIdent;
    sString cadena;

    TString()
    {
        iIdent = -1;
        cadena = "";
    }

};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// class
//--------------------------------------------------------------------------
class _mExport iToString
{
protected:

    iToString(void);

public:

    virtual const char * toString(char * vcSalida, int lSize) = 0;
    virtual const char * toString(sString & salida) = 0;

    const char * toChar(const sString & entrada, sString & salida);
};
//--------------------------------------------------------------------------


/*------------------------------------------------------------------------*\
|* Fin de sString.h
\*------------------------------------------------------------------------*/

