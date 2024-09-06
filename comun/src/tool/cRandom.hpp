//--------------------------------------------------------------------------
// cRandom.hpp
//--------------------------------------------------------------------------


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
//#include "../Util/nComun.h"
//--------------------------------------------------------------------------
#include "nComun.h"
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class _mExport cRandom
{
	long	m_ulSemilla;		// Semilla inicial.
	long	m_ulRand;			// Valor que va cambiando segun se va llamando.
	long	m_ulN;				// Veces que se ha llamado a la funcion, No es el parametro N del metodo.

	double	rand	( void );

public:

	cRandom();
    cRandom(const cRandom & oRandom);
	virtual ~cRandom();

    void    reset       ( void );
	void	inicia		( long p_ulSemilla = -1);
    void    copia       ( const cRandom & oRandom );
    void    setSemilla  ( long ulSemilla = -1);
    void    setNum      ( long ulN );
    void    setRand     ( long ulRand );
	double	aleatorio	( double dIniP, double dFinP );
	long	getSemilla	( void ) { return m_ulSemilla; }
	long	getRand		( void ) { return m_ulRand; }
	long	getN		( void ) { return m_ulN; }
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Fin de cRandom.hpp
//--------------------------------------------------------------------------
