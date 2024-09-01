//--------------------------------------------------------------------------
// cAtributo.cpp
//--------------------------------------------------------------------------

#include "cAtributo.h"
// #include "../../Util/cToken.h"


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cAtributo::cAtributo(void)
{
}

cAtributo::cAtributo(const cAtributo& oCopia)
    : m_sOrigen(oCopia.m_sOrigen)
    , m_sNombre(oCopia.m_sNombre)
    //, m_oValor(oCopia.m_oValor)
{
}


cAtributo::~cAtributo(void)
{
}



//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
void cAtributo::setNombre(std::string& sNombre)
{
    m_sNombre = sNombre;
}


std::string& cAtributo::getNombre(void)
{
    return m_sNombre;
}


//void cAtributo::setValor(std::string& sValor)
//{
//    m_oValor.setValor(sValor.Cad());
//}
//
//
//void cAtributo::setValor(cValor& oValor)
//{
//    m_oValor.copia(oValor);
//}


//cValor& cAtributo::getValor(void)
//{
//    return m_oValor;
//}


//void cAtributo::setValorOrigen(std::string& sValor)
//{
//    m_oValorOrigen.setValor(sValor.Cad());
//}
//
//cValor& cAtributo::getValorOrigen(void)
//{
//    return m_oValorOrigen;
//}


void cAtributo::setOrigen(char* pcOrigen)
{
    //m_sOrigen = pcOrigen;
    //if (!m_sOrigen.EsVacia())
    //{
    //    cToken oTok;
    //    char* pcCad;
    //    bool bOk = false;

    //    pcCad = oTok.Str(pcOrigen, 0, "=");
    //    if (pcCad && oTok.hasToken())
    //    {
    //        m_sNombre = pcCad;
    //        m_sNombre.trim();

    //        pcCad = oTok.Str();
    //        if (pcCad && oTok.esFin())
    //        {
    //            std::string sValor;
    //            sValor = pcCad;
    //            sValor.trim();
    //            sValor.trim('\"');

    //            setValor(sValor);
    //            setValorOrigen(sValor);
    //            bOk = true;
    //        }
    //    }

    //    if (!bOk)
    //    {
    //        std::string sValor;

    //        m_sNombre = "unknown";
    //        sValor.format("[%s]", m_sOrigen.Cad());

    //        setValor(sValor);
    //    }
    //}
}


std::string& cAtributo::getOrigen(void)
{
    return m_sOrigen;
}


//--------------------------------------------------------------------------
// Fin de cParser.cpp
//--------------------------------------------------------------------------
