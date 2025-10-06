//--------------------------------------------------------------------------
// cRandom.cpp
//--------------------------------------------------------------------------


#include "cRandom.hpp"
#include <time.h>
#include <cstdlib>


//--------------------------------------------------------------------------
// Constructor/Destructor
//--------------------------------------------------------------------------
cRandom::cRandom()
	: m_ulSemilla(0)
	, m_ulRand(m_ulSemilla)     // Valor que va cambiando segun se va llamando.
	, m_ulN (0)					// Veces que se ha llamado a la funcion.
{
}

cRandom::cRandom(const cRandom & oRandom)
{
    copia(oRandom);
}

cRandom::~cRandom()
{
}



//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Inicialización con la Semilla ya introducida.
//--------------------------------------------------------------------------
void cRandom::reset ( void )
{
	m_ulRand = m_ulSemilla;		// Valor que va cambiando segun se va llamando.
	m_ulN = 0;					// Veces que se ha llamado a la funcion Rand.
}


//--------------------------------------------------------------------------
// Inicialización con cambio de Semilla si es que se pone diferente.
//--------------------------------------------------------------------------
void cRandom::inicia ( long ulSemillaP )
{
	setSemilla(ulSemillaP);
    reset();
}


//--------------------------------------------------------------------------
// Copia del Motor
// - Lo copia en la situacion en que este. No se reInicia.
//--------------------------------------------------------------------------
void cRandom::copia ( const cRandom & oRandom )
{
	m_ulSemilla = oRandom.m_ulSemilla;    // La Semilla inicial.
	m_ulRand = oRandom.m_ulRand;          // Valor que va cambiando segun se va llamando.
	m_ulN = oRandom.m_ulN;                // Veces que se ha llamado a la funcion Rand.
}


//--------------------------------------------------------------------------
// Parametros del Algoritmo.
//--------------------------------------------------------------------------
void cRandom::setSemilla  ( long lSemilla )
{
	if (lSemilla == -1)
	{
		time_t timer;
		time(&timer);
		lSemilla = static_cast<long>(timer);
	}
    m_ulSemilla = lSemilla;
	m_ulRand = m_ulSemilla;		// Valor que va cambiando segun se va llamando.

	// Si no utilizamos el metodo congruencial, usamos la inicializacion del motor aleatorio de la libreria C
	std::srand(m_ulSemilla);
}


//--------------------------------------------------------------------------
// Establecimiento del numero de veces que se llama.
// - No estoy seguro
//--------------------------------------------------------------------------
void cRandom::setNum ( long ulN )
{
    m_ulN = ulN;
}


//--------------------------------------------------------------------------
// Valor del Rand, seria hacerlo asociado al valor anterior
//--------------------------------------------------------------------------
void cRandom::setRand   ( long ulRand )
{
    m_ulRand = ulRand;
}


//--------------------------------------------------------------------------
// Funcion aleatoria: Metodo congruencial
// Valores A y B definidos arriba.
//--------------------------------------------------------------------------
double cRandom::rand ( void )
{
	double dValor = 0;
	m_ulN++;

    // Pendiente de revision ...
    // creo que lo tengo mal, diria que siempre genera el mismo ...
	// Rando= (25173L * Rando + 12849L) % 65536L;
	// m_ulRand = ((m_ulA * m_ulRand) + m_ulB) % c_ulN;

	// dValor = double(m_ulRand) / double(c_ulN);
	dValor = ::rand();

	return dValor;
}


//--------------------------------------------------------------------------
// La funcion de llamada externa para generar numero aleatorios entre
// dos numeros.
//--------------------------------------------------------------------------
double cRandom::aleatorio ( double p_dIni, double p_dFin )
{
	double dIni, dFin;
	double dRand, dValor;

	if	(p_dIni<p_dFin)
	{
		dIni = p_dIni;
		dFin = p_dFin;
	}
	else
	{
		dIni = p_dFin;
		dFin = p_dIni;
	}

	// Valor= int(Rnd * (M-N+1)) + N
	dRand = rand();

	//// Creo que no deberia de dar valores negativos:
	//dRand = (dRand < 0) ? -dRand : dRand;

    // dValor = (dRand * (dFin - dIni + 1)) + dIni;

    // dValor = (dRand * (dFin - dIni)) + dIni;
	int diff = int(dFin - dIni);
	dValor = (dIni + (int(dRand) % diff));

	return dValor;
}


//--------------------------------------------------------------------------
// Fin de cRandom.cpp
//--------------------------------------------------------------------------
/*
	long	m_ulSemilla;		// Semilla inicial
	long	m_ulRand;			// Valor que va cambiando segun se va llamando.
	long	m_ulN;				// Veces que se ha llamado a la funcion.
	long	m_ulA;				// Valor A del metodo congruencial utilizado

	long	Rnd	( void );

public:
	cRandom();
	virtual ~cRandom();

	void	Inicia		( long p_ulSemilla );
	void	Parametros	( long p_ulA, p_ulB );
	double	Aleatorio	( double p_dIni, double p_dFin );
	long	DarSemilla	( void ) { return m_ulSemilla; }
	long	DarRand		( void ) { return m_ulRand; }
	long	DarN		( void ) { return m_ulN; }
#include  <iostream.h>

static long Rando= 0;

// Es necesario inicializar la semilla aleatoria con un número
void InicializaRandom(long l)
{
  Rando= l;
}

// Funcion que genera números aleatorios
double Rnd()
{
  Rando= (25173L * Rando + 12849L) % 65536L;
  return double(Rando) / double(65536);
}

// Programa principal
void main()
{
  int i;
  InicializaRandom(100);  // Iniciamos la semilla aleatoria en 100

  cout << endl << "Cien números aleatorios :"  <<endl;
  for (i= 0; i < 100; i++)
  {
    cout << Rnd() << "  ";
  }

  cout << endl << "Cien números aleatorios entre 1 y 1000 :"  <<endl;
  for (i= 0; i < 100; i++)
  {
    cout << int(Rnd()*1000) +1 << "  ";
  }
}


  Numero aleatorio entre dos numeros N y M:

  Valor= int(Rnd * (M-N+1)) + N

*/

