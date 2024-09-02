//--------------------------------------------------------------------------
// cAtributo.h
//--------------------------------------------------------------------------


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
//#include "../../Util/cString.h"
// #include "../../Util/cValor.h"
//--------------------------------------------------------------------------
#include <string>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
//class cFile;
//--------------------------------------------------------------------------
class cAtributo
{
protected:

    std::string     m_sOrigen;      // <nombre>="<valor>"
    std::string     m_sNombre;      // nombre
    //cValor      m_oValor;       // valor
    //cValor      m_oValorOrigen; // Si teniamos un valor inicial, este lo guardamos aqui: para tener el nombre de un identificador.

public:

    cAtributo(void);
    cAtributo(const cAtributo& oCopia);
    virtual ~cAtributo(void);

    void setOrigen(char* pcNombre);
    std::string& getOrigen(void);

    void setNombre(std::string& sNombre);
    std::string& getNombre(void);

    //void setValor(std::string& sValor);
    //void setValor(cValor& oValor);
    //cValor& getValor(void);

    //void setValorOrigen(std::string& sValor);
    //cValor& getValorOrigen(void);

    //void escribe(cFile* poFile, int iNivelAnidamiento);
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Fin de cAtributo.h
//--------------------------------------------------------------------------
