/*========================================================================*\
|* sMath.cpp
|*
|*  Created on: 31/08/2024
|*      Author: Enrique Rocafull Ortiz
|*
|* Compendio de funciones matematicas utilies desde 2010.
|*
\*========================================================================*/


#include "sMath.h"


//--------------------------------------------------------------------------
// Si estamos muy cerca de cero, pues zero:
//--------------------------------------------------------------------------
cstatic glm::vec3 sMath::setZero(glm::vec3 vec)
{
    glm::vec3 vZero = vec;

    mSetZero(vZero.x);
    mSetZero(vZero.y);
    mSetZero(vZero.z);

    return vZero;
}


//--------------------------------------------------------------------------
// Nos fabricamos una funcion normalize debido a que hay muchos valores
// que son ya muy pequeños y deben tomarse como cero
//--------------------------------------------------------------------------
cstatic glm::vec3 sMath::normalize(glm::vec3 vector)
{
    glm::vec3 normal = glm::normalize(vector);

    normal = setZero(normal);

    return normal;
}
//--------------------------------------------------------------------------
cstatic float sMath::getFactorReduccion(float fFactorDecr, float fDeltaTime)
{
    float fDelta = fDeltaTime;
    float fDecExp = fFactorDecr;

    mSetZero(fDelta);
    float fFactor = exp(-fDecExp * fDelta);
    return fFactor;
}


/*========================================================================*\
|* Fin de sMath.cpp
\*========================================================================*/
