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


//--------------------------------------------------------------------------
cstatic float* sMath::getMatriz(glm::mat4 mat)
{
    return (float*) &mat;
}


//--------------------------------------------------------------------------
// Si estamos muy cerca de cero, pues zero:
//--------------------------------------------------------------------------
cstatic glm::vec2 sMath::setZero(glm::vec2 vec)
{
    glm::vec2 vZero = vec;

    mSetZero(vZero.x);
    mSetZero(vZero.y);

    return vZero;
}


cstatic glm::vec3 sMath::setZero(glm::vec3 vec)
{
    glm::vec3 vZero = vec;

    mSetZero(vZero.x);
    mSetZero(vZero.y);
    mSetZero(vZero.z);

    return vZero;
}


cstatic glm::vec4 sMath::setZero(glm::vec4 vec)
{
    glm::vec4 vZero = vec;

    mSetZero(vZero.x);
    mSetZero(vZero.y);
    mSetZero(vZero.z);
    mSetZero(vZero.w);

    return vZero;
}


//--------------------------------------------------------------------------
cstatic bool sMath::isZero(glm::vec2 vec)
{
    return mEsZero(vec.x) && mEsZero(vec.y);
}


//--------------------------------------------------------------------------
cstatic bool sMath::isZero(glm::vec3 vec)
{
    return mEsZero(vec.x) && mEsZero(vec.y) && mEsZero(vec.z);
}


//--------------------------------------------------------------------------
cstatic bool sMath::isZero(glm::vec4 vec)
{
    return mEsZero(vec.x) && mEsZero(vec.y) && mEsZero(vec.z) && mEsZero(vec.w);
}


//--------------------------------------------------------------------------
cstatic float sMath::modulo(glm::vec2 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}


//--------------------------------------------------------------------------
cstatic float sMath::modulo(glm::vec3 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}


//--------------------------------------------------------------------------
cstatic float sMath::modulo(glm::vec4 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}


//--------------------------------------------------------------------------
cstatic float sMath::getAngulo(glm::vec2 vector)
{
    // No se si sera esta, o la perpendicular, vamos probando:
    float fdir = rad2deg(atan2f(vector.y, vector.x));

    return fdir;
}


//--------------------------------------------------------------------------
// La dir va cambiando de valor.
// cuando sea negativa la corregimos para que este en el rango de
// 0.0f a 360.0f
//--------------------------------------------------------------------------
cstatic float sMath::corrigeDir(float dir)
{
    float fdir = dir;
    if (fdir < 0.0f)
    {
        fdir = 360.0f + fdir;
    }
    return fdir;
}


//--------------------------------------------------------------------------
// No se como llamarlo:
// - a una cantidad 'todo' la reducimos por la cantidad 'tanto por uno a
//   aConservar'
//   A aConservar debe de ser menor que uno.
//   Si fuese mayor que uno, aumentaria.
//
// De donde viene: 
// La velocidad: la anterior repartida aplicando la reduccion de elasticidad:
// Quitamos decremento, no multiplicamos:
//
//--------------------------------------------------------------------------
cstatic float sMath::aplicaFactor(float todo, float aConservar)
{
    float reduccionDeTodo = 0.0f;

    float quito = todo * (1 - aConservar);
    reduccionDeTodo = todo - quito;

    return reduccionDeTodo;
}


/*========================================================================*\
|* Fin de sMath.cpp
\*========================================================================*/
