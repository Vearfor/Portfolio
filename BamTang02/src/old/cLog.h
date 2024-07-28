/*========================================================================*\
|* cLog.h
\*========================================================================*/
//--------------------------------------------------------------------------
// Control del fichero Log de los programas
//--------------------------------------------------------------------------


#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include "cFile.h"
#include "../Procesos/cCSeccion.h"
#include "../Util/cString.h"
#include "../Listas/tLista.h"
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class _mExport cLog : public cFile
{
    cString m_sDirectorio;

    static cLog * m_poInstancia;

    virtual int init(const char * pcNombreCompletoFichero, eFileMode eMode);

    void setDir(void);
    int creaDir();

    cLog();

public:

    virtual ~cLog();

// Esta parte no me funciona con D_MINGW_
//    static  cLog  * Instancia(void);
//#ifndef _MINGW_
//    static  cLog  * Instancia(const char * pcFormatoFicheroLog, ...);
//#else
//    // Lo de arriba me casca en MinGW
//    static  cLog  * Instancia(const char * pcFormatoFicheroLog);
//#endif

    static  cLog  * Instancia(void);
    static  cLog  * Instancia(const char * pcFormatoFicheroLog, ...);

    static  int     print(const char * pcFormat, ...);
    static  int     error(const char * pcFormat, ...);
    static  int     traza(int iNivel, const char * pcFormat, ...);
    static  bool    siTraza(int iNivel);
    static  int     appendLog(void);
    static  int     closeLog(void);
    static  void    showLog(int iNivel = -1);
    static  void    hideLog(void);
    static  bool    isShow(void);
    static  void    setShow(bool bShow);
    static  void    setShowTime(bool bShow);
    static  void    showTime(void);
    static  void    hideTime(void);
    static  bool    isShowTime(void);

    static  const char * getDir(void);

    static void    memoryLog(void);
    static void    memory(const char * pcFicLog, bool bShow = false);
    static void    memory(cFile * poFile);
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
// Lista de Logs por hilo:
// - Logs especificos y diferentes, pensado para ejecutar la traza de un
//   CICLO de hilo (de ahi el nombre).
//--------------------------------------------------------------------------
// Los quito
//--------------------------------------------------------------------------
//class _mExport cCiclo
//{
//    cCiclo();
//
//    static bool m_bActivo;
//    static cCSection * m_poSeccion;
//    static cListaP<cFile> * m_poListaLogs;
//
//    // void setDir(void);
//
//    cFile * buscaFileThread(dword dwThreadId);
//
//public:
//
//    virtual ~cCiclo();
//
//    mSingletonHeader(cCiclo);
//
//    static int inicio(const char * pcNombre);
//    static int fin(void);
//
//    static int print(const char * pcFormat, ...);
//    static void pausa(bool activaPausa);
//    static bool esActivo(void);
//
//    // static  cCiclo* Instancia(const char * pcNuevoFicheroLog = NULL);
//    // static  int     traza(int iNivel, const char * pcFormat, ...);
//    // static  bool    siTraza(int iNivel);
//    // static  int     appendLog(void);
//    // static  int     closeLog(void);
//    // static  void    showLog(int iNivel = -1);
//    // static  void    hideLog(void);
//    // static  bool    isShow(void);
//    // static  void    setShow(bool bShow);
//    // static  void    memoryLog(void);
//    // static  void    memory(const char * pcFicLog, bool bShow = false);
//    // static  void    memory(cFile * poFile);
//    // static  void    setShowTime(bool bShow);
//    // static  void    showTime(void);
//    // static  void    hideTime(void);
//    // static  bool    isShowTime(void);
//    //static  const char * getDir(void);
//};
////--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cLog.h
\*========================================================================*/
