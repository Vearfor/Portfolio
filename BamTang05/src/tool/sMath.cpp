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
// Nos fabricamos una funcion normalize debido a que hay muchos valores
// que son ya muy pequeños y deben tomarse como cero
//--------------------------------------------------------------------------
cstatic glm::vec3 sMath::normalize(glm::vec3 vector)
{
    glm::vec3 normal = glm::normalize(vector);

    mSetZero(normal.x);
    mSetZero(normal.y);
    mSetZero(normal.z);

    return normal;
}
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de sMath.cpp
\*========================================================================*/
