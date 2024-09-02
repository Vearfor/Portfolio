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
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class iMensajes
{
    static bool m_bFinPrograma; // Hemos recibido el mensaje de fin de programa

protected:

    dword   m_dwThreadId;
    MSG     m_tMsg;             // guardamos el mensaje en curso.
    bool    m_bSalir;	        // o condicion de salida del bucle anterior.
    long    m_lCiclo;           // Ciclo del gestor de mensajes.
    char    m_vcQuienSoy[64];

    iMensajes(const char* pcQuienSoy);
    virtual ~iMensajes(void);

    virtual	long AppProc(const MSG* lpmsg);
    virtual	bool mensajes(void);

    virtual bool finPrograma(void) = 0;

    long MyDispatchMessage(const MSG* lpmsg);

public:

    static int mensajeThread(ulong idThread, uint msg, WPARAM wParam, LPARAM lPARAM, int iTraza = 0, const char* pcFormato = NULL, ...);
    static bool esFinPrograma(void);

    long getCiclo(void) { return m_lCiclo; }
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Fin de iMensajes.h
//--------------------------------------------------------------------------
