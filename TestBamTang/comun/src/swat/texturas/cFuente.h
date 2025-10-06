/*------------------------------------------------------------------------*\
|* cFuente.h
\*------------------------------------------------------------------------*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "cTextura.h"
#include <tool/cItem.h>
#include <tool/nComun.h>
#include <tool/cLog.h>
#include <windows.h>
#include <wingdi.h>
#include <GLM/glm.hpp>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//  Defines necesarios para la declaracion de las fuentes.
//--------------------------------------------------------------------------
#define PfCOORD  { 0.0f, 0.0f, 0.0f, 0.0f },
#define LfCOORD  PfCOORD PfCOORD PfCOORD PfCOORD \
                 PfCOORD PfCOORD PfCOORD PfCOORD 
#define XfCOORD  LfCOORD LfCOORD LfCOORD LfCOORD \
                 LfCOORD LfCOORD LfCOORD LfCOORD \
                 LfCOORD LfCOORD LfCOORD LfCOORD \
                 LfCOORD LfCOORD LfCOORD LfCOORD
//--------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Si ponemos -1, -1, en las funciones de 'escribe' se intenta escribir en
// la siguiente linea. Esto lo vamos a poner en una sola macro de 
// posicion, con la posibilidad de cambiar la profuncidad:
//--------------------------------------------------------------------------
#define mNextLine(z)		glm::vec3(-1.0f, -1.0f, z)
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enumerados
//--------------------------------------------------------------------------
enum class ePesoFuentes : int
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
enum class eTipoFuentes : int
{
	eAT01	= 0,
	eFT01	,
	eFT02	,
	eFN01	,				// Fuentes sin texturas.
	NUM_TIPOS_FUENTES
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//  sFuente
//--------------------------------------------------------------------------
struct _mExport sFuente
{
	//----------------------------------------------------------------------
	// [Fuentes: Indice: Tamaño Base: Espacio Base: Grupo: Tipo: Fichero: Ancho: Alto:
	//  < eDef    , 16, -3, 0, FT2, def.tga    , 0, 0 >
	//----------------------------------------------------------------------
	int		idFuente    { -1 };		// Identificador de (no textura) la fuente
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
	eOldTipoTextura eTipoTex;			// Tipo de Textura.
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
		idFuente = -1;			// Identificador de textura
		iGrupo = 0;				// 0 primer grupo, 1 segundo grupo, generalmente a 0.
		mInicio(vcNombre);		// Nombre del fichero/ o nombre de la fuente
		mInicio(vcFichero);		// Nombre del fichero de la textura si la tuvieramos.
		iAncho = 1;				// Ancho en pixeles.
		iAlto = 1;				// Altura en pixeles.
		iAnchoTex = 1;
		iAltoTex = 1;
		iCharSet = ANSI_CHARSET;
		//----------------------------------------------------------------------
		iPeso = (int) ePesoFuentes::ePNO_PESO;
		bItalic = false;
		bUnderLine = false;
		bStrikeOut = false;
		bOutLine = false;
		//----------------------------------------------------------------------
		eTipoFon = eTipoFuentes::eFN01;		// Tipo de Fuente FT01, FT02.
		eTipoTex = eOldTipoTextura::eTNoReg;	// Tipo de Textura.
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

struct stLisMen
{				// stLisMen
	bool				bMarca		;	// Cuando el primer elemento esta marcado, no se meten mas elementos en la lista.
	sFuente		  *	prFuente	;	// Puntero a la fuente
	int					x, y		;	// Coordenadas del texto.
	int					iSet		;	// Conjunto de la fuente: 0 o 1.
	int					iColor		;	// Color de las letras del texto.
	char			  *	pcMensaje	;	// Texto del mensaje
	int					iLon		;	// Longitud en bytes del mensaje
	int					iNum		;	// Numero de mensaje en la lista.
	struct stLisMen	  *	prSig		;	// Puntero al siguiente en la lista encadenada.
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class cFuente
//--------------------------------------------------------------------------
struct sString;
struct sWindow;
//--------------------------------------------------------------------------
class _mExport cFuente
	: public cItem
{
	sFuente m_tFuente;
	cTextura  *	m_poTextura;
	int			m_iSiguienteX;		    // Posicion siguiente, en pixeles, para escribir a partir de ahi, desde la ultima vez que se escribio.
    int         m_iSiguienteTitulo;     // Posicion siguiente, en pixeles, para escribir a partir de ahi, desde la ultima vez que se escribio el titulo de detalleTitulo.
	int			m_iUltimoY;			    // Ultima altura en pixeles.
	bool		m_bActivada;
    float		m_vfAncho	[256];      // Anchos variables por caracter.
    float       m_fAnchoXCaracter;      // Anchos fijos
    float       m_fCharMaxHeight;       // Altura maxima de los carateres (en pixels).
    bool        m_bFull;                // Fuente a redimensionar en Full Desktop
	cLog::eTraza m_iTraza;
    float       m_fX;
    float       m_fY;

public:

	cFuente     ( void );
	~cFuente    ( void );

	//----------------------------------------------------------------------
	int inicia(const sFuente& tFuente);
	//----------------------------------------------------------------------

	//----------------------------------------------------------------------
	int			Carga			 (sWindow * poWindow, int iCharSet = -1);
    int			Construye		 (sWindow * poWindow, int iCharSet = -1);
    int			destruye		 (void);
    int			CambiaFuente	 (int set, float fEsc);
	int			escribe			 (sWindow* poWindow, const glm::ivec3& vPos, float escala, const glm::vec4& vColor , const char* pcFormat, ...);
	int         saltaLinea       (sWindow * poWindow);
    //----------------------------------------------------------------------
    int         detalleTitulo	        ( sWindow * poWindow, glm::ivec3 pos, float escala, glm::vec4 colTitulo, glm::vec4 colDetalle, const char * pcTitulo, const char * pcFormatDetalle, ...);
    int         detalleTituloSiguiente  ( sWindow * poWindow, int iDespX, int iZ, float escala, glm::vec4 colTitulo, glm::vec4 colDetalle, const char * pcTitulo, const char * pcFormatDetalle, ...);
    int         detalleSiguienteLinea   ( sWindow * poWindow, glm::ivec3 pos, float escala, glm::vec4 colDetalle, const char * pcFormatDetalle, ...);
    //----------------------------------------------------------------------
	sFuente &	DarRegFuente	 ( void ) { return m_tFuente; }
	int			DarBase			 ( void ) { return m_tFuente.iBase; }
	//----------------------------------------------------------------------
	int			DarIdFuente		 ( void ) { return m_tFuente.idFuente; }
	char	  *	DarNombre		 ( void ) { return m_tFuente.vcNombre; }
	int			DarSiguienteX	 ( void ) { return m_iSiguienteX; }
    int         DarSiguienteTit  ( void ) { return m_iSiguienteTitulo; }
	int			DarUltimoY		 ( void ) { return m_iUltimoY; }
	bool		estaActivada	 ( void ) { return m_bActivada; }
	void		setActivada		 ( bool bActivo ) { m_bActivada = bActivo; }
	void		SetSombra		 ( bool bSombra, int iIncSombra = 1 ) { m_tFuente.bSombra = bSombra;  m_tFuente.iIncSombra = iIncSombra; }
    bool        DarSombra        ( void ) { return m_tFuente.bSombra; }
    int         DarIncSombra     ( void ) { return m_tFuente.iIncSombra; }
	uint		getAnchoTexto	 ( const char * pcText, sWindow * poWindow );
    uint        getLongitud      ( std::string * psCad, sWindow * poWindow  );
    uint        getLongitud      ( const char * pcCad, sWindow * poWindow  );
	uint		getAltoTexto	 ( sWindow * poWindow );
    uint        getAltura        ( sWindow * poWindow );
    int         getPeso          ( void );
    void        setFull          ( bool bFull ) { m_bFull = bFull; }
    bool        getFull          ( void ) { return m_bFull; }
    void        setX             ( float fX ) { m_fX = fX; }
    void        setY             ( float fY ) { m_fY = fY; }
    float       getX             ( void ) { return m_fX; }
    float       getY             ( void ) { return m_fY; }
    //----------------------------------------------------------------------

private:

	//----------------------------------------------------------------------
	int		escribeFuente	( sWindow * poWindow, glm::ivec3 pos, float escala, glm::vec4 color, char * pcMsg );
	//----------------------------------------------------------------------
	int		escribeTrozo	( sWindow * poWindow, glm::ivec3 pos, float escala, char * pcMsg );
	//----------------------------------------------------------------------
};
//--------------------------------------------------------------------------


/*------------------------------------------------------------------------*\
|* Fin de cFuente.h
\*------------------------------------------------------------------------*/
