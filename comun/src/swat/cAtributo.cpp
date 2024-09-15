//--------------------------------------------------------------------------
// cAtributo.cpp
//--------------------------------------------------------------------------

#include "cAtributo.h"
#include <tool/strings/cToken.h>


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cAtributo::cAtributo(void)
{
}

cAtributo::cAtributo(const cAtributo & oCopia)
    : m_sOrigen(oCopia.m_sOrigen)
    , m_sNombre(oCopia.m_sNombre)
    , m_oValor(oCopia.m_oValor)
{
}


cAtributo::~cAtributo(void)
{
}



//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
void cAtributo::setNombre(sString & sNombre)
{
    m_sNombre = sNombre;
}


sString & cAtributo::getNombre(void)
{
    return m_sNombre;
}


void cAtributo::setValor(sString & sValor)
{
    m_oValor.setValor(sValor.c_str());
}


void cAtributo::setValor(cValor &oValor)
{
    m_oValor.copia(oValor);
}


cValor & cAtributo::getValor(void)
{
    return m_oValor;
}


void cAtributo::setValorOrigen(sString & sValor)
{
    m_oValorOrigen.setValor(sValor.c_str());
}

cValor & cAtributo::getValorOrigen(void)
{
    return m_oValorOrigen;
}


void cAtributo::setOrigen(char * pcOrigen)
{
    m_sOrigen.load(pcOrigen);
    if (!m_sOrigen.empty())
    {
        cToken oTok;
        char * pcCad;
        bool bOk = false;

        pcCad = oTok.Str(pcOrigen, 0, "=");
        if (pcCad && oTok.hasToken())
        {
            m_sNombre.load(pcCad);
            m_sNombre.trim();

            pcCad = oTok.Str();
            if (pcCad && oTok.esFin())
            {
                sString sValor;
                sValor.load(pcCad);
                sValor.trim();
                sValor.trim('\"');

                setValor(sValor);
                setValorOrigen(sValor);
                bOk = true;
            }
        }

        if (!bOk)
        {
            sString sValor;

            m_sNombre.load("unknown");
            sValor.format("[%s]", m_sOrigen.c_str());

            setValor(sValor);
        }
    }
}


sString & cAtributo::getOrigen(void)
{
    return m_sOrigen;
}


//--------------------------------------------------------------------------
// Fin de cParser.cpp
//--------------------------------------------------------------------------
