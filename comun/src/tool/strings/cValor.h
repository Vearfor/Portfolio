//--------------------------------------------------------------------------
// cValor.h
//--------------------------------------------------------------------------
// Con este elemento representamos elementos de los mas variados tipos:
// - eBOOL
// - eLONG
// - eDOUBLE
// - eSTRING
// - eLISTA	: Apenas estan trabajados y son punteros a void.
// - eVOID	:
//
//--------------------------------------------------------------------------include 


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "sString.h"
#include <tool/nComun.h>
// #include <string>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Define
//--------------------------------------------------------------------------
// Vamos a utilizar como valor invalido un valor alto y negativo
// que sera muy improbable que se produzca.
//--------------------------------------------------------------------------
enum eValores
{
	eNoValido	= -1234567890			// Valor que nadie utiliza.
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enumerados
//--------------------------------------------------------------------------
enum class eTipoValor
{
	eBOOL	= 0,
	eLONG	,
	eDOUBLE	,
	eSTRING	,
	eLISTA	,			// Todavia no implementado, ya que no he decidido como hacerlo
	eVOID	,			// Puntero a todo tipo de elementos
	eNUM_TIPOS_VALOR
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Tipos
//--------------------------------------------------------------------------
// Warning a quitar:
// - warning C4624: 'UValor': el destructor se definió implícitamente como
//                            eliminado
//--------------------------------------------------------------------------
union UValor
{
	bool bValor;
	long lValor;
	double dValor;
	sString* psValor;
	void* pLista;
	void* pVoid;

};
//--------------------------------------------------------------------------
typedef void * Param;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class: Antiguos miValor unido con UValor
// typedef struct miValor
//--------------------------------------------------------------------------
class _mExport cValor
{
	UValor      m_uValor	;
	eTipoValor  m_eTipo		;
	char        m_vcOrigen	[64];

	sString	m_sNoString{ "" };

    void deleteString(void);
    void cargaString(const char * pcValor);

    void inicioValor(int iValor = 0);
    void inicioValor(double dValor);
    void inicioValor(bool bValor);
    void inicioValor(const sString & sValor);
    void inicioValor(const char * pcValor);

    void copiaInterna(const cValor & oValor);

public:

	//----------------------------------------------------------------------
	// no lo pasaba a una 'class' porque queria utilizar inicializadores
	//----------------------------------------------------------------------
	cValor	( int iValor = 0 );
    cValor  ( double dValor );
    cValor  ( bool bValor );
    cValor  ( const char * pcValor );
    cValor  ( const sString & oString );
	cValor	( const cValor & oOrigen );
	virtual ~cValor	(void);

    //static  cValor  m_oValorNulo;      // Objeto a devolver si los valores deben ser No Validos

    virtual bool    operator == ( const cValor & oValor );
	virtual void	operator =	( const cValor & oValor );
	virtual void	copia		( const cValor & oValor );

    virtual void    reset   ( int iValor = 0 );
    virtual void    reset   ( double dValor );
   
    void	setOrigen			( const char * pcOrigen );
    const char * getOrigen      ( void ) { return (const char *)m_vcOrigen; }
    void	setValor			( const char * pcValor );
	void	setValor			( int iValor );
	void	setValor			( long lValor );
	void	setValor			( bool bValor );
	void	setValor			( double dValor );
	void	setValor			( void * vValor );
	void	incrementaValor		( void );
	void	setTipo				( eTipoValor eTipo ) { m_eTipo=eTipo; }
	eTipoValor	getTipo			( void ) { return m_eTipo; }
	void  *	getParam			( void );
	const char * getString		( void );
	double	getDouble			( void );
	int		getInt				( void );
	long	getLong				( void );
	bool	getBool				( void );
	void  *	getLista			( void );
	void  *	getVoid				( void );
	bool	esValido			( void );
	bool	esNoValido			( void );
    const char * toString       ( sString & sReserva, const char * pcFormat = NULL );

	//template<typename T> void	Set	( T tValor );
	//template<typename T> T		Get ( void );
};
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
// Fin de cValor.h
//--------------------------------------------------------------------------
