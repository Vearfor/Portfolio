//--------------------------------------------------------------------------
// sAccion.h
//--------------------------------------------------------------------------

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "nComun.h"
#include "cValor.h"
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Enumerados
//--------------------------------------------------------------------------
enum class eHiloDest : int
{
    eHPrincipal = 0,                //
    eHShell,                        // Hilo de Shell, por ser de Comun.dll
    eHiloDeUsuario,                 // dejar detenidos los hilos que no
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
struct sAccion
{
    int			iIdent;
    char		vcNombre[256];
    piProcP		pFuncion;
    cValor		oParam;
    int         eHilo;          // Hilo al cual se dirige la accion.
    bool        aBorrarEnDestino;

    sAccion(void)
    {
        iIdent = -1;
        mInicio(vcNombre);
        pFuncion = NULL;
        oParam.setValor("");
        eHilo = static_cast<int>(eHiloDest::eHPrincipal);
        aBorrarEnDestino = false;
    }

    static  int ejecutaAccion(const MSG* lpmsg);
    static  int ejecutaAccion(const char* pcComando, void* pParam);
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Fin de sAccion.h
//--------------------------------------------------------------------------
