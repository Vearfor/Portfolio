/*
 * cConsola.h
 *
 *  Created on: 07/12/2015
 *      Author: Enrique Rocafull Ortiz
 */

#ifndef CCONSOLA_H_
#define CCONSOLA_H_

//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "nComun.h"
#include "cLog.h"
#include <string.h>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Defines
//--------------------------------------------------------------------------
// Algunos defines de teclas
//--------------------------------------------------------------------------
#define TF1             59
#define TRETURN         13
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enumerados
//--------------------------------------------------------------------------
enum eModConsola
{
    eMOD_CONSOLA = 0,
    eMOD_WINDOWS,
};

enum eCursor
{
	eMAXCONSOLA = -1,
	eMINCONSOLA = -2,
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// class
//--------------------------------------------------------------------------
class cConsola
{
    static  eModConsola m_eModo;
    static  bool        m_bActiva;
    static  bool        m_bOpened;
    static  bool        m_bVisible;
    static  char        m_vcTitulo[LON_BUFF];           // Titulo inicial
    static  char        m_vcTituloNormal[LON_BUFF];
    static  char        m_vcNombreProceso[LON_BUFF/8];

    //static  HANDLE  m_hOutHandle;
    //static  HANDLE  m_hInHandle;
    //static  HANDLE  m_hNewHandle;
    static  HWND        m_hWindow;

    static  bool    m_bAllocConsoleOriginal;

    static  int AllocConsole(bool bOriginal = true);
    static  int FreeConsole();
    static  int longitud(va_list pstList, const char* pcFormat);

public:

    // Como va a ser unica haremos que todos los metodos sean estaticos
    // No deberia de necesitar constructor, ya que no deberia necesitar
    // destruirse (ocultar la consola al cerra? lo hara solo? habria que liberar recursos?)

    // Vamos a darle constructor y ya veremos que pasa ...
    cConsola(void);
    ~cConsola(void);

    static int kbhit(void);
    static int getch(void);
    static int PulsaTecla(const char * pcFormat = NULL, ...);

    static eModConsola getModo(void);
    static void setModo(eModConsola eModo);
    static bool EsActiva(void);
    static void SetActiva(bool bActiva);

    static int Open(void);
    static int Close(void);
    static int Mostrar(void);
    static int Ocultar(void);
    static int Show(void) { return cConsola::Mostrar(); }
    static int Hide(void) { return cConsola::Ocultar(); }

    //static  HANDLE  GetOutHandle(void) { return m_hOutHandle; }
    //static  HANDLE  GetInHandle(void) { return m_hInHandle; }
    //static  HANDLE  GetNewHandle(void) { return m_hNewHandle; }

	static int escribe(const char * pcFormat, ...);
	static int escribeXY(int x, int y, const char * pcFormat, ...);
    static int error(const char * pcFormat, ...);

    static int escribeListaXY(int x, int y, va_list pstList, const char * pcFormat);
    static int escribeLista(cLog::eOut output, va_list pstList, const char * pcFormat);
	static int cls(int iTraza = 0);
    static int setTituloConsola(const char * pcFormato, ...);
    static int resetTituloConsola(const char * pcTitulo = NULL);
    static const char * getTituloNormal(void);

    static void setNombreProceso(const char* pcNombreProceso);
    static const char* getNombreProceso(void);
};
//--------------------------------------------------------------------------

#endif /* CCONSOLA_H_ */
