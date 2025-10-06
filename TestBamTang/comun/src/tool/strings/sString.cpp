/*------------------------------------------------------------------------*\
|* sString.cpp
|*
|*  Created on: 07/12/2015
|*      Author: Enrique Rocafull Ortiz
\*------------------------------------------------------------------------*/
// #define _CRT_SECURE_NO_WARNINGS

#include "sString.h"
#include "../cTool.h"
//#include <ctype.h>
//#include <conio.h>
//#include <stdarg.h>
#include "cToken.h"
//#include "cUtil.h"
//#include "../Files/cFile.h"
//#include "../Memoria/fMemoria.h"
//#include "../Consola/cConsola.h"
//#include "../Listas/tLista.h"

#if defined(_MINGW_)
#include "../Deprecated/fDeprecated.h"
#endif


//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
char sString::Empty[] = "";
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
sString::sString ( bool bMeTienesQueBorrar )
    : m_pcCad(nullptr)
    , m_iLon(0)
    , m_bMeTienesQueBorrar(bMeTienesQueBorrar)
{
}

sString::~sString ( void )
{
    limpia();
}


//--------------------------------------------------------------------------
// Constructor con inicializadores
//--------------------------------------------------------------------------
sString::sString(const char * pcCad, int lon, bool bMeTienesQueBorrar)
    : m_pcCad(nullptr)
    , m_iLon(0)
    , m_bMeTienesQueBorrar(bMeTienesQueBorrar)
{
    load(pcCad, lon);
}


//--------------------------------------------------------------------------
// Constructor de copia
//--------------------------------------------------------------------------
sString::sString ( const sString & aCopiar )
    : m_pcCad(nullptr)
    , m_iLon(0)
    , m_bMeTienesQueBorrar(false)
{
    load(aCopiar.m_pcCad, aCopiar.m_iLon);
    m_bMeTienesQueBorrar = aCopiar.m_bMeTienesQueBorrar;
}


/*========================================================================*\
|* Metodos Statics, sin instancia ...
\*========================================================================*/
//--------------------------------------------------------------------------
// Calculo de longitud de cadenas variables
//--------------------------------------------------------------------------
cstatic int sString::longitud(va_list pstList, const char * pcFormat)
{
	int lonFinal = 0;
	if (mNoVacia(pcFormat))
	{
        lonFinal = vsnprintf(NULL, 0, pcFormat, pstList);
	}
	return lonFinal;
}


cstatic int sString::strcpy(char * pcDest, int sizeDest, const char * pcSrc)
{
    // Si este ya limpia
    memset(pcDest, 0, sizeDest);
    // Me vale preguntar que este esta vacio:
    // Nos da igual que pregunte:
    if (pcDest && mNoVacia(pcSrc) && sizeDest>0)
    {
        int lonSrc;
        int lonFinal;

		lonSrc = sString::strlen(pcSrc);
        lonFinal = (lonSrc>sizeDest-1)? sizeDest-1: lonSrc;
        memcpy_s(pcDest, sizeDest, pcSrc, lonFinal);
        // Nos aseguramos que pone el nulo final
        pcDest[lonFinal] = 0;
    }

    return 0;
}


cstatic int sString::strncpy ( char * pcDest, int sizeDest, const char * pcSrc, int count )
{
    // Aqui, quizas sea mejor permitir la cadena vacia
    // if (pcDest && mNoVacia(pcSrc) && sizeDest>0)
    if (pcDest && pcSrc && sizeDest>0)
    {
        int lonSrc;
        int lonFinal;

		lonSrc = sString::strlen(pcSrc);
        lonSrc = (count>lonSrc)? lonSrc: count;
        lonFinal = (lonSrc>sizeDest-1)? sizeDest-1: lonSrc;

        memcpy_s(pcDest, sizeDest , pcSrc, lonFinal);
        // Nos aseguramos que pone el nulo final
        pcDest[lonFinal] = 0;
    }

    return 0;
}


cstatic int sString::strncat ( char * pcDest, int sizeDest, const char * pcSrc, int count )
{
    // Es un strcat, no deberia de concatenar nada, mejor que se quede con la
    // pregunta, ya que nada debe de hacer por cadena vacia.
    if (pcDest && mNoVacia(pcSrc) && sizeDest>0)
    {
        int lonDes;
        int lonSrc;
        int lonFinal;

		lonDes = sString::strlen(pcDest);
		lonSrc = sString::strlen(pcSrc);
        lonSrc = (count>lonSrc)? lonSrc: count;

        lonFinal = sizeDest - lonDes - lonSrc - 1;
        if (lonFinal>0)
        {
            // Debemos comprobar que hace strncat con el resto del buffer destino
            // y debe de poner el nulo final.
			::strncat_s(pcDest, sizeDest, pcSrc, lonFinal);
			// pcDest[lonFinal] = 0;
        }
    }

    return 0;
}


cstatic int sString::strcmp ( const char * pcCad1, const char * pcCad2 )
{
    return ::strcmp(pcCad1, pcCad2);
}


cstatic int sString::stricmp ( const char * pcCad1, const char * pcCad2 )
{
    return ::_stricmp(pcCad1, pcCad2);
}


cstatic int sString::strlen ( const char * pcCad )
{
    if (mNoVacia(pcCad))
    {
        return (int) ::strlen(pcCad);
    }

    // Si fuera cadena nula o vacia devolvera 0.
    return 0;
}


cstatic void sString::replace(char * pcCad, char cSrc, char cDest)
{
    int i, iLon;
    iLon = strlen(pcCad);
    for (i = 0; i < iLon; i++)
    {
        pcCad[i] = (pcCad[i] == cSrc) ? cDest : pcCad[i];
    }
    pcCad[i] = 0;
}



/*========================================================================*\
|* Resto metodos, CON instancia ...
\*========================================================================*/
char sString::car(int pos) const
{
    char car = -1;
    if (m_pcCad && pos > -1 && pos < m_iLon)
    {
        car = m_pcCad[pos];
    }
    return car;
}

//--------------------------------------------------------------------------
// limpieza, liberacion de memoria
// Aqui limpia, si o si.
//--------------------------------------------------------------------------
void sString::limpia (void)
{
    delete[] m_pcCad;
    m_iLon = 0;
    m_pcCad = nullptr;
}


//--------------------------------------------------------------------------
// limpieza, liberacion de memoria, sin errores
// Aqui limpia, solo si hay algo que limpiar.
//--------------------------------------------------------------------------
void sString::reset (void)
{
    if (m_pcCad != nullptr)
    {
        delete[] m_pcCad;
    }
    m_iLon = 0;
    m_pcCad = nullptr;
}


//--------------------------------------------------------------------------
// Vacia, hay memoria reservada para poner el valor 0.
// Debiera ser lo mismo que un string igualarlo a "".
//--------------------------------------------------------------------------
void sString::vacia(void)
{
    delete[] m_pcCad;
    m_pcCad = new char[1];
    *m_pcCad = 0;
    m_iLon = 0;
}


int sString::load(int lon)
{
    int iRes = -1;
    if (lon > -1)
    {
        int lonFinal = lon;
        char * pcmem = new char[lonFinal + 1];
        pcmem[lonFinal] = 0;
        if (pcmem)
        {
            // Debemos borrar lo que habia
            limpia();
            // Y ahora si copiamos
            m_pcCad = pcmem;
            m_iLon = lonFinal;
            iRes = 0;
        }
    }
    return iRes;
}


int sString::load ( const char * pcCad, int lon )
{
    int iRes = -1;
    // Debemos admitir el String con valor vacio
    if (pcCad)
    {
        int lonFinal;

        // Si admitimos la cadena vacia, deberia de prevalecer la longitud 'lon = 0' por parametro
        // para meterle la vacia:
        lonFinal = (lon<0) ? sString::strlen(pcCad) : lon;

        char * pcmem = new char[lonFinal + 1];
        memset(pcmem, 0, lonFinal+1);
        // pcmem[lonFinal] = 0;
        if (pcmem)
        {
            // Debemos borrar lo que habia
            limpia();
            // Y ahora si copiamos
            memcpy(pcmem, pcCad, lonFinal);
            m_pcCad = pcmem;
            m_iLon = lonFinal;
            iRes = 0;
        }
    }
    else
    {
        vacia();
    }

    return iRes;
}


//--------------------------------------------------------------------------
// Hay que probar de concatenar una cadena vacia
// con otra con valores ...
//--------------------------------------------------------------------------
int sString::concat(const char * pcCad, int lon)
{
    int iRes = -1;
    // No tenemos que concatenar cadenas vacias
    // Este se puede admitir ...
    if (mNoVacia(pcCad))
    {
        int lonFinal;
        int lonSource;

        // Si admitimos la cadena vacia, deberia de prevalecer la longitud 'lon = 0' por parametro
        // para meterle la vacia:
        // lonSource = (lon<1) ? sString::strlen(pcCad) : lon;
        lonSource = (lon<0) ? sString::strlen(pcCad) : lon;

        lonFinal = lonSource + m_iLon;

        // char * pcmem = (char *)cMemoria::malloc(lonFinal + 1, "sString::concat");
        char * pcmem = new char[lonFinal + 1];
        pcmem[lonFinal] = 0;
        if (pcmem)
        {
            // En el inicio copiamos lo que habia, si es que habia
            if (m_pcCad && m_iLon>0)
            {
                memcpy(pcmem, m_pcCad, m_iLon);
            }
            // Debemos borrar lo que habia
            limpia();   // m_pcCad = NULL y m_iLon = 0.

            // Y ahora copiamos el resto
            // m_iLon al pasar por limpia() esta a cero
            // Pero sabemos que es la diferencia entre lonFinal u lonSource
            m_iLon = lonFinal - lonSource;
            memcpy(pcmem + m_iLon, pcCad, lonSource);

            // Y asignamos los valores finales
            m_pcCad = pcmem;
            m_iLon = lonFinal;

            iRes = 0;
        }
    }

    return iRes;
}


const char* sString::substr(int numPos)
{
    // const char* pcOrigen = sArgum.cad() + 2;
    int lonFinal = (numPos < m_iLon) ? numPos : m_iLon;
    return cad() + lonFinal;
}


/*!
 * Por lo mas sagrado, se borra a si mismo
 * Los creados en la concatenacion de los operadores + quedan marcados
 * para ser eliminados aqui.
 */
void sString::Del ( void ) const
{
    // por lo mas sagrado, se borra a si mismo
    // Si es un string que viene de la concatenacion por operator +
    // con este elemento lo borramos
    if (m_bMeTienesQueBorrar)
    {
        // Por lo mas sagrado, cuidado con esto ...
        delete this;
    }
    // Ojo!! parece muy buena idea, parece que funciona !!
}



//--------------------------------------------------------------------------
// Funciones de format
//--------------------------------------------------------------------------
/*!
 * Carga de sString con formatos variables
 * @param pcFormat
 * @return
 */
sString & sString::format ( const char * pcFormat, ... )
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
sString & sString::format(TVaList & tList, const char * pcFormat)
{
    if (mNoVacia(pcFormat))
    {
        int lonFinal = sString::longitud(tList.stList, pcFormat);
        if (lonFinal>0)
        {
            char * pcBuffer = new char[lonFinal + 1];
            pcBuffer[lonFinal] = 0;
            if (pcBuffer)
            {
                // Debemos borrar lo que habia
                limpia();
                // Y ahora si copiamos
                lonFinal = vsnprintf_s(pcBuffer, lonFinal + 1, lonFinal, pcFormat, tList.stList);
                m_pcCad = pcBuffer;
                m_iLon = lonFinal;
            }
        }
    }

    return *this;
}


/*!
* Versiones Static de format, en origen estaban en cError ...
*/
void sString::formatLista(char * pcDestino, int iLonDestino, va_list pstList, const char * pcFormat)
{
    if (mNoVacia(pcFormat) && pcDestino)
    {
        int lonFinal;
#if defined(_DEPRECATED_) || defined(_MINGW_)
        lonFinal = vsnprintf(NULL, 0, pcFormat, pstList);
#else
        // Y esto, porque no lo utilizo: sString::longitud ??
        lonFinal = vsnprintf_s(sString::Empty, sizeof(sString::Empty), _TRUNCATE, pcFormat, pstList);
        mInicio(sString::Empty);
#endif
        if (lonFinal>0)
        {
            lonFinal = (iLonDestino - 1>lonFinal) ? lonFinal : iLonDestino - 1;

            vsnprintf_s(pcDestino, lonFinal + 1, lonFinal, pcFormat, pstList);
        }
    }
}


void sString::formatCad(char * pcDestino, int iLonDestino, const char * pcFormat, ...)
{
    if (mNoVacia(pcFormat))
    {
        int lonFinal;
        va_list stList;

        va_start(stList, pcFormat);
#if defined(_DEPRECATED_) || defined(_MINGW_)
        lonFinal = vsnprintf(NULL, 0, pcFormat, stList);
#else
        lonFinal = vsnprintf_s(sString::Empty, sizeof(sString::Empty), _TRUNCATE, pcFormat, stList);
        mInicio(sString::Empty);
#endif
        if (lonFinal>0)
        {
            lonFinal = (iLonDestino - 1>lonFinal) ? lonFinal : iLonDestino - 1;

            vsnprintf_s(pcDestino, lonFinal + 1, lonFinal, pcFormat, stList);
        }
        //------
        va_end(stList);
    }
}





/*!
* Pregunta si el contenido de la cadena es vacia
*/
bool sString::esVacia(void)
{
    return mEsVacia(m_pcCad);
}


/*!
 * Quita los blancos por delante y por detras de la cadena
 */
void sString::trim(char cBlancos)
{
    if (m_pcCad)
    {
        cTool::trim(m_pcCad, cBlancos);
        m_iLon = (int) ::strlen(m_pcCad);
    }
}



//----------------------------------------------------------------------
// comvierte el contenido a minusculas
//----------------------------------------------------------------------
void sString::minusculas(void)
{
    cTool::minusculas(m_pcCad, m_iLon);
}


//----------------------------------------------------------------------
// comvierte el contenido a mayusculas
//----------------------------------------------------------------------
void sString::mayusculas(void)
{
    cTool::mayusculas(m_pcCad, m_iLon);
}



//--------------------------------------------------------------------------
// Si el String es de números (todo digitos), devuelve su valor en enteros
// Devuelve cero siempre que no sea un numero
//--------------------------------------------------------------------------
long  sString::Numero  (void)
{
    if (!esVacia())
    {
        if (cTool::esNumero(m_pcCad))
            return atol(m_pcCad);
    }

    return 0;
}


float  sString::Float  (void)
{
    if (!esVacia())
    {
        if (cTool::esFloat(m_pcCad))
            return (float)atof(m_pcCad);
    }

    return 0;
}


double  sString::Double  (void)
{
    if (!esVacia())
    {
        if (cTool::esFloat(m_pcCad))
            return atof(m_pcCad);
    }

    return 0;
}


bool sString::esNumero(void)
{
    return cTool::esFloat(m_pcCad);
}


bool sString::esFloat(void)
{
    return cTool::esFloat(m_pcCad);
}


bool sString::esDouble(void)
{
    return cTool::esFloat(m_pcCad);
}


//--------------------------------------------------------------------------
// Debemos establecer los casos:
// - Si el this esta vacio, devolvemos lista nula: NULL
// - Si no encuentra separador, devolvemos lo que haya.
// - Si no hay separador final, devuelve hasta final, como si no encontrase
//   separador.
// - Si lo ultimo es un separador, no se añade otro elemento mas a la
//   lista
//--------------------------------------------------------------------------
std::vector<sString>* sString::split(const char ** ppcSep, int iNumSep, bool bAdmiteCadenaVacia)
{
    std::vector<sString> * poLista = NULL;
    if (!esVacia())
    {
        cToken oTok;
        sString oItem;
        char * pcTok;

        // Sabemos que la cadena no va a introducir comentarios, por ahora ...
        // Introducimos cadena y longitud de nuestro sString 

        // Vamos a intentar un bucle, que no repita codigo para el elemento final
        pcTok = NULL;
        do
        {
            pcTok = (!pcTok) ?
                oTok.Str(m_pcCad, m_iLon, ppcSep, iNumSep):     // Primera llamada
                oTok.Str();                                     // Siguientes llamadas

            // La cadena devuelta puede estar vacia, hay que admitirla
            if (
                    (pcTok && !oTok.esFin() && bAdmiteCadenaVacia)                  // Admite cadena vacia
                 || (mNoVacia(pcTok) && !oTok.esFin() && !bAdmiteCadenaVacia)       // No admite cadena vacia
                 || (mNoVacia(pcTok) && oTok.esFin())                               // No admite cadena vacia si es fin de Token
               )
            {
                poLista = (poLista == NULL) ? new std::vector<sString>() : poLista;
                if (poLista)
                {
                    oItem = pcTok;
                    // Antes no hice trim ...
                    oItem.trim();

                    if (!oItem.esVacia())       // Este es por la coma final.
                    {
                        poLista->push_back(oItem);
                    }
                }
            }
        }
        while (!oTok.esFin());
    }
    return poLista;
}


//--------------------------------------------------------------------------
// Metodos de sobrecarga de operador
//--------------------------------------------------------------------------
/*!
 * Sobrecarga de operador de asignacion: sString &
 * @param aCopiar
 * @return
 */
sString & sString::operator= ( const sString & aCopiar )
{
	if (this != &aCopiar)
	{
		load(aCopiar.cad(), aCopiar.lon());

	    // Fin Control pila de operador +
	    aCopiar.Del();
	}
    return *this;
}


/*!
 * Sobrecarga de operador de asignacion: char *
 * @param pcCad
 * @return
 */
sString & sString::operator= ( const char * pcCad )
{
    load(pcCad);

    return *this;
}


sString & sString::operator= (int iNum)
{
    char vcNum[LON_BUFF];
    mInicio(vcNum);
    sprintf_s(vcNum, sizeof(vcNum), "%d", iNum);
    load(vcNum);

    return *this;
}



/*!
 * Sobrecarga de operador de adicion y asignacion: sString &
 * @param aCopiar
 * @return
 */
void sString::operator += ( const sString & aCopiar )
{
	concat(aCopiar.cad(), aCopiar.lon());

    // Fin Control pila de operador +
    aCopiar.Del();
}


/*!
 * Sobrecarga de operador de adicion y asignacion: char *
 * @param pcCad
 * @return
 */
void sString::operator += ( const char * pcCad )
{
	concat(pcCad);
}


void sString::operator += (int iNum)
{
    char vcNum[LON_BUFF];
    mInicio(vcNum);
    sprintf_s(vcNum, sizeof(vcNum), "%d", iNum);
    concat(vcNum);
}


sString & sString::operator+ ( const sString & oString )
{
    sString * pNewString;
    if (m_bMeTienesQueBorrar)
    {
        pNewString = this;
    }
    else
    {
        pNewString = new sString(true);
        pNewString->load(m_pcCad, m_iLon);
    }
    pNewString->concat(oString.cad(), oString.lon());

    // Control pila de operador +
    oString.Del();
    return *pNewString;
}


sString & sString::operator+ (const char * pcCad)
{
    sString * pNewString;
    if (m_bMeTienesQueBorrar)
    {
        pNewString = this;
    }
    else
    {
        pNewString = new sString(true);
        pNewString->load(m_pcCad, m_iLon);
    }
    pNewString->concat(pcCad);

    return *pNewString;
}


sString & sString::operator+ (int iNum)
{
    sString * pNewString;
    if (m_bMeTienesQueBorrar)
    {
        pNewString = this;
    }
    else
    {
        pNewString = new sString(true);
        pNewString->load(m_pcCad, m_iLon);
    }
    char vcNum[LON_BUFF];
    mInicio(vcNum);
    sprintf_s(vcNum, sizeof(vcNum), "%d", iNum);
    pNewString->concat(vcNum);

    return *pNewString;
}


//--------------------------------------------------------------------------
// Operadores exigidos por la cLista
//--------------------------------------------------------------------------
// Ordenamiento por la longitud de las cadenas, no solo alfabetico
//--------------------------------------------------------------------------
bool sString::sortString(const sString & sNew, const sString & sOld)
{
    if (sNew.lon() < sOld.lon())
        return true;

    if (sNew.lon() > sOld.lon())
        return false;

    // Aqui son iguales en longitud
    if (sNew < sOld)
        return true;

    return false;
}


bool sString::sortString(const char * pcNew, const char * pcOld)
{
    if (mNoVacia(pcNew) && mNoVacia(pcOld))
    {
        size_t lonNew = strlen(pcNew);
        size_t lonOld = strlen(pcOld);

        if (lonNew < lonOld)
            return true;

        if (lonNew > lonOld)
            return false;

        // Aqui son iguales en longitud
        if ( strcmp(pcNew, pcOld)<0 )
            return true;
    }

    return false;
}


// Oordenamiento alfabetico
bool sString::operator < (const sString & s) const
{
    // Este nos sirve por orden alfabetico A a Z.
    return sString::strcmp(m_pcCad, s.m_pcCad) < 0;     // El propio es menor que el ajeno
}


bool sString::operator == (const sString & s) const
{
    int lonPropio = m_iLon;
    int lonAjeno = s.m_iLon;

    if (lonPropio != lonAjeno)
    {
        return false;
    }

    if (!m_pcCad)
    {
        return false;
    }

    if (!s.m_pcCad)
    {
        return false;
    }

    return (!sString::strcmp(m_pcCad, s.m_pcCad));
}


bool sString::operator == (const char * cadena) const
{
    int lonPropio = m_iLon;
    int lonAjeno = (cadena != nullptr) ? strlen(cadena) : 0;

    if (lonPropio!=lonAjeno)
    {
        return false;
    }

    if (!m_pcCad)
    {
        return false;
    }

    if (!cadena)
    {
        return false;
    }

    return (!sString::strcmp(m_pcCad, cadena));
}


bool sString::operator == (int iNum) const
{
    int lonPropio = m_iLon;
    char vcNum[LON_BUFF];
    mInicio(vcNum);
    sprintf_s(vcNum, sizeof(vcNum), "%d", iNum);
    int lonAjeno = strlen(vcNum);

    if (lonPropio != lonAjeno)
    {
        return false;
    }

    if (!m_pcCad)
    {
        return false;
    }

    return (!sString::strcmp(m_pcCad, vcNum));
}



bool sString::operator != (const sString & s) const
{
    return !(*this == s);
}


bool sString::operator != (const char * cad) const
{
    return !(*this == cad);
}


bool sString::operator != (int iNum) const
{
    return !(*this == iNum);
}


char* sString::operator* () const
{
    return (char *)cad();
}


//--------------------------------------------------------------------------
// Relacionales
//--------------------------------------------------------------------------
// Por definir,
// El linkado no nos las reconoce
// bool operator== ( class sString const & oString, char const * pcCad )
//--------------------------------------------------------------------------
//bool operator== ( const sString & oString, const char * pcCad )
//{
//    bool bRes = false;
//
//    bRes = !(sString::strcmp(oString.cad(), pcCad));
//
//    return bRes;
//}


//bool operator== ( const char * pcCad, const sString & oString )
//{
//    bool bRes = false;
//
//    bRes = !(sString::strcmp(pcCad, oString.cad()));
//
//    return bRes;
//}


//bool operator!= ( const sString & oString, const char * pcCad )
//{
//    bool bRes = false;
//
//    bRes = !!(sString::strcmp(oString.cad(), pcCad));
//
//    return bRes;
//}


//bool operator!= ( const char * pcCad, const sString & oString )
//{
//    bool bRes = false;
//
//    bRes = !!(sString::strcmp(pcCad, oString.cad()));
//
//    return bRes;
//}


//--------------------------------------------------------------------------
// iToString
//--------------------------------------------------------------------------
iToString::iToString(void)
{
    // por ahora vacio
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Convertimos la cadena:
// - parseamos los caracteres para mostrar los caracteres no imprimibles
//--------------------------------------------------------------------------
const char * iToString::toChar(const sString & sCad, sString & sSalida)
{
    int lon = sCad.lon();
    int lonFinal = lon;

    char * pcSal = NULL;
    if (lonFinal > 0)
    {
        char * pcCad = sCad.cad();
        for (int i = 0; i < lon; i++)
        {
            char car = pcCad[i];
            if (!isprint(car))
            {
                int suma = 0;
                switch (car)
                {
                    case '\n':
                    case '\r':
                        suma = 1;
                        break;

                    default:
                        if (!isprint(car))
                        {
                            suma = ((car < 10) ? 0 : (car < 100) ? 1 : 2) + 1;
                        }
                        break;
                }
                lonFinal += suma;
            }
        }

        int index = 0;
        int lonmas1 = lonFinal + 1;
        pcSal = new char[lonmas1];
        memset(pcSal, 0, lonmas1);
        for (int i = 0; i < lon; i++)
        {
            char car = pcCad[i];
            switch (car)
            {
                case '\n':
                    if (index + 2 < lonmas1)
                    {
                        pcSal[index++] = '\\';
                        pcSal[index++] = 'n';
                    }
                    break;

                case '\r':
                    if (index + 2 < lonmas1)
                    {
                        pcSal[index++] = '\\';
                        pcSal[index++] = 'r';
                    }
                    break;

                default:
                    if (isprint(car))
                    {
                    	if (index+1<lonmas1)
                    	{
                            pcSal[index++] = car;
                    	}
                    }
                    else
                    {
                        int iLon, iCar;
                        char vcNum[8];

                        iCar = car;
                        sprintf_s(vcNum, sizeof(vcNum), "\\%d ", iCar);
                        iLon = (int) strlen(vcNum);
                        if (index + iLon < lonFinal + 1)
                        {
                            for (int j = 0; j < iLon; j++)
                            {
                                pcSal[index] = vcNum[j];
                                index++;
                            }
                        }
                    }
                    break;
            }
        }
        // Asignamos el final
        pcSal[lonFinal] = 0;
        // Copio pcSal a sSalida
        sSalida = pcSal;
        // Tengo que borrar pcSal
        // pcSal = (char *) libera(pcSal);
        delete[] pcSal;
        // Asigno el valor de sSalida a pcSal
        pcSal = sSalida.cad();
    }

    return pcSal;
}


//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
// Fin de sString.cpp
//--------------------------------------------------------------------------
