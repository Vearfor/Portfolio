//==========================================================================
//  nExport.h
// 
// Para la declaracion de dlls
//==========================================================================


#ifndef _nExport_h_
#define _nExport_h_
//==========================================================================
// Modulo que contiene los valores comunes de exportacion a los ficheros de
// cabecera utilizados en estas DLLs
//#pragma message ("Importa")
//
//  _MI_DLL   en principio, era para indicar que se utiliza la dll.
//  _DLLUTIL  y este, para indicar, que se exporta (la dll)
//                                  que se importa (el exe)
//
// Al final, la dll define estas macros.
//         y el exe no define ninguna (ni import, ni export)
//==========================================================================
#ifdef _MI_DLL
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#ifndef _DLLUTIL
//---------------------------------------
#define _mExport __declspec(dllimport)	// Se importa, externas
//---------------------------------------
#else
//---------------------------------------
#define _mExport __declspec(dllexport)	// Se exporta, definida _DLLUTIL
//---------------------------------------
#endif  // _DLLUTIL
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
#else
//---------------------------------------*
#define _mExport
//---------------------------------------*
#endif	// _MI_DLL
//==========================================================================
#endif  // _nExport_h_


//==========================================================================
//  Fin de nExport.h
//==========================================================================
