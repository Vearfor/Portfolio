/*========================================================================*\
|* cItem.cpp
\*========================================================================*/


#include "cItem.h"


//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
long    cItem::m_lNumItems = 0;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cItem::cItem(long lIdent)
    : m_lIdent(lIdent)
    , m_sNombre("")
    , m_sMostrar("")
    , m_pvMemory(NULL)
    , m_lonMemory(0)
{
    InicioItem(lIdent);
}

cItem::cItem(const cItem & oCopia)
    : m_pvMemory(NULL)
    , m_lonMemory(0)
{
    ItemCopia(oCopia);
}

cItem::~cItem(void)
{
    FreeMemory();
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
void cItem::ItemCopia(const cItem & oCopia)
{
    // La copia no debe significar un InicioItem, con un nuevo m_lNumItems++

    m_lIdInterno = oCopia.m_lIdInterno;
    m_lIdIndex = oCopia.m_lIdIndex;
    m_lIdent = oCopia.m_lIdent;
    m_sNombre = oCopia.m_sNombre;
    m_sMostrar = oCopia.m_sMostrar;
    // Aqui, hay que hacer una copia
    // ya que al borrar se borra en uno, pero no en la copia
    setMemory(oCopia.m_pvMemory, oCopia.m_lonMemory);
}


//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
void cItem::InicioItem(long lIdent)
{
    m_lNumItems++;
    m_lIdInterno = m_lNumItems;
    m_lIdent = lIdent;
    m_lIdIndex = 0;
    m_sNombre = "";
    m_sMostrar = "";
}


void cItem::setIdent(long lIdent)
{
    m_lIdent = lIdent;
}


long cItem::getIdent(void)
{
    return m_lIdent;
}


void cItem::setIndex(long lIndex)
{
    m_lIdIndex = lIndex;
}


long cItem::getIndex(void)
{
    return m_lIdIndex;
}


long cItem::getIdentInterno(void)
{
    return m_lIdInterno;
}


void cItem::setNombre(const char * pcNombre)
{
    m_sNombre = pcNombre;
}


void cItem::setNombre(const std::string & sNombre)
{
    m_sNombre = sNombre;
}


const char * cItem::getNombre(void) const
{
    return m_sNombre.c_str();
}


void cItem::setMostrar(const char * pcMostrar)
{
    m_sMostrar = pcMostrar;
}


void cItem::setMostrar(const std::string& sMostrar)
{
    m_sMostrar = sMostrar;
}


const char * cItem::getMostrar(void) const
{
    return (const char *) m_sMostrar.c_str();
}


bool cItem::operator == (const cItem & oItem)
{
    bool bRes;

    bRes = (m_lIdent == oItem.m_lIdent);

    return bRes;
}


//--------------------------------------------------------------------------
// Metodos para utilizar una especificacion directa de los elementos
// variables de cada cItem
//--------------------------------------------------------------------------
void cItem::setMemory ( void * pMem, long lLong )
{
    // Si antes tiene valor debe borrarse:
    if (m_pvMemory)
    {
        free(m_pvMemory);
        m_pvMemory = nullptr;
    }
    m_lonMemory = 0;

    if (pMem && lLong>0)
    {
        m_pvMemory = static_cast<void *>(malloc(lLong));
        if (m_pvMemory)
        {
            memcpy(m_pvMemory, pMem, lLong);
            m_lonMemory = lLong;
        }
    }
}


void * cItem::getMemory (void)
{
    return m_pvMemory;
}


long cItem::getLonMemory (void)
{
    return m_lonMemory;
}


void cItem::FreeMemory (void)
{
    if (m_pvMemory)
    {
        free(m_pvMemory);
        m_pvMemory = nullptr;
        m_lonMemory = 0;
    }
}
//==========================================================================



/*========================================================================*\
|* Fin de cItem.cpp
\*========================================================================*/
