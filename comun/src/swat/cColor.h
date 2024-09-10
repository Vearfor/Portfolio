/*========================================================================*\
|* cColor.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include <tool/nComun.h>
#include <GLM/glm.hpp>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// class
//--------------------------------------------------------------------------
class _mExport cColor
{
public:

    // Si llega vNoColor: no se aplica:
    inline static const glm::vec4 vNoColor  {-1.0f,-1.0f,-1.0f,-1.0f };

    inline static const glm::vec4 vBlanco   { 1.0f, 1.0f, 1.0f, 1.0f };
    inline static const glm::vec4 vAzul     { 0.0f, 0.0f, 1.0f, 1.0f };
    inline static const glm::vec4 vVerde    { 0.0f, 1.0f, 0.0f, 1.0f };
    inline static const glm::vec4 vRojo     { 1.0f, 0.0f, 0.0f, 1.0f };
    inline static const glm::vec4 vAmarillo { 1.0f, 1.0f, 0.0f, 1.0f };
    inline static const glm::vec4 vCyan     { 0.0f, 1.0f, 1.0f, 1.0f };
    inline static const glm::vec4 vNegro    { 0.0f, 0.0f, 0.0f, 1.0f };

    inline static const glm::vec4 vFondo{ 0.3f, 0.6f, 1.0f, 1.0f };
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cColor.h
\*========================================================================*/
