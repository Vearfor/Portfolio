/*========================================================================*\
|* cLog.cpp
\*========================================================================*/
//--------------------------------------------------------------------------
// Control del fichero Log de los programas
//--------------------------------------------------------------------------


#include "cLog.h"
#include "cDir.h"
#include "../Devices/cConio.h"
#include "../Util/cError.h"
#include "../Util/cUtil.h"
#include "../Memoria/fMemoria.h"


/*========================================================================*\
|* cLog
\*========================================================================*/
//--------------------------------------------------------------------------
// Singleton
//--------------------------------------------------------------------------
cLog  * cLog::m_poInstancia = NULL;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cLog::cLog()
    : m_sDirectorio("")
{
    setNew(this, "cLog: Valida: Si alguno no es valido, hay 'memory leak'");
}


cLog::~cLog()
{
    m_poInstancia = NULL;
}



//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Instancia de fichero Log
//--------------------------------------------------------------------------
cLog * cLog::Instancia(void)
{
    return cLog::Instancia(NULL);
}


//--------------------------------------------------------------------------
//#ifndef _MINGW_
//cLog * cLog::Instancia(const char * pcFormatoFichero, ...)
//#else
//cLog * cLog::Instancia(const char * pcFormatoFichero)
//#endif
//--------------------------------------------------------------------------
cLog * cLog::Instancia(const char * pcFormatoFichero, ...)
{
    if (m_poInstancia == NULL)
    {
        m_poInstancia = new cLog();
    }

    cString sFichero = "";
    if (mNoVacia(pcFormatoFichero))
    {
        //------------------------------------------------------------------
        //#ifndef _MINGW_
        //        TVaList tList;
        //        va_start(tList.stList, pcFormatoFichero);
        //        sFichero.format(tList, pcFormatoFichero);
        //        va_end(tList.stList);
        //#else
        //        sFichero = pcFormatoFichero;
        //#endif // !_MINGW_
        //------------------------------------------------------------------
        TVaList tList;
        va_start(tList.stList, pcFormatoFichero);
        sFichero.format(tList, pcFormatoFichero);
        va_end(tList.stList);
    }

    // if (mNoVacia(pcFormatoFichero) && m_poInstancia && strcmp(pcFormatoFichero, m_poInstancia->m_vcNombreFichero))

    if (!sFichero.EsVacia() && m_poInstancia && sFichero != m_poInstancia->m_vcNombreFichero)
    {
        int iRes;
        //
        // Si esta abierto:
        //      - se cierra,
        //      - se copia en el nuevo,
        //      - y se borra, (el fichero viejo)
        //
        // Se deja abierto el fichero de log nuevo, con el fichero Nuevo
        //

        if (m_poInstancia->EstaAbierto())
        {
            char vcOldLog[LON_BUFF];
            strcpy_s(vcOldLog, sizeof(vcOldLog), m_poInstancia->m_vcNombreFichero);

            iRes =
                (
                        cLog::closeLog() != 0
                    ||  m_poInstancia->init(*sFichero, eFileMode::eWRITE) != 0
                    ||  m_poInstancia->creaDir() != 0
                    ||  m_poInstancia->abrir() != 0
                    ||  copiarFichero(m_poInstancia->m_pFile, vcOldLog) != 0
                    ||  borrarFichero(vcOldLog) != 0
                );
        }
        // No esta abierto, se abre el fichero de log
        else
        {
            iRes =
                (
                        //m_poInstancia->init(pcFormatoFichero, eWRITE)
                        m_poInstancia->init(*sFichero, eFileMode::eWRITE) != 0
                    ||  m_poInstancia->creaDir() != 0
                    ||  m_poInstancia->abrir() != 0
                );
        }

        if (iRes)
        {
            mDelete(m_poInstancia);
        }
    }

    return m_poInstancia;
}


//--------------------------------------------------------------------------
// Reescribimos el init de cFile para introducir la extracccion del
// directorio/camino del fichero de log
//--------------------------------------------------------------------------
int cLog::init(const char * pcNombreCompletoFichero, eFileMode eMode)
{
    cFile::init(pcNombreCompletoFichero, eMode);
    setDir();
    return 0;
}


//--------------------------------------------------------------------------
// Como la llamaremos fuera, la dejamos en una linea sencilla
//
// Me cuesta entender el codigo, una vez hecho.
// Es mejor intentar meter funciones que resuelven un problema para futuro
//
//   Entiendo mejor:  'closeLog();'
//
//              que:  'cLog::Log()->cerrar();'
//
//--------------------------------------------------------------------------
//
// Final en todos nuestros programas:
//
// Deberian llamarse en este orden:
//
//          memoryLog();        // Genera informe de Memoria en el Log
//          closeLog();         // Cierra el Log
//
// La ultima linea con memoria activa:
//
//          cMemoria::cierraRegistro("cApp");
//
// Tengo que asegurarme la secuencia....
//
//--------------------------------------------------------------------------
int cLog::closeLog(void)
{
    //// Deberian de llamar con:
    // cLog::Log()->cerrar();

    //// Con esta funcion la llamada es mas intuitiva:
    // cLog::closeLog();
    if (m_poInstancia)
    {
        m_poInstancia->cerrar();
    }

    // return (m_poInstancia) ? m_poInstancia->cerrar() : 0;

    return 0;
}


//--------------------------------------------------------------------------
// Para ocultar o mostrar el log en la consola si esta estuviera activa
//--------------------------------------------------------------------------
void cLog::showLog(int iNivel)
{
    if (siTraza(iNivel))
    {
        mDo(m_poInstancia)->cFile::setShow(true);
    }
}

void cLog::hideLog(void)
{
    mDo(m_poInstancia)->cFile::setShow(false);
}


bool cLog::isShow(void)
{
    if (m_poInstancia)
    {
        return m_poInstancia->cFile::isShow();
    }
    return false;
}


void cLog::setShow(bool bShow)
{
    if (m_poInstancia)
    {
        m_poInstancia->cFile::setShow(bShow);
    }
}

//--------------------------------------------------------------------------
// Apertura del log en modo Append
// Para continuar escribiendo sobre un contenido de log que ya existe ...
//--------------------------------------------------------------------------
int cLog::appendLog()
{
    int iRes = -1;
    if (m_poInstancia)
    {
        if (!m_poInstancia->EstaAbierto())
        {
            m_poInstancia->m_pFile = abrirFichero(m_poInstancia->m_vcNombreFichero, eFileMode::eAPPEND);
            if (m_poInstancia->m_pFile)
            {
                iRes = 0;
            }
        }
    }

    return iRes;
}


//--------------------------------------------------------------------------
// Mostramos el informe de memoria por el fichero de Log
//--------------------------------------------------------------------------
void cLog::memoryLog(void)
{
#ifdef _ACTMEM_
    if (m_poInstancia && m_poInstancia->EstaAbierto() && cMemoria::esActiva())
    {
        memory(m_poInstancia);
    }
#endif
}


//--------------------------------------------------------------------------
// Escritura directa en el fichero de log
//--------------------------------------------------------------------------
int cLog::print(const char * pcFormat, ...)
{
    if (mNoVacia(pcFormat) && m_poInstancia && m_poInstancia->EstaAbierto())
    {
        va_list stList;

        va_start(stList, pcFormat);
        //------
        m_poInstancia->escribeLista(0, stList, pcFormat);
        //------
        va_end(stList);
    }
    return 0;
}


// Se diferencia de la anterior en que la salida a Consola, si la tiene,
// sera con colores de error:
int cLog::error(const char * pcFormat, ...)
{
    if (mNoVacia(pcFormat) && m_poInstancia && m_poInstancia->EstaAbierto())
    {
        va_list stList;
        va_start(stList, pcFormat);
        //------
        //------
        if (m_poInstancia->isShow())
        {
            //word actualColor = 0;
            //actualColor = cConio::GetColor();
            //cConio::SetColor(cConio::GetErrorColor());
            //m_poInstancia->escribeLista(0, stList, pcFormat);
            //cConio::SetColor(actualColor);

            //word actualColor = 0;
            //actualColor = cConio::GetColor();

            cConio::SetColor(cConio::GetErrorColor());
            m_poInstancia->escribeLista(0, stList, pcFormat);
            cConio::SetColor(cConio::GetNormalColor());
        }
        else
        {
            m_poInstancia->escribeLista(0, stList, pcFormat);
        }
        //------
        va_end(stList);
    }
    return 0;
}


//--------------------------------------------------------------------------
// Escritura directa en el fichero de log, dependiendo de su nivel de traza
//--------------------------------------------------------------------------
int cLog::traza(int iNivel, const char * pcFormat, ...)
{
    if (mNoVacia(pcFormat) && m_poInstancia && m_poInstancia->EstaAbierto())
    {
        va_list stList;

        va_start(stList, pcFormat);
        //------
        m_poInstancia->escribeLista(iNivel, stList, pcFormat);
        //------
        va_end(stList);
    }
    return 0;
}


//--------------------------------------------------------------------------
// Devuelve true si el nivel de traza es inferior
// al nivel de traza establecido para el fichero de log.
//--------------------------------------------------------------------------
bool cLog::siTraza(int iNivel)
{
    if (m_poInstancia)
    {
        if (iNivel < m_poInstancia->getNivelTraza() + 1)
        {
            return true;
        }
    }
    return false;
}


//--------------------------------------------------------------------------
// Establece el nombre del directorio a partir del nombre completo
// del fichero de log
//--------------------------------------------------------------------------
void cLog::setDir(void)
{
    char vcDirectorio[LON_BUFF];
    nsUtil::uDarCaminoFichero(m_vcNombreFichero, vcDirectorio, sizeof(vcDirectorio));
    m_sDirectorio = vcDirectorio;
    //setNew((void *)m_sDirectorio.Cad(), "cLog:Dir");
}


//--------------------------------------------------------------------------
// Me he encontrado con el inesperado problema de que no esta creando
// el directorio del Log
//--------------------------------------------------------------------------
int cLog::creaDir(void)
{
    int iRes = -1;

    iRes = (cDir::existe(m_sDirectorio)) ? 0 : -1;
    if (iRes)
    {
        iRes = cDir::makeDir(m_sDirectorio);
    }

    return iRes;
}


//--------------------------------------------------------------------------
// Devuelve el directorio
//--------------------------------------------------------------------------
const char * cLog::getDir(void)
{
    const char * pcDir = NULL;

    if (m_poInstancia)
    {
        if (m_poInstancia->m_sDirectorio.EsVacia())
        {
            m_poInstancia->setDir();
        }
        else
        {
            pcDir = m_poInstancia->m_sDirectorio.Cad();
        }
    }

    return pcDir;
}


//--------------------------------------------------------------------------
// Cadena de tiempo en la traza:
//--------------------------------------------------------------------------
void cLog::setShowTime(bool bShow)
{
    if (m_poInstancia)
    {
        m_poInstancia->m_bShowTime = bShow;
    }
}


void cLog::showTime(void)
{
    if (m_poInstancia)
    {
        m_poInstancia->m_bShowTime = true;
    }
}


void cLog::hideTime(void)
{
    if (m_poInstancia)
    {
        m_poInstancia->m_bShowTime = false;
    }
}


bool cLog::isShowTime(void)
{
    if (m_poInstancia)
    {
        return m_poInstancia->m_bShowTime;
    }
    return false;
}


//--------------------------------------------------------------------------
// Generacion de informe de memoria a un archivo ya determinado.
//--------------------------------------------------------------------------
void cLog::memory(const char * pcFicLog, bool bShow)
{
#ifdef _ACTMEM_
    cFile oFile;
    char vcFiclog[LON_BUFF];

    cMemoria::nombreFichero(pcFicLog, vcFiclog, sizeof(vcFiclog));

    oFile.init(vcFiclog, eFileMode::eWRITE);
    if (!oFile.abrir())
    {
        oFile.setShow(bShow);
        memory(&oFile);
        oFile.setShow(false);
        oFile.cerrar();
    }
#endif
}


//--------------------------------------------------------------------------
// Generamos metodo para encapsular el uso del fichero de log
// como salida del informe de memoria.
//--------------------------------------------------------------------------
void cLog::memory(cFile * poFile)
{
#ifdef _ACTMEM_
    cMemoria::TNodo * ptPrimero = cMemoria::getPrimerNodo();

    // Quito la ultima refrencia al directorio del Log:
    m_poInstancia->m_sDirectorio.limpia();

    // Asi no aparecera en el informe del log que vamos a mostrar:
    //
    //<<    10 [    2224] [       7] [newarray::cString::load]  Val:"Files/">>
    //

    poFile->print("\n");
    poFile->print(" ===============================================================\n");
    if (!ptPrimero)
    {
        poFile->print("  Lista de nodos en memoria Vacia\n");
    }
    else
    {
        // cMemoria::TNodo * ptNext, * ptAnterior;
        cMemoria::TNodo * ptNext;
        long    lCont, lKbytes, lMbytes, lRestoKbytes;
        long    lMaxMemoria;
        long    lMemoriaActual;
        long    lMemNodos;
        long    lMemTotal;
        bool    bNodoValido;

        poFile->print("  Lista de nodos actuales en memoria (no liberados)\n");
        poFile->print(" ===============================================================\n");
        poFile->print(" %5s [%8s] [%8s] [%s]\n", "Cont", "Ident", "Bytes", "Descripcion del Chivato");
        poFile->print(" ---------------------------------------------------------------\n");

        for (
            bNodoValido = true,
            //ptAnterior = NULL,
            ptNext = ptPrimero,
            lCont = lMemoriaActual = 0;
            ptNext && bNodoValido;)
        {
            lCont++;
            lMemoriaActual += ptNext->m_ilon;
            poFile->print(" %5ld [%8ld] [%8d] [%s]",
                lCont,
                ptNext->m_lIdent,
                ptNext->m_ilon,
                ptNext->m_vcChivato
            );

            if (!strcmp(ptNext->m_vcChivato, "newarray::cString::load::cad"))
            {
                poFile->print("  Val:\"%s\"", (char *)ptNext->m_pmem);
            }
            poFile->print("\n");

            // ptAnterior = ptNext;        // Lo dejo por motivos de depuracion, por si hay error.
            ptNext = ptNext->m_ptNext;
            bNodoValido = cMemoria::nodoValido(ptNext);
        }

        if (!bNodoValido)
        {
//#define _BREAK_EN_EL_LOG_
#if defined(_DEBUG) && defined(_BREAK_EN_EL_LOG_)
            char    vcTitulo[LON_BUFF];
            sprintf_s(vcTitulo, sizeof(vcTitulo), "'': Gestor de Memoria");     //, m_vcPrograma);

            (
                (cMemoria::mensajeWindow(
                    NULL,
                    MB_OKCANCEL,
                    vcTitulo,
                    "[Break provocado por el Usuario] Memoria en el gestor no valida\n\n"\
                    "<Da a 'Aceptar' para ver detalles.>\n"\
                    "<Da a 'Cancelar' para continuar.>\n\n"\
                    "<Si aceptas, el programa podra continuar si damos a 'Continue'>"
                ) != 1)
                || (cMemoria::ctrlDebugBreak(), 0)
            );
#endif
            // Sea como sea hay que marcar la memoria corrupta y avisar de ello en el log.
            poFile->print(" ---------------------------------------------------------------\n");
            poFile->print("  Hemos terminado por Nodo siguiente no valido y no nulo.\n");
            poFile->print("  Puede de existir un problema.\n");
        }

        if (lCont)
        {
            if (lMemoriaActual>1)
            {
                poFile->print(" ---------------------------------------------------------------\n");
                poFile->print("                [%10ld] <-- Total\n", lMemoriaActual);
                poFile->print("   No liberados [%10ld]\n", lCont);
            }
            poFile->print(" ---------------------------------------------------------------\n");
            poFile->print(" Mem.o liberada :%10ld    (Registro)\n", cMemoria::getBytesAsignados());       //  m_lMemoria
            poFile->print(" Bloques/Nodos  :%10ld \n", cMemoria::getNumBloques());                        // m_lNumBloques
            poFile->print(" ---------------------------------------------------------------\n");

            lKbytes = lMemoriaActual / 1024;
            lMbytes = lMemoriaActual / (1024 * 1024);
            lRestoKbytes = lKbytes - (lMbytes * 1024);
            poFile->print(" Memoria no liberada        :  %10ld bytes ::  %10ld Kb%s ::  %7ld Mega%s y %4ld Kb%s\n",
                lMemoriaActual, lKbytes, ((lKbytes == 1) ? " " : "s"), lMbytes, ((lMbytes == 1) ? " " : "s"), lRestoKbytes, ((lRestoKbytes == 1) ? " " : "s"));

            lMemNodos = lCont * sizeof(cMemoria::TNodo);
            lKbytes = lMemNodos / 1024;
            lMbytes = lMemNodos / (1024 * 1024);
            lRestoKbytes = lKbytes - (lMbytes * 1024);
            poFile->print(" Memoria en %6ld Bloques  :  %10ld bytes ::  %10ld Kb%s ::  %7ld Mega%s y %4ld Kb%s\n",
                lCont,
                lMemNodos, lKbytes, ((lKbytes == 1) ? " " : "s"), lMbytes, ((lMbytes == 1) ? " " : "s"), lRestoKbytes, ((lRestoKbytes == 1) ? " " : "s"));

            lMemTotal = lMemoriaActual + lMemNodos;
            lKbytes = lMemTotal / 1024;
            lMbytes = lMemTotal / (1024 * 1024);
            lRestoKbytes = lKbytes - (lMbytes * 1024);
            poFile->print(" Memoria Total no liberada  : %11ld bytes ::  %10ld Kb%s ::  %7ld Mega%s y %4ld Kb%s\n",
                lMemTotal, lKbytes, ((lKbytes == 1) ? " " : "s"), lMbytes, ((lMbytes == 1) ? " " : "s"), lRestoKbytes, ((lRestoKbytes == 1) ? " " : "s"));

            poFile->print(" ---------------------------------------------------------------\n\n");
        }
        poFile->print(" ---------------------------------------------------------------\n");

        //------------------------------------------------------------------
        // Maxima memoria asignada
        //------------------------------------------------------------------
        lMaxMemoria = cMemoria::getMaxBytesAsignados();
        lKbytes = lMaxMemoria / 1024;              //  m_lMaxMemoria
        lMbytes = lMaxMemoria / (1024 * 1024);
        lRestoKbytes = lKbytes - (lMbytes * 1024);

        poFile->print(" Maxima Memoria asignada    : %11ld bytes ::  %10ld Kb%s ::  %7ld Mega%s y %4ld Kb%s\n",
            lMaxMemoria, lKbytes, ((lKbytes == 1) ? " " : "s"), lMbytes, ((lMbytes == 1) ? " " : "s"), lRestoKbytes, ((lRestoKbytes == 1) ? " " : "s"));
        //------------------------------------------------------------------

        //------------------------------------------------------------------
        // Maxima memoria asignada a los nodos de memoria
        //------------------------------------------------------------------
        lMemNodos = cMemoria::getMaxNumBloques() * sizeof(cMemoria::TNodo);
        lKbytes = lMemNodos / 1024;
        lMbytes = lMemNodos / (1024 * 1024);
        lRestoKbytes = lKbytes - (lMbytes * 1024);

        poFile->print(" Memoria en %6ld Bloques  : %11ld bytes ::  %10ld Kb%s ::  %7ld Mega%s y %4ld Kb%s\n",
            cMemoria::getMaxNumBloques(),
            lMemNodos, lKbytes, ((lKbytes == 1) ? " " : "s"), lMbytes, ((lMbytes == 1) ? " " : "s"), lRestoKbytes, ((lRestoKbytes == 1) ? " " : "s"));
        //------------------------------------------------------------------

        //------------------------------------------------------------------
        // Memoria Total: suma de memoria + suma de los nodos
        //------------------------------------------------------------------
        lMemTotal = lMaxMemoria + lMemNodos;
        lKbytes = lMemTotal / 1024;
        lMbytes = lMemTotal / (1024 * 1024);
        lRestoKbytes = lKbytes - (lMbytes * 1024);

        poFile->print(" Memoria Total              : %11ld bytes ::  %10ld Kb%s ::  %7ld Mega%s y %4ld Kb%s\n",
            lMemTotal, lKbytes, ((lKbytes == 1) ? " " : "s"), lMbytes, ((lMbytes == 1) ? " " : "s"), lRestoKbytes, ((lRestoKbytes == 1) ? " " : "s"));
        //------------------------------------------------------------------
    }
    poFile->print(" ===============================================================\n");
#endif
}



/*========================================================================*\
|* cCiclo
\*========================================================================*/
//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
//bool cCiclo::m_bActivo = false;
//cListaP<cFile> * cCiclo::m_poListaLogs = NULL;
//cCSection * cCiclo::m_poSeccion = NULL;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
//cCiclo::cCiclo()
//{
//    setNew(this, "cCiclo");
//    //setDir();
//    m_poSeccion = new cCSection("Seccion del Ciclo");
//    m_poListaLogs = new cListaP<cFile>("Lista de ficheros Logs de Ciclo");
//}
//
//
//cCiclo::~cCiclo()
//{
//    mDelete(m_poListaLogs);
//    mDelete(m_poSeccion);
//    m_poInstancia = NULL;
//}



//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
//mSingleton(cCiclo);
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Busca el fichero que pertenece al ThreadId dado
//--------------------------------------------------------------------------
//cFile * cCiclo::buscaFileThread(dword dwThreadId)
//{
//    cFile * poFile = NULL;
//    if (m_poInstancia && m_poListaLogs && m_poListaLogs->Count()>0)
//    {
//        for (auto itFile = m_poListaLogs->begin(); !itFile.esFin() && !poFile; itFile++)
//        {
//            auto poFichero = **itFile;
//            if (poFichero)
//            {
//                poFile = (poFichero->getDwThread()==dwThreadId) ? poFichero : NULL;
//            }
//        }
//    }
//    return poFile;
//}


//--------------------------------------------------------------------------
// Inicio del fichero de ciclo
//--------------------------------------------------------------------------
//int cCiclo::inicio(const char * pcNombre)
//{
//    if (!m_bActivo && mNoVacia(pcNombre) && Instancia())
//    {
//        m_poSeccion->entrar();
//        cFile * poFile = new cFile();
//        if (poFile)
//        {
//            const char * pcDir = cLog::getDir();
//            cString ficheroCompleto;
//            poFile->setDwThread(GetCurrentThreadId());
//
//            // ficheroCompleto.format("%s/%s.log", m_sDirectorio.Cad(), pcNombre);
//            ficheroCompleto.format("%s/%s.log", pcDir, pcNombre);
//
//            //poFile->init(ficheroCompleto.Cad(), eFileMode::eAPPEND);
//            poFile->init(ficheroCompleto.Cad(), eFileMode::eWRITE);
//            if (poFile->abrir())
//            {
//                // Error en la apertura:
//                cError oError = cError("Error: cCiclo::inicio: No se ha podido abrir el fichero: [%s]", ficheroCompleto.Cad());
//            }
//            else
//            {
//                m_poListaLogs->Insertar(poFile);
//                // Pausa quitada, no vaya a ser que alguien lo haya dejado activo
//                poFile->setPausa(false);
//                // El sistema de logs por ciclo de hilo queda activo
//                m_bActivo = true;
//            }
//        }
//        m_poSeccion->salir();
//    }
//
//    return 0;
//}


//--------------------------------------------------------------------------
// Fin del fichero de ciclo
//--------------------------------------------------------------------------
//int cCiclo::fin(void)
//{
//    if (m_bActivo && m_poInstancia)
//    {
//        m_poSeccion->entrar();
//        cFile * poFile = m_poInstancia->buscaFileThread(GetCurrentThreadId());
//        if (poFile)
//        {
//            poFile->cerrar();
//            // Debemos quitarlo de la lista
//            m_poListaLogs->Remove(poFile);
//            // El sistema de logs por ciclo de hilo no esta activo
//            m_bActivo = false;
//        }
//        m_poSeccion->salir();
//    }
//    return 0;
//}


//--------------------------------------------------------------------------
// Escritura directa en el fichero de log de ciclo
//--------------------------------------------------------------------------
//int cCiclo::print(const char * pcFormat, ...)
//{
//    if (m_bActivo && mNoVacia(pcFormat) && m_poInstancia)
//    {
//        m_poSeccion->entrar();
//        cFile * poFile = m_poInstancia->buscaFileThread(GetCurrentThreadId());
//        if (poFile && !poFile->esPausa() && poFile->EstaAbierto())
//        {
//            va_list stList;
//
//            va_start(stList, pcFormat);
//            //------
//            poFile->escribeLista(0, stList, pcFormat);
//            //------
//            va_end(stList);
//        }
//        m_poSeccion->salir();
//    }
//    return 0;
//}


//--------------------------------------------------------------------------
// Si la pausa esta activa los print no se lanzan
// Con esto evitamos en los bucles prints de ciclo no necesarios.
//--------------------------------------------------------------------------
//void cCiclo::pausa(bool bActivaPausa)
//{
//    if (m_bActivo)
//    {
//        cFile * poFile = m_poInstancia->buscaFileThread(GetCurrentThreadId());
//        if (poFile)
//        {
//            poFile->setPausa(bActivaPausa);
//        }
//    }
//}


//--------------------------------------------------------------------------
// Comprueba si esta activo el proceso del cCiclo
//--------------------------------------------------------------------------
//bool cCiclo::esActivo(void)
//{
//    return m_bActivo;
//}



/*========================================================================*\
|* Fin de cLog.cpp
\*========================================================================*/
