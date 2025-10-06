//==========================================================================
//  nDllMain.h
//==========================================================================


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "cDll.h"
//--------------------------------------------------------------------------


// #if defined(_WINDOWS)	//
//--------------------------------------------------------------------------
#ifdef _MINGW_
// #pragma message "== DLLMAIN =="
#else
#pragma message (" == DLLMAIN ==")
#endif
//--------------------------------------------------------------------------
// A estas alturas: obsoleto!
//--------------------------------------------------------------------------
// Modifique las siguientes definiciones si tiene que seleccionar como
// destino una plataforma antes que las especificadas a continuación.
// Consulte la referencia MSDN para obtener la información más reciente
// sobre los valores correspondientes a las diferentes plataformas.
//--------------------------------------------------------------------------
//#ifndef WINVER				// Permitir el uso de características específicas de Windows XP o posterior.
//#define WINVER 0x0501		// Cambiar al valor apropiado correspondiente a otras versiones de Windows.
//#endif
//
//#ifndef _WIN32_WINNT		// Permitir el uso de características específicas de Windows XP o posterior.                   
//#define _WIN32_WINNT 0x0501	// Cambiar al valor apropiado correspondiente a otras versiones de Windows.
//#endif						
//
//#ifndef _WIN32_WINDOWS		// Permitir el uso de características específicas de Windows 98 o posterior.
//#define _WIN32_WINDOWS 0x0410 // Cambiar a fin de establecer el valor apropiado para Windows Me o posterior.
//#endif
//
//#ifndef _WIN32_IE			// Permitir el uso de las características específicas de IE 6.0 o posterior.
//#define _WIN32_IE 0x0600	// Cambiar para establecer el valor apropiado a otras versiones de IE.
//#endif
//
//#define WIN32_LEAN_AND_MEAN		// Excluir material rara vez utilizado de encabezados de Windows
//--------------------------------------------------------------------------
// Archivos de encabezado de Windows:
//--------------------------------------------------------------------------
// #include <windows.h>
//--------------------------------------------------------------------------


//#ifdef _MANAGED
//#pragma managed(push, off)
//#endif
//--------------------------------------------------------------------------
// DllMain
//--------------------------------------------------------------------------
#define MODULODLL(Clase,sQuienSoy)	\
BOOL APIENTRY DllMain ( HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved )	\
{                                                                               \
    Clase * poDll;																\
    switch (fdwReason)                                                          \
    {                                                                           \
		case DLL_PROCESS_ATTACH:												\
			cDll::PreInicio();                                                  \
            poDll = (Clase *)Clase::Instancia();								\
			mDo(poDll)->Inicio_Dll (hModule);									\
			break;																\
		case DLL_THREAD_ATTACH:													\
			break;																\
		case DLL_THREAD_DETACH: 												\
			break;																\
		case DLL_PROCESS_DETACH:												\
			poDll = (Clase *) Clase::Instancia();								\
			mDo(poDll)->Salida_Dll();											\
			mDelete(poDll);														\
            cDll::PostFin(sQuienSoy);											\
			break;																\
        default:																\
			break;																\
    }                                                                           \
    return TRUE;                                                                \
}
//--------------------------------------------------------------------------
//BOOL APIENTRY DllMain ( HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved )
//{
//	ClassDll  *	poDll;
//	switch (fdwReason)
//	{
//		case DLL_PROCESS_ATTACH:
//			poMem = cMemoria::Instancia();
//			poMem->SetActiva(false);
//			poMem->AddNumInstancias();
//			poDll = (ClassDll *) ClassDll::Instancia();
//			mDo(poDll)->Inicio_Dll (hModule);
//			break;
//		case DLL_THREAD_ATTACH:
//			break;
//		case DLL_THREAD_DETACH:
//			break;
//		case DLL_PROCESS_DETACH:
//			poDll = (ClassDll *) ClassDll::Instancia();
//			mDo(poDll)->Salida_Dll();
//			mDelete(poDll);
//			break;
//		default:
//			break;
//	}
//	return TRUE;
//}
//--------------------------------------------------------------------------
//#ifdef _MANAGED
//#pragma managed(pop)
//#endif
//--------------------------------------------------------------------------
// #endif		//	(_WINDOWS)


//==========================================================================
// Fin de nDllMain.h
//==========================================================================
