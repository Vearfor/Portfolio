//--------------------------------------------------------------------------
// iMensajes.h
//--------------------------------------------------------------------------
// Interfaz de mensajes de entre Hilos.
//--------------------------------------------------------------------------


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "nComun.h"
#include "cLog.h"
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class iMensajes
{
    static bool m_bFinPrograma; // Hemos recibido el mensaje de fin de programa

protected:

    MSG     m_tMsg;             // guardamos el mensaje en curso.
    bool    m_bSalir;	        // o condicion de salida del bucle anterior.
    long    m_lCiclo;           // Ciclo del gestor de mensajes.

    iMensajes();
    virtual ~iMensajes(void);

    virtual	long AppProc(const MSG* lpmsg);

    long MyDispatchMessage(const MSG* lpmsg);

public:

    virtual	bool mensajes(void);

    static int mensajeThread(ulong idThread, uint msg, WPARAM wParam, LPARAM lPARAM, cLog::eTraza iTraza = cLog::eTraza::min, const char* pcFormato = NULL, ...);
    static bool esFinPrograma();

    long getCiclo(void) { return m_lCiclo; }
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Fin de iMensajes.h
//--------------------------------------------------------------------------
