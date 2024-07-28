/*
 * cFile.cpp
 *
 *  Created on: 07/12/2015
 *      Author: Enrique Rocafull Ortiz
 */

#include "cFile.h"
#include "../Util/cToken.h"
#include "../Util/cError.h"
#include "../Util/cTime.h"
#include "../Consola/cConsola.h"
#include "../Memoria/fMemoria.h"

#include <stdarg.h>
#include <io.h>

#if defined(_MINGW_)
#include "../Deprecated/fDeprecated.h"
#endif



//==========================================================================
// Statics
//==========================================================================
int     cFile::giTraza = 2;
//==========================================================================

//--------------------------------------------------------------------------


//==========================================================================
// Constructor & Destructor
//==========================================================================
cFile::cFile(void)
    : m_bShow(false)
	, m_iNivelTraza(eTRAZA_DEFECTO)
    , m_iX(-1)
    , m_iY(-1)
    , m_poBuffer(NULL)
    , m_lPuntero(0)
    , m_pFile (NULL)
    , m_eMode (eFileMode::eLEER_TODO)
    , m_bShowTime(false)
    , m_uiThreadId(0)
{
    setNew(this, "cFile");
    mInicio(m_vcNombreFichero);
}

cFile::~cFile ( void )
{
    cerrar();
}



//==========================================================================
// Statics
//==========================================================================
bool cFile::existe(const char * pcFile)
{
    bool bExiste = false;

    if (mNoVacia(pcFile))
    {
        //FILE * fp;
        //fopen_s(&fp, pcFile, "r");
        //if (fp)
        //{
        //    bExiste = true;
        //    fclose(fp);
        //}

        // Modos ---
        // 00 - solo existencia
        // 02 - solo escritura
        // 04 - solo lectura
        // 06 - lectura y escritura
        // Mejor _access, menos pesado que hacer open y close.
        if (!_access(pcFile, 0))
        {
            bExiste = true;
        }
    }

    return bExiste;
}


//--------------------------------------------------------------------------
// lectura de todo el fichero en un buffer ...
//--------------------------------------------------------------------------
cString * cFile::leerTodo(FILE * fp)
{
    cString * poTodo = NULL;
    if (fp)
    {
        int iLongitud;
        char * pcBuffer;

        fseek(fp, 0, SEEK_END);
        iLongitud = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        // pcBuffer = (char *)cMemoria::malloc(iLongitud + 1, "cFile::leerTodo::char * pcBuffer");
        pcBuffer = new char[iLongitud + 1];
        setNew(pcBuffer, "cFile::leerTodo::char * pcBuffer");
        if (pcBuffer)
        {
            if (fread(pcBuffer, 1, iLongitud, fp)>0)
            {
                poTodo = new cString();
                if (poTodo)
                {
                    if (poTodo->load(pcBuffer, iLongitud))
                    {
                        mDelete(poTodo);
                    }
                    // pcBuffer = (char *)cMemoria::free(pcBuffer);
                    delete[] pcBuffer;
                }
            }
        }
    }
    return poTodo;
}


/*!
 * Leemos todo el contenido de un fichero en un cString
 * Procuraremos no necesitar que el fichero este abierto
 * Se abre y se cierra en el momento.
 * @return Devuelve el contenido del fichero en un puntero cString
 */
cString * cFile::leerTodo ( const char * pcNombreFichero )
{
    cString * poTodo = NULL;
    FILE * fp;

    fp = abrirFichero(pcNombreFichero, eFileMode::eLEER_TODO);
    if (fp)
    {
        poTodo = cFile::leerTodo(fp);
        cerrarFichero(fp);
    }

    return poTodo;
}


//--------------------------------------------------------------------------
// Nuestro fichero esta abierto para escritura ...
// Abrimos el pasado por parametro y copiamos/concatenamos su contenido
//--------------------------------------------------------------------------
FILE * cFile::abrirFichero(const char * pcNombreFichero, eFileMode eMode)
{
    FILE * pFile = NULL;

    switch (eMode)
    {
        case eFileMode::eLEER_TODO:
            fopen_s(&pFile, pcNombreFichero, "rb");
            break;
        case eFileMode::eWRITE:
            fopen_s(&pFile, pcNombreFichero, "wb");
            break;
        case eFileMode::eAPPEND:
            fopen_s(&pFile, pcNombreFichero, "ab");
            break;
        case eFileMode::eMEMORY:
            // ni idea de como hacerlo ... memoria redimensionable ...
            break;
        case eFileMode::eREAD_SIN:
            // Lectura sin buffer, y no cierra el fichero.
            break;
    }

    if (pFile == NULL)
    {
        cError oError(" No se pudo abrir: %s\n", pcNombreFichero);
    }

    return pFile;
}


void cFile::cerrarFichero(FILE * pFile)
{
    if (pFile)
    {
        fclose(pFile);
    }
}


int	cFile::copiarFichero(FILE * pfDest, char * pcFichero)
{
    if (mNoVacia(pcFichero) && pfDest)
    {
        FILE * pfRead;

        pfRead = cFile::abrirFichero(pcFichero, eFileMode::eLEER_TODO);
        if (pfRead)
        {
            char vcLinea[LON_BUFF];

            mInicio(vcLinea);

            for (
                    fgets(vcLinea, sizeof(vcLinea), pfRead);
                    !feof(pfRead);
                    fgets(vcLinea, sizeof(vcLinea), pfRead)
                )
            {
                fprintf(pfDest, "%s", vcLinea);
            }

            cFile::cerrarFichero(pfRead);
        }
    }

    return 0;
}


int cFile::borrarFichero(char * pcFichero)
{
    int iRes = 0;
    //struct dirent *d;
    //DIR *dir;
    //char buf[256];

    //dir = opendir(m_vcFichero);   // Borrado de los ficheros que cumplan este patron ...
    //while (d = readdir(dir))
    //{
    //    sprintf(buf, "%s/%s", m_vcFichero, d->d_name);
    //    iRes = remove(buf);
    //}
    //------------------------------------------------------------------
    // DeleteFile(m_vcFichero);
    //------------------------------------------------------------------
    if (cFile::existe(pcFichero))
    {
        int iRes = ::remove(pcFichero);
        if (iRes)
        {
            cError oError(" No se pudo borrar el fichero [%s]", pcFichero);
        }
    }
    return iRes;
}



//==========================================================================
// Resto metodos
//==========================================================================
int cFile::init(const char * pcNombreCompletoFichero, eFileMode eMode)
{
    cerrar();
    cString::strcpy(m_vcNombreFichero, sizeof(m_vcNombreFichero), pcNombreCompletoFichero);
    m_eMode = eMode;
    m_pFile = NULL;
    m_poBuffer = NULL;
    m_lPuntero = 0;
    return 0;
}


//--------------------------------------------------------------------------
// Un fichero cFile estara abierto, cuando exista su puntero FILE
// Debe de tener nombre de fichero, que se haya podido abrir
//--------------------------------------------------------------------------
bool cFile::EstaAbierto(void)
{
    return (m_pFile!=NULL) || (m_poBuffer && m_eMode == eFileMode::eLEER_TODO);
}


int cFile::abrir(void)
{
    int iRes = 0;
    if (!m_pFile)   // No ha sido abierto ...
    {
        iRes = -1;
        m_pFile = abrirFichero(m_vcNombreFichero, m_eMode);
        if (m_pFile)
        {
            if (m_eMode == eFileMode::eLEER_TODO)
            {
                m_poBuffer = cFile::leerTodo(m_pFile);
                cerrarFichero(m_pFile);
                m_pFile = NULL;
                m_lPuntero = 0;
            }
            iRes = 0;
        }
    }
    return iRes;
}


int cFile::cerrar(void)
{
    if (EstaAbierto())
    {
        if (m_pFile)
        {
            cerrarFichero(m_pFile);
            m_pFile = NULL;
        }
        // Y si tenemos el buffer lleno se elimina
        mDelete(m_poBuffer);
    }
    return 0;
}


/*!
 * Encapsula la funcion de fin de fichero
 * @return
 */
bool cFile::finFichero ( void )
{
    bool bFin = true;

    if (m_eMode == eFileMode::eLEER_TODO)
    {
        if (m_pFile)
        {
            bFin = (feof(m_pFile)) ? true : false;
        }
        else if (m_poBuffer)
        {
            // Al verres
            bFin = (m_lPuntero < m_poBuffer->Lon())? false: true;
        }
    }

    return bFin;
}


/*!
 * Encapsula la escritura en un fichero abierto para escribir
 * @param pcBuffer
 * @param ilonBuffer
 * @return
 */
int cFile::escribe ( char * pcBuffer, int ilonBuffer )
{
    int iRes = -1;

    if ( m_pFile && (m_eMode == eFileMode::eWRITE || m_eMode == eFileMode::eAPPEND) )
    {
        // EnterCriticalSection(&m_xFicSeccion);

        if (fwrite(pcBuffer, ilonBuffer, 1, m_pFile)>0)
        {
            iRes = 0;
        }

        // LeaveCriticalSection(&m_xFicSeccion);
    }

    return iRes;
}


/*!
 * Encapsula la lectura en un fichero abierto para leer
 * @param pcBuffer
 * @param ilonBuffer
 * @return
 */
int cFile::lee ( char * pcBuffer, int ilonBuffer )
{
    int iRes = -1;

    if ( m_pFile && m_eMode == eFileMode::eLEER_TODO )
    {
        // fread devuelve el numero de elementos completos
        // 1: es el numero de bloques/elementos a leer
        //    si ha leido bien, sera 1, si el elemento esta completo
        //                      sera 0, si el elemento no esta completo, pero termina por no leer el bloque
        //
        // fread(pcBuffer, ilonBuffer, 1, m_pFile)
        //
        // Asi que lo ponemos al reves
        int nElementosLeidos;
        memset(pcBuffer, 0, ilonBuffer);
        nElementosLeidos = (int) fread(pcBuffer, 1, ilonBuffer, m_pFile);
        if (nElementosLeidos>-1)
        {
            iRes = nElementosLeidos;
            m_lPuntero += nElementosLeidos;
        }
    }

    return iRes;
}


cString * cFile::leerTodo(void)
{
    // Abre y cierra el fichero ...
    m_poBuffer = cFile::leerTodo(m_vcNombreFichero);
    // Tenemos el buffer
    m_lPuntero = 0;
    // Lo devuelve y lo conservamos ...
    return m_poBuffer;
}


//--------------------------------------------------------------------------
// Devuelve el cString que tiene el contenido del fichero que
// se suele leer con leerTodo
//--------------------------------------------------------------------------
cString * cFile::getBuffer(void)
{
    return m_poBuffer;
}


//--------------------------------------------------------------------------
// Lectura del caracter siguiente.
//--------------------------------------------------------------------------
char cFile::leerCar(void)
{
    // Devolviamos 0.
    char cCar = EOF;
    if (m_poBuffer && !m_poBuffer->EsVacia() && m_eMode == eFileMode::eLEER_TODO)
    {
        if (!finFichero())
        {
            char * pcFichero = m_poBuffer->Cad();
            m_lPuntero++;
            return *(pcFichero + (m_lPuntero - 1));
        }
    }
    return cCar;
}


//--------------------------------------------------------------------------
// Lectura de la linea
// El valor devuelto es memoria de cToken.
// o utiliza la memoria suministrada en pcDest.
//--------------------------------------------------------------------------
char * cFile::leerLinea(cToken * poTok, char * pcDest, int iSizeDest)
{
    char * pcLinea = NULL;
    if (m_poBuffer && !m_poBuffer->EsVacia() && m_eMode == eFileMode::eLEER_TODO)
    {
        if (!finFichero())
        {
            char * pcRes;
            if (m_lPuntero == 0)    pcRes = poTok->Str(m_poBuffer->Cad(), "\n");
            else				    pcRes = poTok->Str();
            m_lPuntero = poTok->getPuntero();

            pcLinea = pcRes;
            if (pcDest)
            {
                strncpy_s(pcDest, iSizeDest, pcRes, iSizeDest - 1);
                pcDest[iSizeDest - 1] = 0;
                pcLinea = pcDest;
            }


        }
    }
    return pcLinea;
}


//--------------------------------------------------------------------------
// Escritura generica:  a fichero a pantalla/consola, etc ...
//--------------------------------------------------------------------------
int cFile::escribeLista(int iNivelTraza, va_list pstList, const char * pcFormat)
{
    if (m_pFile && (iNivelTraza < m_iNivelTraza + 1))     // iNivel > m_iNivelTraza, ==> no hace nada
    {
        // Hemos preguntado antes por el pFile, si no esta abierto, no mostraria
        // ni por consola
        if (m_bShow)
        {
            if (m_iX > -1 && m_iY > -1)
            {
                cConsola::escribeListaXY(m_iX, m_iY, pstList, pcFormat);
            }
            else
            {
                cConsola::escribeLista(pstList, pcFormat);
            }
        }

        setTimeString();

        vfprintf(m_pFile, pcFormat, pstList);
        fflush(m_pFile);
    }

    return 0;
}


//--------------------------------------------------------------------------
// No debe de ser la escritura en consola con cFile, para eso ya tenemos
// cConsola, no lo mezclemos con cFile
// Debe de ser la escritura con formato del cFile en curso
// y si esta activo m_bShow, se muestra en consola.
//--------------------------------------------------------------------------
int cFile::print(const char * pcFormat, ...)
{
    // EnterCriticalSection(&m_xFicSeccion);

    if (mNoVacia(pcFormat) && EstaAbierto())
    {
        va_list stList;

        va_start(stList, pcFormat);
        //------
        escribeLista(0, stList, pcFormat);
        //------
        va_end(stList);
    }

    // LeaveCriticalSection(&m_xFicSeccion);

    return 0;
}


//--------------------------------------------------------------------------
// Y esta es lo mismo que el anterior pero con nivel de traza
//--------------------------------------------------------------------------
int cFile::traza(int iNivel, const char * pcFormat, ...)
{
    //EnterCriticalSection(&m_xFicSeccion);

    if (mNoVacia(pcFormat) && EstaAbierto())
    {
        va_list stList;

        va_start(stList, pcFormat);
        //------
        escribeLista(iNivel, stList, pcFormat);
        //------
        va_end(stList);
    }

    //LeaveCriticalSection(&m_xFicSeccion);

    return 0;
}


//--------------------------------------------------------------------------
// A veces es necesario que nos den el puntero FILE de nuestro fichero
//--------------------------------------------------------------------------
FILE * cFile::getFile(void)
{
    return m_pFile;
}


//--------------------------------------------------------------------------
// Cadena de tiempo en la traza:
//--------------------------------------------------------------------------
void cFile::setShowTime(bool bShow)
{
    m_bShowTime = bShow;
}


void cFile::showTime(void)
{
    m_bShowTime = true;
}


void cFile::hideTime(void)
{
    m_bShowTime = false;
}


bool cFile::isShowTime(void)
{
    return m_bShowTime;
}


//--------------------------------------------------------------------------
// Construye la cadena de tiempo
//--------------------------------------------------------------------------
void cFile::setTimeString(void)
{
    if (m_bShowTime)
    {
        char vcHora[64];
        cTime::Hora(vcHora, sizeof(vcHora), eHoraFormato::HORA_MILSEC);
        fprintf(m_pFile, "[%s] ", vcHora);
    }
}



//--------------------------------------------------------------------------
// Fin de cFile.cpp
//--------------------------------------------------------------------------
