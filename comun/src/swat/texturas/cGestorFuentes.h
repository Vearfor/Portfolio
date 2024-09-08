//--------------------------------------------------------------------------
// nLisFuentes.h
//--------------------------------------------------------------------------


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "cFuente.h"
#include "../Listas/tLista.h"
#include "../Listas/iLista.h"
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class	cFileXml;
class   cInterfaz;
class   cGLWindow;
//--------------------------------------------------------------------------
class _mExport cGestorFuentes
	: public cListaP<cFuente>
	, public iLista
{
    char	m_vcDirFuentes[LON_BUFF];
    int		m_iNumFuentes;
    bool	m_bCargadas;
    bool    m_bGeneraFuentes;
    int     m_iTraza;

public:

	cGestorFuentes();
	virtual ~cGestorFuentes();

    _abstract iLista * creaNewLista(int iIdent = -1);

    int			init(cFileXml * p_ptFicXml, char * pcDirFuentes, cInterfaz * poInterfaz);

	//
	// 	Deberia utilizar solo los parametros que vamos a utilizar en CreateFont
	//
	//  CreateFont(
	//		m_ptFuente->iAlto,			    // Altura de la fuente
	//		m_ptFuente->iAncho,				// Ancho medio de los caracteres
	//		0,								// Angulo de escape (?)
	//		0,								// Angulo de orientacion de la linea base ( y el eje X)
	//		m_ptFuente->iPeso,				// Peso de la fuente
	//		FALSE,							// italic atribute option
	//		FALSE,							// underline attribute option
	//		FALSE,							// strikeout attribute option
	//		iCSet,							// conjunto de caracteres
	//		OUT_TT_PRECIS,					// output precision
	//		CLIP_DEFAULT_PRECIS,			// clipping precision
	//		ANTIALIASED_QUALITY,			// output quality
	//		FF_DONTCARE | DEFAULT_PITCH,		// pitch and family
	//		m_ptFuente->vcNombre			// typeface name
	// 	)
	// 
	// 	   Si no es asi habria que ampliar los parametros
	// 	   Por ahora comentare los no utilizados con CreateFont
	//
	int         cargaFuente(
					int iIdent,				// Necesario para identificarlo en la lista del Gestor
					const char* pcNombre,
					int iAncho,
					int iAlto,
					int peso,
					bool bItalic,
					bool bUnderLine,
					bool bStrikeOut,
					bool sombra,
					// int iGrupo,
					// eTipoFuentes eTipoFon,	// Fijo eFN01
					// eTipoTextura eTipoTex,	// eTNoReg
					// float fEsc,				// Esc, generalmente a 1, el Angulo de escape de CreateFont ?
					// float fOffset,
					int iCharSet = ANSI_CHARSET,
					const char* pcDirFuentes = nullptr,
					bool full = false);

    //----------------------------------------------------------------------
    int		    genera(bool bFuerzaCarga = false, const char* pcOrigen = NULL);
    int		    generaCR(bool bFuerzaCarga = false, const char* pcOrigen = NULL, bool bSinContexto = false);
    //----------------------------------------------------------------------
	cFuente	  *	getFuente			( int iFuente );
	//----------------------------------------------------------------------
	int			SetDirFuentes		( char * pcDirFuentes );
    //----------------------------------------------------------------------
    int			ConsFuente			( cGLWindow* poWindow, int iIdTex, char * pcNombre, int iAncho, int iAlto, int iCharSet = -1 );
	//----------------------------------------------------------------------
	cFuente* creaFuente(int iFuente);
	void        activaGeneraFuentes(void) { m_bGeneraFuentes = true; }
	//----------------------------------------------------------------------
	// 	Enumeracion Fuentes Disponibles en el Sistema
	// 	+ Pruebas para ver resultados
	//----------------------------------------------------------------------
	static int iFuentes;
	static int iInicio;
	static void	CargaTest			(cGLWindow* poWindow, int iNivelTraza, const char* pcFontName = nullptr);
	static void	PruebaTest			(cGLWindow* poWindow, int iFuenteTest);
	static void arriba();
	static void abajo();
	static int EnumeraFuentes		( cGLWindow * poWindow, int iNivelTraza, const char * pcFaceName, int iCharset );
	static char* BuscaCharSet		( int iCharSet );
	static bool	BuscaTipoFuente		( int iTipoFuente, cString & oFound );
	//----------------------------------------------------------------------

	mSingletonHeader(cGestorFuentes);
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Fin de nLisFuentes.h
//--------------------------------------------------------------------------
