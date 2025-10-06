/*========================================================================*\
|* cItem.h
\*========================================================================*/
//--------------------------------------------------------------------------
// Clase base
// - identificador incluido
//--------------------------------------------------------------------------


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <dll/nExport.h>
#include <string>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// struct
//--------------------------------------------------------------------------
struct _mExport sItem
{
    int		iIndex;         // Indice identificador.
    char	vcNombre[64];   // Nombre asociado al identificador
    void* pVoid;            // Puntero a otro tipo de informacion.

};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class _mExport cItem
{
    static  long    m_lNumItems;    // Static: numero de Items dados de alta (muchos, muchos, muchos ... )

    long    m_lIdInterno;           // Identificador interno (este si que sera distinto para todos)
    long    m_lIdIndex;             // Si tenemos el elemento guardado en una lista guardamos aqui su posicion en la lista.
                                    // Este mecanismo funcionara si no metemos el mismo elemento/puntero en mas de una lista
                                    // Funcionara en la ultima lista que haga AddItem.

    virtual void FreeMemory (void);

    void InicioItem (long lIdent);

protected:

    cItem(long lIdent = -1);
    cItem(const cItem & oCopia);

    virtual void ItemCopia(const cItem & oCopia);

    long m_lIdent;
    std::string m_sNombre;
    std::string m_sMostrar;
    void * m_pvMemory;
    long m_lonMemory;

public:

    virtual ~cItem(void);

    virtual void setIdent(long lIdent);
    virtual long getIdent(void);

    virtual void setIndex(long lIndex);
    virtual long getIndex(void);

    virtual long getIdentInterno(void);

    virtual void setNombre(const char * pcNombre);
    virtual void setNombre(const std::string & sNombre);
    virtual const char * getNombre(void) const;

    virtual void setMostrar(const char * pcMostrar);
    virtual void setMostrar(const std::string & sNombre);
    virtual const char * getMostrar(void) const;

    virtual bool operator == (const cItem & oItem);

    virtual void    setMemory       (void * pvMemory, long lLong);
    virtual void  * getMemory       (void);
    virtual long    getLonMemory    (void);
};
//--------------------------------------------------------------------------



/*========================================================================*\
|* Fin de cItem.h
\*========================================================================*/
