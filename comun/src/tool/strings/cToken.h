/*==========================================================================*\
|* cToken.h
\*==========================================================================*/


#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include "sString.h"
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
struct sString;
//--------------------------------------------------------------------------
class _mExport cToken
{
	bool		m_bFin;					// Indica si hemos llegado el fin de la cadena.
	bool		m_bHay;					// Indica si aun saliendo ha encontrado token.
	sString	  *	m_poBuffer;				// Donde se guarda la copia.
	int			m_iPuntero;				// Puntero en la cadena.
	char	 **	m_ppcSep;				// Lista de separadores (Patrones).
	int			m_iNumSep;				// Numero de separadores (Patrones).
	sString	  *	m_poToken;				// Cadena hasta token.
    // cListaComentarios * m_pLComent;

    int         m_iIniLectura;

    int		LimpiaLista(void);
    int		CopiaLista( const char ** ppcSep, int iNumSep );
    int		Resultado( int p_iPosAnt );

public:

	cToken();
	virtual ~cToken();

    int		Inicia(const char * pcBuffer, long lLon, const char ** ppcSep, int iNumSep);
    int		ListaSeparadores(const char ** p_ppcSep, int p_iNumSep);

    char  *	Str                 ( const char * pcBuffer, int iLonBuffer, const char ** ppcSep, int iNumSep);
    char  *	Str                 ( const char * pcBuffer, int iLonBuffer, const char * pcPatron);
	char  *	Str					( const char * pcBuffer, const char * pcPatron );
	char  *	Str					( void );
    int		RevToken            ( const char * pcOrigen, int iLonBuffer, const char * pcSep, char * pcDestino);

    char  *	leer                ( char * pcBuffer, int iLonBuffer, const char * pcPatron, const char ** ppcSep, int iNumSep);
	char  *	leer				( void );

    char  * getBuffer           ( void );
    long    getLongitud         ( void );
	char  *	getToken			( void );
	int		getLonToken			( void );
	int		getPuntero			( void ) { return m_iPuntero; }
	bool	hasToken			( void ) { return m_bHay; }
	bool	esFin				( void ) { return m_bFin; }
	bool	esComentario        ( char * pcDonde, int & iLonComentario );
    void	addComentario       ( int iIdent, const char * pcIni, const char * pcFin );
    void	removeComentario    ( int iIdent );

    char  * getFinal            ( void );
    void    setPuntero          ( int iPuntero ) { m_iPuntero = iPuntero; }
    int     getIniLectura       ( void ) { return m_iIniLectura; }
};
//--------------------------------------------------------------------------


/*==========================================================================*\
|* Fin de cToken.h
\*==========================================================================*/
