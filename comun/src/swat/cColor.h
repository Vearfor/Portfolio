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

    //----------------------------------------------------------------------
    // Transparentes
    //----------------------------------------------------------------------
    inline static const glm::vec4 vBlanco_T     { 1.0f, 1.0f, 1.0f, 0.4f };
    inline static const glm::vec4 vAzul_T       { 0.0f, 0.0f, 1.0f, 0.4f };
    inline static const glm::vec4 vVerde_T      { 0.0f, 1.0f, 0.0f, 0.4f };
    inline static const glm::vec4 vRojo_T       { 1.0f, 0.0f, 0.0f, 0.4f };
    inline static const glm::vec4 vAmarillo_T   { 1.0f, 1.0f, 0.0f, 0.4f };
    inline static const glm::vec4 vCyan_T       { 0.0f, 1.0f, 1.0f, 0.4f };
    //----------------------------------------------------------------------
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cColor.h
\*========================================================================*/
