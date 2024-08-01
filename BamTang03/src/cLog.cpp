/*========================================================================*\
|* cLog.cpp
\*========================================================================*/
//--------------------------------------------------------------------------
// Control del fichero Log de los programas
//--------------------------------------------------------------------------


#include "cLog.h"
#include "cConsola.h"
#include "cConio.h"
#include "nComun.h"

//#include "cDir.h"
//#include "../Devices/cConio.h"
//#include "../Util/cError.h"
//#include "../Util/cUtil.h"
//#include "../Memoria/fMemoria.h"
/*========================================================================*\
|* Statics
\*========================================================================*/
FILE* cLog::m_pFile = nullptr;
std::string cLog::m_pPathLog = "";
/*========================================================================*/


/*========================================================================*\
|* cLog
\*========================================================================*/
cLog::cLog()
{
    cConsola::Open();
}

cLog::~cLog()
{
    cConsola::Close();
    if (m_pFile)
    {
        fclose(m_pFile);
    }
}


int cLog::escribeLista(va_list pstList, const char* pcFormat)
{
    return cConsola::escribeLista(pstList, pcFormat);
}

//--------------------------------------------------------------------------
// Escritura directa en la salida estandar de log
//--------------------------------------------------------------------------
int cLog::print(const char * pcFormat, ...)
{
    if (mNoVacia(pcFormat))
    {
        va_list stList;

        va_start(stList, pcFormat);
        //------
        escribeLista(stList, pcFormat);
        //------
        va_end(stList);
    }
    return 0;
}


//--------------------------------------------------------------------------
// Escritura directa en el fichero de log
//--------------------------------------------------------------------------
int cLog::log(const char* pcFormat, ...)
{
    if (mNoVacia(pcFormat) && m_pFile)
    {
        va_list stList;

        va_start(stList, pcFormat);
        //------
        escribeLista(stList, pcFormat);
        //------
        va_end(stList);
    }
    return 0;
}


// Se diferencia de la anterior en que la salida a Consola, si la tiene,
// sera con colores de error:
int cLog::error(const char * pcFormat, ...)
{
    if (mNoVacia(pcFormat))
    {
        va_list stList;
        va_start(stList, pcFormat);
        //------
        cConio::SetColor(cConio::GetErrorColor());
        escribeLista(stList, pcFormat);
        cConio::SetColor(cConio::GetNormalColor());
        //------
        va_end(stList);
    }
    return 0;
}


int cLog::openLog(const char* pcPathFile)
{
    fopen_s(&m_pFile, pcPathFile, "w");
    if (!m_pFile)
    {
        fprintf(stderr, " cLog::opelog: Error: fopen: no puede abrir el fichero: %s\n", pcPathFile);
        return -1;
    }

    m_pPathLog = pcPathFile;

    return 0;
}


/*========================================================================*\
|* Fin de cLog.cpp
\*========================================================================*/
