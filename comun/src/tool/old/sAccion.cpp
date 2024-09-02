//--------------------------------------------------------------------------
// sAccion.cpp
//--------------------------------------------------------------------------

#include "sAccion.h"
#include "consola/cConsola.h"

/*========================================================================*\
|* Ejecuta eTM_EJECUTA_ACCION
\*========================================================================*/
int sAccion::ejecutaAccion(const MSG* lpmsg)
{
    int iRes = 0;
    sAccion* ptAccion = (sAccion*)lpmsg->lParam;
    if (ptAccion)
    {
        //Funcion <-- ptAccion->pFuncion
        //Param   <-- ptAccion->oParam
        // No se esta utilizando 
        // lpmsg->wParam !!!

        void* pParam = (lpmsg->wParam) ? (void*)lpmsg->wParam : ptAccion->oParam.getParam();

        // Las que tienen funcion suelen ser las de la lista de Acciones Disponibles
        if (ptAccion->pFuncion)
        {
            // No hay que borrar ptAccion, a no ser que vengan de "enviaComando"
            // ... estas deben de tener una marca para poder borrarlas ...
            iRes = (ptAccion->pFuncion)(pParam);
        }
        else
        {
            // Si no hay funcion en ptAccion->pFuncion == NULL
            // y no estamos utilizando lpmsg->wParam
            //ptAccion->pFuncion = NULL;
            //ptAccion->vcNombre = ? ? ;
            //ptAccion->eHilo = Hiloque se habra utilizado;
            //ptAccion->iIdent = identificador - 1;
            //ptAccion->oParam = habremos metido los parametros;
            // y no estamos utilizando lpmsg->wParam
            // Utilizemoslo para poner el comando
            // o utilicemos ptAccion->vcNombre de 256 de largo.

            // Construyamos la ejecucion con spawn
            // ejecutaAccion(oComando, sParametros)
            iRes = ejecutaAccion(ptAccion->vcNombre, pParam);
        }

        if (ptAccion->aBorrarEnDestino)
        {
            delete ptAccion;
        }
    }

    return iRes;
}


//--------------------------------------------------------------------------
// Ejecucion de la accion enviada comando/proceso externo:
//--------------------------------------------------------------------------
int sAccion::ejecutaAccion(const char* pcComando, void* pParam)
{
    intptr_t iRes = -1;

    miNulo(pcComando);

    char* pcParam = (char*)pParam;
    cConsola::escribe("\n\n");
    cConsola::escribe(" Ejecuta comando: %s %s\n", pcComando, (!pcParam) ? "" : pcParam);
    iRes = 0;

    // Todo:
    //cString sLineaArgumemtos;

    //sLineaArgumemtos.format("%s %s", pcComando, (!pcParam) ? "" : pcParam);

    //// Descomposicion de argumentos
    //cLisArgum oLisArgum;

    //oLisArgum.Construye(sLineaArgumemtos.Cad());

    //// Probamos sin espera _P_NOWAIT, en iRes se devuelve el identificador
    //// de proceso, (creo?) ...
    //iRes = _spawnv(_P_NOWAIT, pcComando, oLisArgum.GetArgv());

    // Ya nos faltaria la comunicacion de los resultados, pero ya
    // sería continuar con la comunicacion entre procesos ...
    cConsola::escribe("\n Res: %d\n", iRes);
    iRes = 0;

    return (int)iRes;
}


//--------------------------------------------------------------------------
// Fin sAccion.cpp
//--------------------------------------------------------------------------
