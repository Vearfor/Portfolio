/*==========================================================================*\
|* cToken.cpp
\*==========================================================================*/


#include "cToken.h"


//--------------------------------------------------------------------------
// Constructores
//--------------------------------------------------------------------------
cToken::cToken()
    : m_bFin(false)			// Indica si hemos llegado el fin de la cadena.
    , m_bHay(false)			// Indica si aun saliendo ha encontrado token.
    , m_poBuffer(NULL)		// Donde se guarda la copia.
    , m_iPuntero(0)			// Puntero en la cadena.
    , m_ppcSep(NULL)		// Lista de separadores (Patrones).
    , m_iNumSep(0)			// Numero de separadores (Patrones).
    , m_poToken(NULL)		// Cadena hasta token.
    //, m_pLComent(NULL)      // Lista de comentarios admitidos en la cadena.
    , m_iIniLectura(0)
{
}

cToken::~cToken()
{
    // mDelete(m_pLComent);
	mDelete(m_poBuffer);
	mDelete(m_poToken);
	LimpiaLista();
}



//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
int	cToken::Inicia(const char * pcBuffer, long lLon, const char ** ppcSep, int iNumSep)
{
	if	(m_poBuffer==NULL)
	{
        m_poBuffer = new sString();
		miNulo(m_poBuffer);

        mDelete(m_poToken);
        m_poToken = new sString();
		miNulo(m_poToken);
	}

	if	(mNoVacia(pcBuffer))
	{
		m_bFin = false;							// Indica si hemos llegado el fin de la cadena.
		m_bHay = false;							// Indica si aun saliendo ha encontrado token.
		m_iPuntero = 0;							// Puntero en la cadena.
        m_iIniLectura = 0;
        // mDo(m_pLComent)->ReInicia();

        long lLonFinal = (lLon==0)? (long) strlen(pcBuffer): lLon;
		miError(m_poBuffer->load( pcBuffer, lLonFinal ));
		m_poToken->limpia();
	}

	miError(CopiaLista(ppcSep,iNumSep));

	return 0;
}


//--------------------------------------------------------------------------
// Limpia lista de separadores
//--------------------------------------------------------------------------
int	cToken::LimpiaLista	( void )
{
	if	(m_ppcSep!=NULL)
	{
		for	(int i=0; i<m_iNumSep; i++ )
		{
            delete m_ppcSep[i];
            m_ppcSep[i] = NULL;
		}
        delete[] m_ppcSep;
        m_ppcSep = NULL;
	}

	return 0;
}


//--------------------------------------------------------------------------
// Copia lista de separadores
//--------------------------------------------------------------------------
int	cToken::CopiaLista	( const char ** p_ppcSep, int iNumSep )
{
	if	(p_ppcSep)
	{
		char	vcMens	[32];
		char ** ppcSep;
		int	iNSep, i, iLen;

		iNSep = iNumSep;	// Numero de separadores
		if	(iNSep==0)
		{
			char ** ppcAux;
			char * pcAux;
			ppcAux = (char **) p_ppcSep;
			for	(
					pcAux=ppcAux[iNSep];
					pcAux;
				)
			{
				iNSep++;
				pcAux=ppcAux[iNSep];	// Puede estar realizando un acceso no debido.
			}
		}

		int iSepmas1 = iNSep + 1;
        ppcSep = new char * [iSepmas1];
        miNulo(ppcSep);
        // setNew(ppcSep, "Lista separadores");

		for ( i=0; i<iNSep; i++ )
		{
			if	(mNoVacia(p_ppcSep[i]))
			{
				iLen = (int) strlen((char *) p_ppcSep[i]);
				mInicio(vcMens);
				sprintf_s(vcMens, sizeof(vcMens), "Separador %d", i);
				int ilonmas1 = iLen + 1;
                ppcSep[i] = new char[ilonmas1];
				miNulo(ppcSep[i]);
				strncpy_s(ppcSep[i], ilonmas1, p_ppcSep[i], iLen);
			}
		}
		ppcSep[iNSep]=NULL;

		miError(LimpiaLista());
		m_ppcSep = ppcSep;
		m_iNumSep = iNSep;
	}

	return 0;
}


//--------------------------------------------------------------------------
// Construye el resultado.
//--------------------------------------------------------------------------
int	cToken::Resultado	( int p_iPosAnt )
{
	if	(m_iPuntero == p_iPosAnt)
	{
		// Creo que no lo ha encontrado
		char  * pcCad;
        long    lLonFinal;

		pcCad = (char *) m_poBuffer->cad() + m_iPuntero;
        lLonFinal = m_poBuffer->lon() - m_iPuntero;    // Longitud hasta el final del buffer
		m_poToken->load(pcCad, lLonFinal);
		m_bFin = true;
		m_bHay = false;
		m_iPuntero = m_poBuffer->lon();
	}
	else
	{
		m_bFin = false;  // Llega hasta el fin
		m_bHay = true;
	}

	return 0;
}


//--------------------------------------------------------------------------
// Informa de la lista de separadores
//--------------------------------------------------------------------------
int cToken::ListaSeparadores	( const char ** p_ppcSep, int p_iNumSep )
{
	miError(CopiaLista(p_ppcSep,p_iNumSep));

	return 0;
}


//--------------------------------------------------------------------------
// Str:: el principal de Token
//--------------------------------------------------------------------------
char * cToken::Str	( const char * pcBuffer, int iLon, const char ** ppcSep, int iNumSep )
{
	mpError(Inicia(pcBuffer,iLon,ppcSep,iNumSep));

	char  *	pcOrigen	= (char *) m_poBuffer->cad();
	int		iLongitud	= m_poBuffer->lon();
	int		i, iCurAnt, iLonComentario;
    bool    bDentroComentario;
	char  *	pcCad;

	for	(
			iCurAnt=m_iPuntero, i=m_iPuntero, m_bHay=false;
			i<iLongitud && !m_bHay;
			i++
		)
	{
		pcCad = pcOrigen + i;

        bDentroComentario = esComentario(pcCad, iLonComentario);
        i += iLonComentario;
        pcCad = pcOrigen + i;

		if	(!bDentroComentario)
		{
			char  *	pcSep;
			int		l, j, iLen;

			for ( l=0; l<m_iNumSep; l++ )
			{
				pcSep = m_ppcSep[l];
				iLen = (int) strlen(pcSep);

				if ( !memcmp(pcOrigen + i, pcSep, iLen) )
				{
					for ( j=0; j<iLen; j++ )
						pcOrigen[i+j]=0;
                    m_poToken->load(pcOrigen + m_iPuntero, i - m_iPuntero);
					m_iPuntero = i + iLen;
                    m_bHay = true;
                    break;
				}    
			}
		}
	}
	Resultado(iCurAnt);

	return (char *) m_poToken->cad();
}


// Hay que transformar la funcion con el patron en una unica funcion
// con lista de separadores ...
// El elemento patron no debe de existir ...
char * cToken::Str ( const char * pcBuffer, int iLon, const char * pcPatron )
{
    const char * vcSep[2];

    vcSep[0] = (char *) pcPatron;
    vcSep[1] = NULL;

    return Str(pcBuffer, iLon, vcSep, 1);
}


char * cToken::Str	( const char * pcBuffer, const char * pcPatron )
{
    const char * vcSep[2];

    vcSep[0] = (char *) pcPatron;
    vcSep[1] = NULL;

    return Str(pcBuffer, 0, vcSep, 1);
}


char * cToken::Str	( void )
{
    // Para todos los casos ...
    return Str(NULL, 0, NULL, 0);	// Busqueda de lista de separadores o de patrones.
}



//--------------------------------------------------------------------------
// Devuelve la cadena que hay desde el final hasta que encuentra el
// separador.
// Supone que el destno tiene longitud suficiente y de que tiene memoria.
//--------------------------------------------------------------------------
int	cToken::RevToken ( const char * pcOrigen, int iLon, const char * pcSep, char * pcDestino )
{
	int i;

	if (!pcOrigen)
		return 1;
	if (!pcDestino)
		return 1;

	for ( i=iLon-1; i>0; i-- )
	{
		if ( !strncmp(pcOrigen+i-strlen(pcSep),pcSep,strlen(pcSep)) )
		{
			strcpy_s(pcDestino, iLon,pcOrigen+i);
			return i;
		}
	}

	return 0;
}


//--------------------------------------------------------------------------
// Con la combinacion de los Str por lista de separadores y los Str por
// patron, se construyen los Leer para obtener valores para un patron
// y separadores dados.
//--------------------------------------------------------------------------
char  *	cToken::leer ( char * pcBuffer, int lLon, const char * pcPatron, const char ** ppcSep, int iNumSep )
{
    m_iIniLectura = 0;

	Str(pcBuffer,lLon,pcPatron);

    // Ha llegado al fin de la cadena, y no lo ha encontrado ...
	if	(m_bFin)
		return NULL;

    m_iIniLectura = getPuntero();

    return Str(NULL,0,ppcSep,iNumSep);
}


char  *	cToken::leer ( void )
{
    m_iIniLectura = 0;

	char * pcToken;

	Str(NULL,NULL);

	if	(m_bFin)
		return NULL;

    m_iIniLectura = getPuntero();

	pcToken = Str(NULL,0,NULL,0);

	return pcToken;
}


//--------------------------------------------------------------------------
// Devolucion del ultimo token encontrado.
//--------------------------------------------------------------------------
char * cToken::getToken	( void )
{
	char * pcToken = NULL;

	if (m_poToken)
	{
		pcToken = (char *) m_poToken->cad();
	}

	return pcToken;
}


//--------------------------------------------------------------------------
// Longitud del token actual
//--------------------------------------------------------------------------
int	cToken::getLonToken	 ( void )
{
	int	iLon = 0;

	if	(m_poToken)
	{
		iLon = m_poToken->lon();
	}

	return iLon;
}


//--------------------------------------------------------------------------
// Establece los Tokens de los comentarios: inicial y final
//--------------------------------------------------------------------------
void cToken::addComentario(int iIdent, const char * pcIni, const char * pcFin)
{
    //// Solo al añadir comentarios, creamos la lista de comentarios:
    //if (m_pLComent == NULL)
    //{
    //    m_pLComent = new cListaComentarios("Lista de Comentarios");
    //    m_pLComent->Inicio();
    //}

    //if (m_pLComent)
    //{
    //    m_pLComent->Add(iIdent, pcIni, pcFin);

    //    m_pLComent->bEsComentario = false;			// Estamos dentro de comentario
    //    if (m_pLComent->iNumComentarios>0)
    //    {
    //        m_pLComent->bHayComentarios = true;
    //    }
    //}
}


//--------------------------------------------------------------------------
// Nos basta con poner el inicio del comentario
// Encontrado el de inicio, tambien quita el de fin
//--------------------------------------------------------------------------
void cToken::removeComentario(int iIdent)
{
    //if (m_pLComent)
    //{
    //    m_pLComent->Remove(iIdent);

    //    m_pLComent->bEsComentario = false;			// Estamos dentro de comentario
    //    if (m_pLComent->iNumComentarios<1)
    //    {
    //        m_pLComent->bHayComentarios = false;
    //    }
    //}
}


//--------------------------------------------------------------------------
// Decide si estamos en entorno de comentarios para continuar o no
// comprobando tokens.
//--------------------------------------------------------------------------
bool cToken::esComentario ( char * pcCad, int & iLonComentario )
{
	bool bRes = false;
    //iLonComentario = 0;

    //if (m_pLComent)
    //{
    //    if (m_pLComent->bHayComentarios)
    //    {
    //        if (m_pLComent->bEsComentario)
    //        {
    //            if (m_pLComent->EsFinComentario(pcCad, iLonComentario))
    //            {
    //                m_pLComent->bEsComentario = false;
    //            }
    //        }
    //        else
    //        {
    //            if (m_pLComent->EsInicioComentario(pcCad, iLonComentario))
    //            {
    //                m_pLComent->bEsComentario = true;
    //            }
    //        }
    //        bRes = m_pLComent->bEsComentario;
    //    }
    //}

	return bRes;
}


/*==========================================================================*\
|* Una vez empecemos a trocear los tokens por los separadores determinados
|* Nos puede interesar dar lo que queda del buffer.
|* Si no hemos troceado nada devolvera la cadena inicial,
|* hasta lo que hemos avanzado ...
\*==========================================================================*/
char  * cToken::getFinal ( void )
{
    char  * pcCad;

    pcCad = (char *) m_poBuffer->cad() + m_iPuntero;

    return pcCad;
}


/*==========================================================================*\
|* Valores a los que no dabamos acceso
\*==========================================================================*/
char  * cToken::getBuffer   ( void )
{
    return (char *) m_poBuffer->cad();
}


long    cToken::getLongitud ( void )
{
    return m_poBuffer->lon();
}


/*==========================================================================*\
|* Fin de cToken.cpp
\*==========================================================================*/
