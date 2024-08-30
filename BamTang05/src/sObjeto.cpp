/*------------------------------------------------------------------------*\
|* sObjeto.h
\*------------------------------------------------------------------------*/

#include "sObjeto.h"


sObjeto::sObjeto()
{
}

sObjeto::sObjeto(const std::string& pathModelFile, glm::vec3& pos, glm::vec3 rot, glm::vec3 sca, cModel* pModel)
    : m_pathModelFile(pathModelFile)
    , m_position(pos)
    , m_scale(sca)
    , m_pModel(pModel)
{
}

sObjeto::~sObjeto()
{
    // Del puntero al modelo se encargara el Gestor de modelos: cGestorModelos, no tenemos que borrarlo nosotros.
}


int sObjeto::load()
{
    // Solo si no esta cargado, se carga
    if (!m_pModel)
    {
        //m_pModel = cGestorModelos::loadModel(m_pathModelFile);
    }
    return 0;
}


int sObjeto::render()
{
    if (m_pModel)
    {
        // si esta cargado, se renderiza
    }

    return 0;
}


/*------------------------------------------------------------------------*\
|* Fin de sObjeto.h
\*------------------------------------------------------------------------*/
