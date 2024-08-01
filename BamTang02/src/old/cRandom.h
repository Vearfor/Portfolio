//--------------------------------------------------------------------------
// cRandom.h
//--------------------------------------------------------------------------


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "../Util/nComun.h"
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Constantes, para el metodo congruencial deben de definirse unos
// parametros A y B adecuados.
//--------------------------------------------------------------------------
const long c_ulA = 25173L;
const long c_ulB = 12849L;
const long c_ulN = 65536L;		// Entre 0 y N
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class _mExport cRandom  
{
	long	m_ulSemilla;		// Semilla inicial.
	long	m_ulRand;			// Valor que va cambiando segun se va llamando.
	long	m_ulA;				// Parametro A del metodo congruencial utilizado.
	long	m_ulB;				// Parametro B del metodo congruencial utilizado.
	long	m_ulN;				// Veces que se ha llamado a la funcion, No es el parametro N del metodo.

	double	rand	( void );

public:

	cRandom();
    cRandom(const cRandom & oRandom);
	virtual ~cRandom();

    void    reset       ( void );
	void	inicia		( long p_ulSemilla );
    void    copia       ( const cRandom & oRandom );
	void	parametros	( long p_ulA= c_ulA, long p_ulB= c_ulB);
    void    setSemilla  ( long ulSemilla );
    void    setParmA    ( long ulParmA );
    void    setParmB    ( long ulParmB );
    void    setNum      ( long ulN );
    void    setRand     ( long ulRand );
	double	aleatorio	( double dIniP, double dFinP );
	long	getSemilla	( void ) { return m_ulSemilla; }
	long	getParamA   ( void ) { return m_ulA; }
	long	getParamB   ( void ) { return m_ulB; }
	long	getRand		( void ) { return m_ulRand; }
	long	getN		( void ) { return m_ulN; }
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Fin de cRandom.h
//--------------------------------------------------------------------------
