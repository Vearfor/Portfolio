/*========================================================================*\
|* sEscena.cpp
\*========================================================================*/


#include "sEscena.h"
#include "../cCamara.h"
#include <GLM/gtc/matrix_transform.hpp>


//--------------------------------------------------------------------------
sEscena::sEscena()
{
}

sEscena::~sEscena()
{
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Genera Mtrices de proyeccion
//--------------------------------------------------------------------------
void sEscena::generaMatrices(float anchoOnSize, float altoOnSize)
{
    float fAspect = (altoOnSize > 0) ? anchoOnSize / altoOnSize : 1.0f;

    // float fov = (m_poCamara) ? m_poCamara->getZoom() : m_fFov;

    m_perspective = glm::perspective<float>(glm::radians(m_fFov), fAspect, m_fPCercano, m_fPLejano);
    //                          left  right                    bottom                  top   cercano             lejano.
    // Tenemos pendiente probar el primer plano.
    m_ortho = glm::ortho<float>(0.0f, anchoOnSize, 0.0f, altoOnSize, -100.0f, 100.0f);

    // m_ortho = glm::ortho<float>(0.0f, anchoOnSize, altoOnSize, 0.0f, m_fPCercano, m_fPLejano);
    // m_ortho = glm::ortho<float>(0.0f, anchoOnSize, 0.0f, altoOnSize, -100.0f, 100.0f);

}


/*========================================================================*\
|* sEscena.cpp
\*========================================================================*/
