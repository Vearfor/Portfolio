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

    MSG     m_tMsg;             // guardamos el mensaje en curso.
    bool    m_bSalir;	        // o condicion de salida del bucle anterior.
    long    m_lCiclo;           // Ciclo del gestor de mensajes.

    iMensajes();
    virtual ~iMensajes(void);

    virtual	long AppProc(const MSG* lpmsg);
    virtual	bool mensajes(void);

    long MyDispatchMessage(const MSG* lpmsg);

public:

    long getCiclo(void) { return m_lCiclo; }
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Fin de iMensajes.h
//--------------------------------------------------------------------------
