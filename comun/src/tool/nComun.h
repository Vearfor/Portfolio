/*========================================================================*\
|* nComun.h
|*
|*  Created on: 07/12/2015
|*      Author: Enrique Rocafull Ortiz
\*========================================================================*/


#ifndef NCOMUN_H_
#define NCOMUN_H_
//==========================================================================
// Includes
//==========================================================================
#include <dll/nExport.h>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <cstring>
//--------------------------------------------------------------------------


//==========================================================================
// Defines
//==========================================================================
#define LON_BUFF            1024
#define cstatic
#define coverride
#define	AM_INICIO_MENSAJES  WM_USER + 1000
//==========================================================================


//==========================================================================
// Macros
//==========================================================================
#define mEsVacia(s)         (s==NULL || *s==0)
#define mNoVacia(s)         (s!=NULL && *s!=0)
//--------------------------------------------------------------------------
#define mSizeArray(varray)  (sizeof(varray)/sizeof(varray[0]))
//--------------------------------------------------------------------------
#define miError(punt)       { if (punt!=0) { return -1; } }
#define miNulo(punt)        { if (!punt) { return -1; } }
#define mpError(punt)       { if (punt!=0) { return nullptr; } }
#define mpNulo(punt)        { if (!punt) { return nullptr; } }
//--------------------------------------------------------------------------
#define mInicio(s)          std::memset(s, 0, sizeof(s))
//--------------------------------------------------------------------------
#define mCopia(dest,src)	\
    strncpy_s(dest, sizeof(dest), src, sizeof(dest)-1)
//--------------------------------------------------------------------------
// cString::strncpy(dest,sizeof(dest),src,sizeof(dest)-1)
//--------------------------------------------------------------------------
#define mDo(punt)           if (punt) punt
//--------------------------------------------------------------------------
#define mDelete(mem)        { if (mem) { delete mem; mem=nullptr; } }
#define mDeleteV(mem)       { if (mem) { delete[] mem; mem=nullptr; } }
//--------------------------------------------------------------------------
#define mbTrue(v)           ((!strcmp(v,"true"))? true: false)
#define msTrue(s)           ((s)? "true": "false")
//==========================================================================

/*------------------------------------------------------------------------*\
|* Macros para mantener/crear clases Singleton 
\*------------------------------------------------------------------------*/
//--------------------------------------------------------------------------
// Declaracion de m_poInstancia y de Instancia:
// 
// En el Header, en la parte public de la clase ponemos:
// 
// mSingletonHeader(<nombre-clase-singleton>)
// 
// Y en el Cpp, ya debajo de constructor y destructor, ponemos:
// 
// mSingleton(<nombre-calse-singleton>)
// 
//--------------------------------------------------------------------------
#define mSingletonDeleteHeader  \
static void * Delete(void);\
static bool Existe(void);
//--------------------------------------------------------------------------
#define	mSingletonHeader(Clase) \
static Clase * m_poInstancia;	\
static Clase * Instancia(void); \
mSingletonDeleteHeader
//--------------------------------------------------------------------------
#define mSingleton(Clase)               \
Clase * Clase::m_poInstancia = NULL;    \
Clase * Clase::Instancia ( void )       \
{                                       \
    if	(m_poInstancia==NULL)           \
    {                                   \
        m_poInstancia = new Clase();    \
    }                                   \
    return m_poInstancia;               \
}                                       \
void * Clase::Delete(void)              \
{                                       \
    if (m_poInstancia)                  \
    {                                   \
        delete m_poInstancia;           \
        m_poInstancia = NULL;           \
    }                                   \
    return NULL;                        \
}                                       \
bool Clase::Existe(void)                \
{                                       \
    return (!!m_poInstancia);           \
}
//--------------------------------------------------------------------------
#define mSingletonDelete(Clase)     \
void * Clase::Delete(void)          \
{                                   \
    if (m_poInstancia)              \
    {                               \
        delete m_poInstancia;       \
        m_poInstancia = NULL;       \
    }                               \
    return NULL;                    \
}                                   \
bool Clase::Existe(void)            \
{                                   \
    return (!m_poInstancia);        \
}
//--------------------------------------------------------------------------
#define mSingletonNull          \
if (m_poInstancia == this)      \
{                               \
    m_poInstancia = NULL;       \
}
/*------------------------------------------------------------------------*/




//==========================================================================
// Typedefs
//==========================================================================
// typedef unsigned long       ulong;
// typedef WORD                word;
// typedef DWORD               dword;
//==========================================================================
using ulong = unsigned long;
using uint = unsigned int;
using word = WORD;
using dword = DWORD;
using byte = unsigned char;
//--------------------------------------------------------------------------
// Tipos funciones ...
//--------------------------------------------------------------------------
// typedef int(*piProcP)(void*);
//--------------------------------------------------------------------------
using piProcP = int (*)(void*);
//==========================================================================


//--------------------------------------------------------------------------
#endif /* NCOMUN_H_ */


/*========================================================================*\
|* Fin de nComun.h
\*========================================================================*/
