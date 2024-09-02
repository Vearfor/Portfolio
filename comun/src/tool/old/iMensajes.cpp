//--------------------------------------------------------------------------
// iMensajes.cpp
//--------------------------------------------------------------------------
// Interfaz de mensajes de entre Hilos.
//--------------------------------------------------------------------------


#include "iMensajes.h"
#include "nMensajes.h"
#include "cLog.h"
//#include "sAccion.h"


//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
bool iMensajes::m_bFinPrograma = false;  // Recibido Fin de programa
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
iMensajes::iMensajes(const char* pcQuienSoy)
{
    m_dwThreadId = GetCurrentThreadId();
    memset((void*)&m_tMsg, 0, sizeof(MSG));
    m_bSalir = false;
    m_lCiclo = 0;
    mCopia(m_vcQuienSoy, pcQuienSoy);
}

iMensajes::~iMensajes(void)
{
}



//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
// Bucle de mensajes ...
bool iMensajes::mensajes(void)
{
    // Metemos aqui el control de los ciclos:
    m_lCiclo++;

    // PeekMessage no bloquea la ejecucion del programa
    // - si no hay mensajes continua ...
    // - Remarks de iMensajes::mensajeThread: debe de llamarse de esta manera
    //   para que cree la cola de mensajes.
    // - Si necesitamos evento de hilo listo deberiamos hacer el 'set'
    //   despues del PeekMessage (del primero ?)
    while (PeekMessage(&m_tMsg, NULL, 0, 0, PM_NOREMOVE) == TRUE)
    {
        //==================================================================
        // GetMessage si bloquea la ejecución del programa.
        // - Se queda a la espera mientras no haya mensajes
        if (GetMessage(&m_tMsg, NULL, 0, 0))	// Antiguo bucle de mensajes.
        {										// Sustituye if por while
            TranslateMessage(&m_tMsg);			// y quita el resto.
            MyDispatchMessage(&m_tMsg);
        }
        //==================================================================
        else
        {
            /*============================================================*\
            |* Parece ser que esta salida esta provocada por haber
            |* recibido: 'PostQuitMessage(0);'
            |* Nos valdra para la salida por destruccion de ventana.
            \*============================================================*/
            return true;
            /*============================================================*/
        }
        //==================================================================
    }

    /*====================================================================*\
    |* Aqui tengo el problema:
    |* - He puesto m_bFin a true pero al haber recibido como mensaje
    |* 'eTM_FIN_PROGRAMA' sale del bucle y termina devolviendo false.
    |*
    |*  'return false;'
    |*
    |* Solucion:
    |*  Hay que preguntar si tenemos que devolver true y no false si
    |*  m_bFin es true.
    |* O mas simple todavia:  devolver Siempre m_bFin.
    |* (El otro problema: m_bFin es de cHilo y no de iMensajes)
    |* Utilizamos m_bSalir = true en eTM_FIN_PROGRAMA
    \*====================================================================*/
    return m_bSalir;
    /*====================================================================*/
}


//--------------------------------------------------------------------------
// Nuestro :: DispatchMessage, pero incompleto, esta aqui como ejemplo
// de lo que podemos hacer.
//--------------------------------------------------------------------------
long iMensajes::MyDispatchMessage(const MSG* lpmsg)
{
    long lRes = 0;

    if (lpmsg)
    {
        lRes = (long) ::DispatchMessage(lpmsg);
        // Nos podemos fabricar nuestro dispatch teniendo una lista de las ventanas generadas.
        // Dejando al dispatcher lanzar las ventanas que no tengan opengl.

        // Deberiamos quedarnos con el valor que devuelve ??
        // Si continua o no continua, ... doc de DispatchMessage ???

        // Podemos tener esto:
        // lRes = m_poAccionMensaje->EjecutaAccion(lpmsg);
        // o llamamos al proceso :
        lRes = AppProc(lpmsg);
        // En fin! Funciona!!
    }

    return lRes;
}


//--------------------------------------------------------------------------
// Funcion de gestión de mensajes, para el hilo principal.
//--------------------------------------------------------------------------
long iMensajes::AppProc(const MSG* lpmsg)
{
    long lRes = 0;

    switch ((eCommonMensajes)lpmsg->message)
    {
        //case eCommonMensajes::eTM_EJECUTA_ACCION:
        //    sAccion::ejecutaAccion(lpmsg);
        //    break;

        case eCommonMensajes::eTM_FIN_PROGRAMA:
            if (!m_bFinPrograma)
            {
                m_bFinPrograma = true;
                //cLog::print("\n");
                cLog::print(" ===========================================================================\n");
                cLog::print("  - Empieza el Fin del Programa ...\n");
                cLog::print(" ===========================================================================\n");
                cLog::print("\n");
            }

            cLog::print(" iMensajes::AppProc: Recibido eTM_FIN_PROGRAMA: [%s]\n", (char*)m_vcQuienSoy);
            finPrograma();
            m_bSalir = true;
            break;

        case eCommonMensajes::eTM_ERROR_HILO:
            {
                char vcMensaje[256];
                mCopia(vcMensaje, "Error inesperado ejecutando hilo");
                throw(vcMensaje);
            }
            break;

        default:
            break;
    }

    return lRes;
}


//--------------------------------------------------------------------------
// Metemos los postthreadmessage en una funcion que los controle, para
// poder generar la traza de los mensajes enviados.
//--------------------------------------------------------------------------
int iMensajes::mensajeThread(ulong idThread, uint msg, WPARAM wParam, LPARAM lParam, int iTraza, const char* pcFormato, ...)
{
    // Si hay mensaje que mostrar, que sea vea antes del postThreadMessage
    // Antes del lanzamiento del mensaje
    BOOL bRes = PostThreadMessage(idThread, msg, wParam, lParam);
    if (!bRes)
    {
        cLog::error(" Error: iMensajes::mensajeThread: PostThreadMessage\n");
        return -1;
    }
    return 0;
}


//--------------------------------------------------------------------------
// Si el proceso de fin de programa se ha desatado, debemos saberlo
// para evitar posibles interbloqueos
//--------------------------------------------------------------------------
bool iMensajes::esFinPrograma(void)
{
    return m_bFinPrograma;
}



//--------------------------------------------------------------------------
// Fin de iMensajes.cpp
//--------------------------------------------------------------------------
