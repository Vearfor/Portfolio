/*========================================================================*\
|* sRenderObject.cpp
\*========================================================================*/

#include "sRenderObject.h"
#include "../tool/cLog.h"
#include "../tool/nComun.h"
#include "shaders/cShader.h"
#include "texturas/cTextura.h"
#include "cMalla.h"
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>


sRenderObject::sRenderObject()
{
}
    
sRenderObject::~sRenderObject()
{
}


void sRenderObject::render(cShader* pShader, int locModel, cRect<float> * pInRect)
{
    glm::vec3 pos{ pInRect->left, 0.0, pInRect->top };

    render(pShader, locModel, pos);
}


void sRenderObject::render(cShader* pShader, int locModel, glm::vec3 posRender)
{
    mDo(m_pTextura)->useTextura(0);
    glm::mat4 localModel{ 1.0 };
    localModel = glm::translate(localModel, posRender);
    localModel = glm::scale(localModel, m_escala);
    pShader->SetUniform(locModel, localModel);
    m_pMalla->drawMalla();
}


/*========================================================================*\
|* Fin de sRenderObject.cpp
\*========================================================================*/
