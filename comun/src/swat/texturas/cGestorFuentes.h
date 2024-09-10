//--------------------------------------------------------------------------
// cGestorFuentes.h
//--------------------------------------------------------------------------

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "cFuente.h"
#include <map>
#include <string>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
struct sWindow;
struct sString;
//--------------------------------------------------------------------------
class _mExport cGestorFuentes
{
	std::map<int, cFuente*> m_mapFuentes;

public:

	cGestorFuentes();
	virtual ~cGestorFuentes();

	int cargaFuente(
		int iIdent,					// Necesario para identificarlo en la lista
									// del Gestor junto con el nombre.
		const char* pcNombre,
		int iAncho,
		int iAlto,
		int iGrupo,
		eTipoFuentes tipoFuente,
		eOldTipoTextura tipoTextura,
		int peso,
		bool bItalic,
		bool bUnderLine,
		bool bStrikeOut,
		bool sombra,
		bool bOutLine = false,
		const char* pcDirFuentes = nullptr,
		int iCharSet = ANSI_CHARSET,
		bool full = false,
		float fEsc = 1.0f,
		float fOffset = 0.0f,
		int iIncSombra = 1);

	int insertar(cFuente* pFuente);
	cFuente* getFuente(int iFuente);
	// cFuente* getFuente(const std::string& sFuente);
	int genera(sWindow *);
	void deleteFuentes();

	//----------------------------------------------------------------------
	static char* BuscaCharSet ( int iCharSet );
	static bool	BuscaTipoFuente ( int iTipoFuente, std::string & oFound );
	//----------------------------------------------------------------------

	mSingletonHeader(cGestorFuentes);

	/*--------------------------------------------------------------------*\
	|* 	Enumeracion Fuentes Disponibles en el Sistema
	|*   - comprobaciones del sistema
	\*--------------------------------------------------------------------*/
	static int iFuentes;
	/*--------------------------------------------------------------------*/
	static int iInicio;
	static const int KNUM_FUENTES_TEST = 8;
	/*--------------------------------------------------------------------*/
	static void arriba();
	static void abajo();
	static void PruebaFuentes(sWindow* poWindow, int iFuenteTest);
	/*--------------------------------------------------------------------*/
	static void	CargaTest (sWindow* poWindow, cLog::eTraza eNivelTraza, const char* pcFontName = nullptr);
	static int EnumeraFuentes ( sWindow* poWindow, cLog::eTraza eNivelTraza, const char* pcFaceName, int iCharset);
	/*--------------------------------------------------------------------*/
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Fin de cGestorFuentes.h
//--------------------------------------------------------------------------
