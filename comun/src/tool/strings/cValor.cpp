//--------------------------------------------------------------------------
// cValor.cpp
//--------------------------------------------------------------------------


#include "cValor.h"
#include <tool/cTool.h>


//--------------------------------------------------------------------------
// Static
//--------------------------------------------------------------------------
//cValor  cValor::m_oValorNulo(eNoValido);
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cValor::cValor	( int iValor )
{
	inicioValor(iValor);
}

cValor::cValor (double dValor)
{
    inicioValor(dValor);
}

cValor::cValor (bool bValor)
{
    inicioValor(bValor);
}

cValor::cValor (const char * pcValor)
{
    inicioValor(pcValor);
}

cValor::cValor ( const sString & sValor )
{
    inicioValor(sValor);
}

cValor::cValor	( const cValor & oValor )
{
	copia(oValor);
}

cValor::~cValor	( void )
{
    deleteString();
}



//--------------------------------------------------------------------------
// Resto de metodos.
//--------------------------------------------------------------------------
void cValor::deleteString(void)
{
    if (m_eTipo == eTipoValor::eSTRING)
    {
        mDelete(m_uValor.psValor);
    }
}


void cValor::cargaString(const char * pcValor)
{
    m_eTipo = eTipoValor::eSTRING;

    // TODO:
    // ¿Quien y cuando se borra? s
    sString* psValor = new sString(pcValor);
    m_uValor.psValor = psValor;
    
    m_sNoString.load("");
}


void cValor::operator = ( const cValor & oValor )
{
    copia(oValor);
}


//--------------------------------------------------------------------------
// Copia de un Valor a otro.
// - para usar fuera de los constructores
//--------------------------------------------------------------------------
void cValor::copia( const cValor & oValor )
{
    deleteString();
    copiaInterna(oValor);
}


//--------------------------------------------------------------------------
// y este es el que usamos en le constructor.
// sin hacer deleteString
//--------------------------------------------------------------------------
void cValor::copiaInterna(const cValor & oValor)
{
    m_eTipo = oValor.m_eTipo;
    mCopia(m_vcOrigen, oValor.m_vcOrigen);
    m_sNoString.load("");

    switch (m_eTipo)
    {
        case eTipoValor::eSTRING:
            cargaString(oValor.m_uValor.psValor->cad());
            break;

        case eTipoValor::eDOUBLE:
            m_uValor.dValor = oValor.m_uValor.dValor;
            break;

        case eTipoValor::eLONG:
            m_uValor.lValor = oValor.m_uValor.lValor;
            break;

        case eTipoValor::eBOOL:
            m_uValor.bValor = oValor.m_uValor.bValor;
            break;

        case eTipoValor::eLISTA:
            m_uValor.pLista = oValor.m_uValor.pLista;
            break;

        case eTipoValor::eVOID:
            m_uValor.pVoid = oValor.m_uValor.pVoid;
            break;

        default:
            break;
    }
}


bool cValor::operator == ( const cValor & oValor )
{
    bool bIgual = (m_eTipo == oValor.m_eTipo);

    if (!bIgual)
        return false;

    bIgual = false;
	switch(m_eTipo)
	{
		case eTipoValor::eSTRING:
            if (m_uValor.psValor && oValor.m_uValor.psValor)
            {
                sString sValor1 = m_uValor.psValor->cad();
                sString sValor2 = oValor.m_uValor.psValor->cad();
                bIgual = (sValor1 == sValor2);
            }
			break;

		case eTipoValor::eDOUBLE:
			bIgual = (m_uValor.dValor == oValor.m_uValor.dValor);
			break;

		case eTipoValor::eLONG:
			bIgual = (m_uValor.lValor == oValor.m_uValor.lValor);
			break;

		case eTipoValor::eBOOL:
			bIgual = (m_uValor.bValor == oValor.m_uValor.bValor);
			break;

		case eTipoValor::eLISTA:
			bIgual = (m_uValor.pLista == oValor.m_uValor.pLista);
			break;

		case eTipoValor::eVOID:
            bIgual = (m_uValor.pVoid == oValor.m_uValor.pVoid);
			break;

		default:
			break;
	}

    return bIgual;
}


//--------------------------------------------------------------------------
// Cambiamos la cadena para indicar el origen de los valores.
//--------------------------------------------------------------------------
void cValor::setOrigen(const char * pcOrigen)
{
    mCopia(m_vcOrigen, pcOrigen);
}


//--------------------------------------------------------------------------
// inicio cValor para usar en los consturctores
// - funcion private
//--------------------------------------------------------------------------
void cValor::inicioValor(int iValor)
{
    // Inicio Valor, para usar en los constructores
    // No debe de tener el deleteString

    m_eTipo = eTipoValor::eLONG;
    m_uValor.lValor = iValor;
    mCopia(m_vcOrigen, "inicioValor int");

    m_sNoString.load("");
}


void cValor::inicioValor(double dValor)
{
    // Inicio Valor, para usar en los constructores
    // No debe de tener el deleteString

    m_eTipo = eTipoValor::eDOUBLE;
    m_uValor.dValor = dValor;
    mCopia(m_vcOrigen, "inicioValor double");

    m_sNoString.load("");
}


void cValor::inicioValor(bool bValor)
{
    m_eTipo = eTipoValor::eBOOL;
    m_uValor.bValor = bValor;
    mCopia(m_vcOrigen, "inicioValor bool");

    m_sNoString.load("");
}


void cValor::inicioValor(const sString & sValor)
{
    inicioValor(sValor.cad());
}


void cValor::inicioValor(const char * pcValor)
{
    m_sNoString.load("");
    if (pcValor == NULL)
    {
        m_eTipo = eTipoValor::eLONG;
        m_uValor.lValor = 0;
        mCopia(m_vcOrigen, "inicioValor Cadena Nula");
    }
    else
    {
        if (!strcmp(pcValor, "true") || !strcmp(pcValor, "false"))
        {
            m_eTipo = eTipoValor::eBOOL;
            m_uValor.bValor = !!mbTrue(pcValor);
            mCopia(m_vcOrigen, "inicioValor Cadena Booleana");
        }
        else
        {
            if (cTool::esNumero(pcValor))
            {
                if (cTool::esEntero(pcValor))
                {
                    m_eTipo = eTipoValor::eLONG;
                    m_uValor.lValor = atol(pcValor);
                    mCopia(m_vcOrigen, "inicioValor Cadena Entera");
                }
                else
                {
                    m_eTipo = eTipoValor::eDOUBLE;
                    m_uValor.dValor = atof(pcValor);
                    mCopia(m_vcOrigen, "inicioValor Cadena Double");
                }
            }
            else
            {
                cargaString(pcValor);
                mCopia(m_vcOrigen, "inicioValor Cadena Simple");
            }
        }
    }
}


//--------------------------------------------------------------------------
// reset cValor para usar fuera
// -  funcion public
// Si podra tener deleteString
//--------------------------------------------------------------------------
void cValor::reset ( int iValor )
{
    deleteString();
    m_eTipo = eTipoValor::eLONG;
    m_uValor.lValor = iValor;

    m_sNoString.load("");

    mCopia(m_vcOrigen, "reset int");
}


void cValor::reset ( double dValor )
{
    deleteString();
    m_eTipo = eTipoValor::eDOUBLE;
    m_uValor.dValor = dValor;

    m_sNoString.load("");

    mCopia(m_vcOrigen, "reset double");
}


//--------------------------------------------------------------------------
// Metodos sobrecargados de setValor
//--------------------------------------------------------------------------
void cValor::setValor ( const char * pcValor )
{
    deleteString();
    inicioValor(pcValor);

	// No sabemos como hacer que identifique la lista.
	// o el de cualquier puntero.
}

void cValor::setValor ( int p_iValor )
{
    deleteString();
	m_eTipo = eTipoValor::eLONG;
	m_uValor.lValor = p_iValor;
    m_sNoString.load("");
    mCopia(m_vcOrigen, "Entero");
}

void cValor::setValor ( long p_lValor )
{
    deleteString();
	m_eTipo = eTipoValor::eLONG;
	m_uValor.lValor = p_lValor;
    m_sNoString.load("");
    mCopia(m_vcOrigen, "Long");
}

void cValor::setValor ( bool p_bValor )
{
    deleteString();
	m_eTipo = eTipoValor::eBOOL;
	m_uValor.bValor = p_bValor;
    m_sNoString.load("");
    mCopia(m_vcOrigen, "Bool");
}

void cValor::setValor ( double p_dValor )
{
    deleteString();
	m_eTipo = eTipoValor::eDOUBLE;
	m_uValor.dValor = p_dValor;
    m_sNoString.load("");
    mCopia(m_vcOrigen, "Double");
}

void cValor::setValor(void * p_vValor)
{
    deleteString();
    m_eTipo = eTipoValor::eVOID;
    m_uValor.pVoid = p_vValor;
    m_sNoString.load("");
    mCopia(m_vcOrigen, "Puntero");
}


//--------------------------------------------------------------------------
// Utilicemos un Template
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Incrementa el valor interno, solo para los tipos en los cuales tiene
// sentido: eDOUBLE y eLONG
//--------------------------------------------------------------------------
void cValor::incrementaValor ( void )
{
	switch(m_eTipo)
	{
		case eTipoValor::eSTRING:
		case eTipoValor::eBOOL:
			break;		//

		case eTipoValor::eDOUBLE:
			m_uValor.dValor = m_uValor.dValor + 1;
			break;

		case eTipoValor::eLONG:
			m_uValor.lValor = m_uValor.lValor + 1;
			break;

		default:
			break;
	}
}


//--------------------------------------------------------------------------
// Devuelve el valor dado, segun el tipo, como un puntero a void.
// Para pasar su contenido a un valor de parametro.
//--------------------------------------------------------------------------
void  *	cValor::getParam ( void )
{
	void * pvParam = NULL;

	switch(m_eTipo)
	{
		case eTipoValor::eSTRING:
            if (m_uValor.psValor)
            {
                pvParam = (void *) m_uValor.psValor->cad();     // m_uValor.vcValor;
            }
			break;

		case eTipoValor::eDOUBLE:
			pvParam = (void *) &m_uValor.dValor;
			break;

		case eTipoValor::eLONG:
			pvParam = (void *) &m_uValor.lValor;
			break;

		case eTipoValor::eBOOL:
			pvParam = (void *) &m_uValor.bValor;
			break;

		case eTipoValor::eLISTA:
			pvParam = m_uValor.pLista;
			break;

		case eTipoValor::eVOID:
			pvParam = m_uValor.pVoid;
			break;

		default:
			break;
	}

	return pvParam;
}

//--------------------------------------------------------------------------
// Metodos de devolucion del valor del objeto para todos los tipos:
// eSTRING, eDOUBLE, eBOOL, eLONG, eLISTA, eVOID
//--------------------------------------------------------------------------
const char * cValor::getString()
{
	const char * pcRes = NULL;

    switch (m_eTipo)
    {
        case eTipoValor::eSTRING:
            if (m_uValor.psValor)
            {
                pcRes = m_uValor.psValor->cad();
            }
            break;

        case eTipoValor::eBOOL:
            {
                m_sNoString.format("%s", msTrue(m_uValor.bValor));
                pcRes = m_sNoString.cad();
            }
            break;
        case eTipoValor::eLONG:
            {
                m_sNoString.format("%d", m_uValor.lValor);
                pcRes = m_sNoString.cad();
            }
            break;
        case eTipoValor::eDOUBLE:
            {
                m_sNoString.format("%f", m_uValor.dValor);
                pcRes = m_sNoString.cad();
            }
            break;

        case eTipoValor::eLISTA:
        case eTipoValor::eVOID:
        case eTipoValor::eNUM_TIPOS_VALOR:
        default:
            break;
    }

	// pcRes = (m_eTipo==eTipoValor::eSTRING)? m_uValor.vcValor: NULL;
    return (const char *) pcRes;
}

double	cValor::getDouble ( void )
{
	double	dRes;
	dRes = (m_eTipo== eTipoValor::eDOUBLE)? m_uValor.dValor:
		(m_eTipo== eTipoValor::eLONG)? (double) m_uValor.lValor:
		eNoValido;
	return dRes;
}

int	cValor::getInt (void)
{
	int	iRes;
	iRes = (m_eTipo== eTipoValor::eLONG)? m_uValor.lValor:
		(m_eTipo== eTipoValor::eDOUBLE)? (int) m_uValor.dValor:
		eNoValido;
	return iRes;
}

long cValor::getLong (void)
{
	long	lRes;
	lRes = (m_eTipo== eTipoValor::eLONG)? m_uValor.lValor:
		(m_eTipo== eTipoValor::eDOUBLE)? (long) m_uValor.dValor:
		eNoValido;
	return lRes;
}

bool cValor::getBool (void)
{
	bool	bRes;
	bRes = (m_eTipo== eTipoValor::eBOOL)? m_uValor.bValor: false;
	return bRes;
}

void * cValor::getLista (void)
{
	void  *	pvRes;
	pvRes = (m_eTipo== eTipoValor::eLISTA)? m_uValor.pLista: NULL;
	return pvRes;
}

void  *	cValor::getVoid	(void)
{
	void  *	pvRes;
	pvRes = (m_eTipo== eTipoValor::eVOID)? m_uValor.pVoid: NULL;
	return pvRes;
}


//--------------------------------------------------------------------------
// Utilicemos un Template
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Comprobaciones sobre si el Valor guardado es valido o no.
//--------------------------------------------------------------------------
bool cValor::esNoValido	( void )
{
	bool	bRes;
//	bRes = (m_eTipo==eTipoValor::eLONG && m_uValor.lValor==m_lNoValido) || (m_eTipo==eTipoValor::eDOUBLE && m_uValor.dValor==m_lNoValido);
	bRes = (m_eTipo== eTipoValor::eLONG && m_uValor.lValor==eNoValido) || (m_eTipo== eTipoValor::eDOUBLE && m_uValor.dValor==eNoValido);
	return bRes;
}


bool cValor::esValido	( void )
{
	bool	bRes;
//	bRes = !((m_eTipo==eTipoValor::eLONG && m_uValor.lValor==m_lNoValido) || (m_eTipo==eTipoValor::eDOUBLE && m_uValor.dValor==m_lNoValido));
	bRes = !((m_eTipo== eTipoValor::eLONG && m_uValor.lValor==eNoValido) || (m_eTipo== eTipoValor::eDOUBLE && m_uValor.dValor==eNoValido));
	return bRes;
}


//--------------------------------------------------------------------------
// Necesitamos la representacion escrita de 
//--------------------------------------------------------------------------
const char * cValor::toString ( sString & sReserva, const char * pcFormat )
{
    sString sFormato;
    switch(m_eTipo)
    {
		case eTipoValor::eSTRING:
            sFormato.load( (pcFormat)? pcFormat: "%s");
            sReserva.format(sFormato.cad(), (m_uValor.psValor) ? m_uValor.psValor->cad() : "nulo");
            break;

		case eTipoValor::eDOUBLE:
            sFormato.load( (pcFormat)? pcFormat: "%f");
            sReserva.format(sFormato.cad(), m_uValor.dValor);
			break;

		case eTipoValor::eLONG:
            sFormato.load( (pcFormat)? pcFormat: "%ld");
            sReserva.format(sFormato.cad(), m_uValor.lValor);
			break;

		case eTipoValor::eBOOL:
            sFormato.load((pcFormat) ? pcFormat : "%s");
            sReserva.format(sFormato.cad(), msTrue(m_uValor.bValor));
			break;

		case eTipoValor::eLISTA:
            sFormato.load( (pcFormat) ? pcFormat : "<lista>");
            sReserva.format(sFormato.cad());
			break;

		case eTipoValor::eVOID:
            sFormato.load( (pcFormat) ? pcFormat : "<void>");
            sReserva.format(sFormato.cad());
			break;

		default:
			break;
    }

    return (const char *) sReserva.cad();
}



//--------------------------------------------------------------------------
// Fin de cValor.cpp
//--------------------------------------------------------------------------
