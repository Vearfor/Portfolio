/*========================================================================*\
|* sString.cpp
|*
|* Vamos a tener que hacerlo como lo teniamos en OrionWork: cString
|* Lo vamos a fusilar, porque, en principio, funcionaba.
\*========================================================================*/


#include "sString.h"
#include "../cTool.h"


//--------------------------------------------------------------------------
// Carga
//--------------------------------------------------------------------------
int sString::load(const char* pcBuffer, int longitud)
{
    limpia();
    *this = pcBuffer;
    if (longitud < 1)
    {
        m_lon = (int)strlen(pcBuffer);
    }
    else
    {
        m_lon = longitud;
    }
    return 0;
}


int sString::limpia()
{
    *this = "";
    m_lon = 0;
    return 0;
}


//--------------------------------------------------------------------------
// Metodos/Funciones de format
//--------------------------------------------------------------------------
sString& sString::format(const char* pcFormat, ...)
{
    if (mNoVacia(pcFormat))
    {
        TVaList tList;

        va_start(tList.stList, pcFormat);
        format(tList, pcFormat);
        va_end(tList.stList);
    }

    return *this;
}


/*!
*  Es lo mismo que format, pero construye el contenido del string
*  con los valores de una lista de argumentos pasados por parametros
*/
sString& sString::format(TVaList& tList, const char* pcFormat)
{
    if (mNoVacia(pcFormat))
    {
        int lonFinal = sString::longitud(tList.stList, pcFormat);
        int lonmas1 = lonFinal + 1;
        if (lonFinal > 0)
        {
            char* pcBuffer = new char[lonmas1];
            pcBuffer[lonmas1] = 0;
            if (pcBuffer)
            {
                // Y ahora si copiamos
                lonFinal = vsnprintf_s(pcBuffer, lonmas1, lonFinal, pcFormat, tList.stList);
                load(pcBuffer, lonFinal);
                delete[] pcBuffer;
            }
        }
    }

    return *this;
}


//--------------------------------------------------------------------------
// Limpieza de blancos del contenido de sString
//--------------------------------------------------------------------------
void sString::trim(char blancos)
{
    char* pcCad = const_cast<char*>((*this).c_str());
    cTool::trim(pcCad, blancos);
    load(pcCad, (int) strlen(pcCad));
}


//--------------------------------------------------------------------------
// Calculo de longitud de cadenas variables
//--------------------------------------------------------------------------
cstatic int sString::longitud(va_list pstList, const char* pcFormat)
{
    int lonFinal = 0;
    if (mNoVacia(pcFormat))
    {
        lonFinal = vsnprintf(NULL, 0, pcFormat, pstList);
    }
    return lonFinal;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Metodos de sobrecarga de operador
//--------------------------------------------------------------------------
sString& sString::operator= (const char* pcCad)
{
    load(pcCad, (int) strlen(pcCad));
    return *this;
}
//--------------------------------------------------------------------------


////--------------------------------------------------------------------------
//// Metodos de sobrecarga de operador
////--------------------------------------------------------------------------
// * Sobrecarga de operador de asignacion: sString &
// * @param aCopiar
// * @return
// 
//sString& sString::operator= (const sString& aCopiar)
//{
//    if (this != &aCopiar)
//    {
//        *this = aCopiar;
//    }
//    return *this;
//}
//
//
// * Sobrecarga de operador de asignacion: char *
// * @param pcCad
// * @return
// 
//sString& sString::operator= (const char* pcCad)
//{
//    *this = pcCad;
//
//    return *this;
//}
//
//
//sString& sString::operator= (int iNum)
//{
//    char vcNum[LON_BUFF];
//    mInicio(vcNum);
//    sprintf_s(vcNum, sizeof(vcNum), "%d", iNum);
//    *this = vcNum;
//
//    return *this;
//}
//
//
//
//
// * Sobrecarga de operador de adicion y asignacion: sString &
// * @param aCopiar
// * @return
//
//void sString::operator += (const sString& aCopiar)
//{
//    concat(aCopiar.Cad(), aCopiar.Lon());
//
//    // Fin Control pila de operador +
//    aCopiar.Del();
//}
//
//
//
// * Sobrecarga de operador de adicion y asignacion: char *
// * @param pcCad
// * @return
//
//void sString::operator += (const char* pcCad)
//{
//    concat(pcCad);
//}
//
//
//void sString::operator += (int iNum)
//{
//    char vcNum[LON_BUFF];
//    mInicio(vcNum);
//    sprintf_s(vcNum, sizeof(vcNum), "%d", iNum);
//    concat(vcNum);
//}
//
//
//sString& sString::operator+ (const sString& oString)
//{
//    sString* pNewString;
//    if (m_bMeTienesQueBorrar)
//    {
//        pNewString = this;
//    }
//    else
//    {
//        pNewString = new sString(true);
//        pNewString->load(m_pcCad, m_iLon);
//    }
//    pNewString->concat(oString.Cad(), oString.Lon());
//
//    // Control pila de operador +
//    oString.Del();
//    return *pNewString;
//}
//
//
//sString& sString::operator+ (const char* pcCad)
//{
//    sString* pNewString;
//    if (m_bMeTienesQueBorrar)
//    {
//        pNewString = this;
//    }
//    else
//    {
//        pNewString = new sString(true);
//        pNewString->load(m_pcCad, m_iLon);
//    }
//    pNewString->concat(pcCad);
//
//    return *pNewString;
//}
//
//
//sString& sString::operator+ (int iNum)
//{
//    sString* pNewString;
//    if (m_bMeTienesQueBorrar)
//    {
//        pNewString = this;
//    }
//    else
//    {
//        pNewString = new sString(true);
//        pNewString->load(m_pcCad, m_iLon);
//    }
//    char vcNum[LON_BUFF];
//    mInicio(vcNum);
//    sprintf_s(vcNum, sizeof(vcNum), "%d", iNum);
//    pNewString->concat(vcNum);
//
//    return *pNewString;
//}


/*========================================================================*\
|* Fin de sString.cpp
\*========================================================================*/
