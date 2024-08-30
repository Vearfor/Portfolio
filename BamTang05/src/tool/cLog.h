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


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cLog
{
public:

    enum class eTraza : uint8_t
    {
        min = 0,
        nor = 1,
        max = 2
    };

    enum class eOut : uint8_t
    {
        std = 1,
        err = 2,
        fil = 4,
    };

private:

    static  int     open(void);
    static  int     close(void);

public:

    cLog();
    ~cLog();

    static FILE* m_pFile;
    static std::string m_pPathLog;
    static eTraza m_eNivelTraza;
    static bool m_escribeLog;

    static int escribeLista(cLog::eTraza eNivel, cLog::eOut output, va_list pstList, const char* pcFormat);
    static  bool siTraza(cLog::eTraza traza);
    static  int traza(cLog::eTraza eNivel, const char* pcFormat, ...);
    static  int print(const char* pcFormat, ...);
    static  int log(const char* pcFormat, ...);
    static  int error(const char* pcFormat, ...);

    static int openLog(const char* pcFormat);
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cLog.h
\*========================================================================*/
