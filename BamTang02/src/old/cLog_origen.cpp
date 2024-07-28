/*------------------------------------------------------------------------*\
|* cLog.cpp
\*------------------------------------------------------------------------*/

#include "cLog.h"
#include <stdarg.h>

//--------------------------------------------------------------------------
// int cLog::escribeLista(int iNivelTraza, FILE* stdErrOut, va_list pstList, const char* pcFormat)
//--------------------------------------------------------------------------
int cLog::escribeLista(FILE* stdErrOut, va_list pstList, const char* pcFormat)
{
    // if (hayTraza(iNivelTraza))
    {
        char vMem[LON_BUFF * 2];
        mInicio(vMem);
//#ifdef _WINDOWS
//        vsprintf_s(vMem, sizeof(vMem), pcFormat, pstList);
//#else
//        vsprintf(vMem, pcFormat, pstList);
//#endif
        vsprintf_s(vMem, sizeof(vMem), pcFormat, pstList);

        //if (s_outputMode == OutputMode::ONLY_LOG || s_outputMode == OutputMode::LOG_AND_SCREEN)
        //{
        //    logLista(vMem);
        //}

        //if (stdErrOut && (s_outputMode == OutputMode::ONLY_SCREEN || s_outputMode == OutputMode::LOG_AND_SCREEN))
        //{
        //    screenLista(stdErrOut, vMem);
        //}

        screenLista(stdErrOut, vMem);
    }
    return 0;
}


//--------------------------------------------------------------------------
int cLog::screenLista(FILE* stdErrOut, const char* mensaje)
{
    if (stdErrOut)
    {
        fprintf(stdErrOut, "%s", mensaje);
        fflush(stdErrOut);
    }
    return 0;
}


//--------------------------------------------------------------------------
int cLog::log(const char* pcFormat, ...)
{
    if (pcFormat)
    {
        va_list stList;
        va_start(stList, pcFormat);
        escribeLista(stdout, stList, pcFormat);
        va_end(stList);
    }
    return 0;
}


//--------------------------------------------------------------------------
int cLog::error(const char* pcFormat, ...)
{
    if (pcFormat)
    {
        va_list stList;
        va_start(stList, pcFormat);
        escribeLista(stderr, stList, pcFormat);
        va_end(stList);
    }
    return 0;
}


/*------------------------------------------------------------------------*\
|* Fin de cLog.cpp
\*------------------------------------------------------------------------*/
