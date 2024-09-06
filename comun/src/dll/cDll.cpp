//--------------------------------------------------------------------------
// cDll.cpp
//--------------------------------------------------------------------------


#include "cDll.h"


//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
//char    cMemoria::m_vcNomShared	[LON_BUFF/4] = "";
//--------------------------------------------------------------------------
int		cDll::m_iInstanciaDll	= 0;
int		cDll::m_iNumInstancias	= 0;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cDll::cDll(void)
{
	m_hInstancia = NULL;
	m_iInstanciaDll ++;
	m_iIdDll = m_iInstanciaDll;
}

cDll::~cDll(void)
{
}



//--------------------------------------------------------------------------
// Resto de metodos.
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Primera funcion de Inicio de la Dll
//--------------------------------------------------------------------------
int	cDll::Inicio_Dll	( HMODULE hModule )
{
	m_hInstancia = hModule;
	m_iNumInstancias ++;

	return 0;	// Devolvemos 0 si todo ha ido bien.
}


//--------------------------------------------------------------------------
// Salida: Destruccion de instancia de Dll
//--------------------------------------------------------------------------
int	cDll::Salida_Dll	( void )
{
	m_iNumInstancias --;

	return 0;	// Devolvemos 0 si todo ha ido bien.
}


//--------------------------------------------------------------------------
// Funciones de sincronizacion de memoria.
// En modulos distintos los elementos ocupan espacios de memoria diferentes
// asi como sus recursos.
// Con esto intentamos que si la memoria ya nos viene creada la instancia
// del gestor de memoria, intentar utilizar la misma.
// El modulo central será el que tiene que controlar su existencia.
//--------------------------------------------------------------------------
//void cDll::SetMemory	( void * pvGestorMemoria )
//{
//	cMemoria  *	poMem = (cMemoria *) pvGestorMemoria;
//	poMem->SetInstancia(poMem);
//}
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
// Metodos estaticos a utilizar en el DllMain
//--------------------------------------------------------------------------
void cDll::PreInicio(void)
{
#ifdef _ACTMEM_
    cMemoria::setRegistro(false);   // Modificas cDll y cApp para activar o no activar
    cMemoria::setActiva(true);
    cMemoria::abreRegistro();
#endif
}


void cDll::PostFin(const char * pcQuien)
{
#ifdef _ACTMEM_
    cMemoria::cierraRegistro(pcQuien);
    cMemoria::cierraLogError(pcQuien);
#endif
}


//--------------------------------------------------------------------------
// Fin de cDll.cpp
//--------------------------------------------------------------------------
