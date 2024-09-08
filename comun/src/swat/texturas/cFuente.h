//--------------------------------------------------------------------------
//  nFuente.h
//--------------------------------------------------------------------------


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "cTextura.h"
#include <windows.h>
#include <wingdi.h>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//  Defines necesarios para la declaracion de las fuentes.
//--------------------------------------------------------------------------
// #define W_DIR_FUENTES		"Fuentes"
#define PfCOORD  { 0.0f, 0.0f, 0.0f, 0.0f },
#define LfCOORD  PfCOORD PfCOORD PfCOORD PfCOORD \
                 PfCOORD PfCOORD PfCOORD PfCOORD 
#define XfCOORD  LfCOORD LfCOORD LfCOORD LfCOORD \
                 LfCOORD LfCOORD LfCOORD LfCOORD \
                 LfCOORD LfCOORD LfCOORD LfCOORD \
                 LfCOORD LfCOORD LfCOORD LfCOORD
//--------------------------------------------------------------------------
#define mBeginE		if (wBeginE())
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enumerados
//--------------------------------------------------------------------------
enum ePesoFuentes
{
    ePNO_PESO		= FW_DONTCARE	,	// 0
    ePFINO			= FW_THIN      	,	// 100
    ePEXTRA_LIGERO	= FW_EXTRALIGHT	,	// 200
    ePLIGERO		= FW_LIGHT		,	// 300
    ePNORMAL		= FW_NORMAL		,	// 400
    ePMEDIO			= FW_MEDIUM		,	// 500
    ePSEMI_BOLD		= FW_SEMIBOLD	,	// 600
    ePBOLD			= FW_BOLD		,	// 700
    ePEXTRA_BOLD	= FW_EXTRABOLD	,	// 800
    ePPESADO		= FW_HEAVY		,	// 900
};
//--------------------------------------------------------------------------
enum class eTipoFuentes
{
	eAT01	= 0,
	eFT01	,
	eFT02	,
	eFN01	,				// Fuentes sin texturas.
	NUM_TIPOS_FUENTES
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//  Tipos typedef
//--------------------------------------------------------------------------
struct _mExport sFuente
{
	//----------------------------------------------------------------------
	// [Fuentes: Indice: Tamaño Base: Espacio Base: Grupo: Tipo: Fichero: Ancho: Alto:
	//  < eDef    , 16, -3, 0, FT2, def.tga    , 0, 0 >
	//----------------------------------------------------------------------
	int		idTextura	;			// Identificador de textura
	int		iGrupo		;			// 0 primer grupo, 1 segundo grupo, generalmente a 0.
	char	vcNombre	[256];		// Nombre del fichero/ o nombre de la fuente
	char	vcFichero	[256];		// Nombre del fichero de la textura si la tuvieramos.
	int		iAncho		;			// Ancho en pixeles.
	int		iAlto		;			// Altura en pixeles.
    int     iAnchoTex   ;
    int     iAltoTex    ;
	int		iCharSet	;
	//----------------------------------------------------------------------
	int		iPeso		;
	bool	bItalic		;
	bool	bUnderLine	;
	bool	bStrikeOut	;
	bool	bOutLine	;
	//----------------------------------------------------------------------
	//int		iTipoFon;			// Tipo de Fuente FT01, FT02.
	//int		iTipoTex;			// Tipo de Textura.
	//----------------------------------------------------------------------
	eTipoFuentes eTipoFon;			// Tipo de Fuente FT01, FT02.
	eTipoTextura eTipoTex;			// Tipo de Textura.
	//----------------------------------------------------------------------
	int		iTextura	;			// Número OpenGL de la textura.
	float	fEsc		;			// Escala, generalmente a 1
	int		iBase		;			// Número Base de la fuente.
	int		iNItems		;			// Número de elementos.
	float   fCoord		[128][4];	// Coordenadas de cada letra
	//----------------------------------------------------------------------
	bool	bNuevo		;
    //----------------------------------------------------------------------
    float   fOffset     ;           // Offset en Y para corregir la altura de algunas fuentes.
    bool    bSombra     ;           // Si va a tener una sobra (efecto relieve)
    int     iIncSombra  ;           // Inc. de este efecto.
    //----------------------------------------------------------------------

    sFuente()
    {
        reset();
    }

    void reset()
    {
		//----------------------------------------------------------------------
		idTextura = -1;			// Identificador de textura
		iGrupo = 0;				// 0 primer grupo, 1 segundo grupo, generalmente a 0.
		mInicio(vcNombre);		// Nombre del fichero/ o nombre de la fuente
		mInicio(vcFichero);		// Nombre del fichero de la textura si la tuvieramos.
		iAncho = 1;				// Ancho en pixeles.
		iAlto = 1;				// Altura en pixeles.
		iAnchoTex = 1;
		iAltoTex = 1;
		iCharSet = ANSI_CHARSET;
		//----------------------------------------------------------------------
		iPeso = ePNO_PESO;
		bItalic = false;
		bUnderLine = false;
		bStrikeOut = false;
		bOutLine = false;
		//----------------------------------------------------------------------
		eTipoFon = eTipoFuentes::eFN01;		// Tipo de Fuente FT01, FT02.
		eTipoTex = eTipoTextura::eTNoReg;	// Tipo de Textura.
		//----------------------------------------------------------------------
		iTextura = 0;			// Número OpenGL de la textura.
		fEsc = 1.0f;			// Escala, generalmente a 1
		iBase = 0;				// Número Base de la fuente.
		iNItems = 128;			// Número de elementos.
		//----------------------------------------------------------------------
		bNuevo = false;
		//----------------------------------------------------------------------
		fOffset = 0.0f;			// Offset en Y para corregir la altura de algunas fuentes.
		bSombra = false;		// Si va a tener una sobra (efecto relieve)
		iIncSombra = 1;			// Inc. de este efecto.
		//----------------------------------------------------------------------
		for (int j = 0; j < 128; j++)
        {
            for (int k = 0; k < 4; k++)
                fCoord[j][k] = 0.0f;
        }
		//----------------------------------------------------------------------
	}

};

typedef struct stLisMen  {				// stLisMen
	bool				bMarca		;	// Cuando el primer elemento esta marcado, no se meten mas elementos en la lista.
	sFuente		  *	prFuente	;	// Puntero a la fuente
	int					x, y		;	// Coordenadas del texto.
	int					iSet		;	// Conjunto de la fuente: 0 o 1.
	int					iColor		;	// Color de las letras del texto.
	char			  *	pcMensaje	;	// Texto del mensaje
	int					iLon		;	// Longitud en bytes del mensaje
	int					iNum		;	// Numero de mensaje en la lista.
	struct stLisMen	  *	prSig		;	// Puntero al siguiente en la lista encadenada.
} miLisMensajes;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class cFuente
//--------------------------------------------------------------------------
class   cString;
class   cGLWindow;
//--------------------------------------------------------------------------
class _mExport cFuente : public cItem
{
	sFuente* m_ptFuente;
	cTextura  *	m_poTextura;
	int			m_iSiguienteX;		    // Posicion siguiente, en pixeles, para escribir a partir de ahi, desde la ultima vez que se escribio.
    int         m_iSiguienteTitulo;     // Posicion siguiente, en pixeles, para escribir a partir de ahi, desde la ultima vez que se escribio el titulo de detalleTitulo.
	int			m_iUltimoY;			    // Ultima altura en pixeles.
	bool		m_bActivada;
    float		m_vfAncho	[256];      // Anchos variables por caracter.
    float       m_fAnchoXCaracter;      // Anchos fijos
    float       m_fCharMaxHeight;       // Altura maxima de los carateres (en pixels).
    bool        m_bFull;                // Fuente a redimensionar en Full Desktop
    int         m_iTraza;
    int         m_iX;
    int         m_iY;

public:

	cFuente     ( void );
	~cFuente    ( void );

	//----------------------------------------------------------------------
    int			Inicia			 (sFuente * ptFuente);
    int			Carga			 (cGLWindow * poWindow, int iCharSet = -1);
    int			Construye		 (cGLWindow * poWindow, int iCharSet = -1);
    int			Destruye		 (void);
    int			CambiaFuente	 (int set, float fEsc);
    int			escribe			 (cGLWindow * poWindow, int iX, int iY, int iZ, float escala, int iCol, const char * pcFormat, ...);
    int         saltaLinea       (cGLWindow * poWindow);
    //----------------------------------------------------------------------
    int         detalleTitulo	        ( cGLWindow * poWindow, int iX, int iY, int iZ, float escala, int iColTitulo, int iColDetalle, const char * pcTitulo, const char * pcFormatDetalle, ...);
    int         detalleTituloSiguiente  ( cGLWindow * poWindow, int iDespX, int iZ, float escala, int iColTitulo, int iColDetalle, const char * pcTitulo, const char * pcFormatDetalle, ...);
    int         detalleSiguienteLinea   ( cGLWindow * poWindow, int iX, int iY, int iZ, float escala, int iColDetalle, const char * pcFormatDetalle, ...);
    //----------------------------------------------------------------------
	sFuente  *	DarRegFuente	 ( void ) { return m_ptFuente; }
	int			DarBase			 ( void ) { return m_ptFuente->iBase; }
	//----------------------------------------------------------------------
	char	  *	DarNombre		 ( void ) { return m_ptFuente->vcNombre; }
	int			DarSiguienteX	 ( void ) { return m_iSiguienteX; }
    int         DarSiguienteTit  ( void ) { return m_iSiguienteTitulo; }
	int			DarUltimoY		 ( void ) { return m_iUltimoY; }
	bool		estaActivada	 ( void ) { return m_bActivada; }
	void		setActivada		 ( bool bActivo ) { m_bActivada = bActivo; }
	void		SetSombra		 ( bool bSombra, int iIncSombra = 1 ) { m_ptFuente->bSombra = bSombra;  m_ptFuente->iIncSombra = iIncSombra; }
    bool        DarSombra        ( void ) { return m_ptFuente->bSombra; }
    int         DarIncSombra     ( void ) { return m_ptFuente->iIncSombra; }
	uint		getAnchoTexto	 ( const char * pcText, cGLWindow * poWindow );
    uint        getLongitud      ( cString * psCad, cGLWindow * poWindow  );
    uint        getLongitud      ( const char * pcCad, cGLWindow * poWindow  );
	uint		getAltoTexto	 ( cGLWindow * poWindow );
    uint        getAltura        ( cGLWindow * poWindow );
    int         getPeso          ( void );
    void        setFull          ( bool bFull ) { m_bFull = bFull; }
    bool        getFull          ( void ) { return m_bFull; }
    void        setX             ( int iX ) { m_iX = iX; }
    void        setY             ( int iY ) { m_iY = iY; }
    int         getX             ( void ) { return m_iX; }
    int         getY             ( void ) { return m_iY; }
    //----------------------------------------------------------------------

private:

	//----------------------------------------------------------------------
	int		escribeFuente	( cGLWindow * poWindow, int iX, int iY, int iZ, float escala, int iCol, char * pcMsg );
	//----------------------------------------------------------------------
	int		escribeTrozo	( cGLWindow * poWindow, int iX, int iY, int iZ, float escala, char * pcMsg );
	//----------------------------------------------------------------------
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//  Fin de nFuente.h.
//--------------------------------------------------------------------------
