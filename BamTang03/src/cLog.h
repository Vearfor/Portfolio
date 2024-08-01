/*========================================================================*\
|* cLog.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include <stdio.h>
#include <stdarg.h>
#include <string>
//--------------------------------------------------------------------------
//#include <string>
//#include "cFile.h"
//#include "../Procesos/cCSeccion.h"
//#include "../Util/cString.h"
//#include "../Listas/tLista.h"
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cLog
{
    static int escribeLista(va_list pstList, const char* pcFormat);

    static  int     open(void);
    static  int     close(void);

public:

    cLog();
    ~cLog();

    static FILE* m_pFile;
    static std::string m_pPathLog;

    static  int print(const char* pcFormat, ...);
    static  int log(const char* pcFormat, ...);
    static  int error(const char* pcFormat, ...);

    static int openLog(const char* pcFormat);
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cLog.h
\*========================================================================*/
