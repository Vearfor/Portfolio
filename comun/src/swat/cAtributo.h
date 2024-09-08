//--------------------------------------------------------------------------
// cAtributo.h
//--------------------------------------------------------------------------


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <tool/cValor.h>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cFile;
//--------------------------------------------------------------------------
class _mExport cAtributo
{
protected:

    sString     m_sOrigen;      // <nombre>="<valor>"
    sString     m_sNombre;      // nombre
    cValor      m_oValor;       // valor
    cValor      m_oValorOrigen; // Si teniamos un valor inicial, este lo guardamos aqui: para tener el nombre de un identificador.

public:

    cAtributo(void);
    cAtributo(const cAtributo & oCopia);
    virtual ~cAtributo(void);

    void setOrigen(char * pcNombre);
    sString & getOrigen(void);

    void setNombre(sString & sNombre);
    sString & getNombre(void);

    void setValor(sString & sValor);
    void setValor(cValor & oValor);
    cValor & getValor(void);

    void setValorOrigen(sString & sValor);
    cValor & getValorOrigen(void);

    void escribe(cFile * poFile, int iNivelAnidamiento);
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Fin de cAtributo.h
//--------------------------------------------------------------------------
