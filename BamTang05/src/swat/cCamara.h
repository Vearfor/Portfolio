/*========================================================================*\
|* cCamara.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <GLM/glm.hpp>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constantes
//--------------------------------------------------------------------------
const float YAW = 180.0f;
const float PITCH = 0.0f;
const float DEFAULT_FOV = 45.0f;
const glm::vec3 Pos_Origin = glm::vec3(0.0f, 0.0f, 0.0f);
const glm::vec3 VecUP_Y = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 VecUP_Z = glm::vec3(0.0f, 0.0f, 1.0f);
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cCamara
{
    glm::vec3 m_posCamara{ Pos_Origin };
    glm::vec3 m_posTarget{ Pos_Origin };
    glm::vec3 m_vecUp{ VecUP_Y };
    glm::vec3 m_vecSide{ Pos_Origin };      // Deberia salir del Cross entre el vecLook y el vecUp

    const glm::vec3 WORL_UP{ VecUP_Y };     // O podria ser VecUP_Z (para todos)

    float m_yaw{ YAW };
    float m_pitch{ PITCH };
    float m_fov{ DEFAULT_FOV };             // que para otros es el zoom.

    glm::mat4 m_view{ 1.0f };

public:

    cCamara(glm::vec3 position = Pos_Origin, float yaw = YAW, float pitch = PITCH);
    cCamara(
        glm::vec3 position,
        glm::vec3 target = Pos_Origin,
        glm::vec3 vUp = VecUP_Y
    );
    ~cCamara();

    void mousemove();

    void arriba();
    void derecha();
    void abajo();
    void izquierda();

    void update();

    glm::mat4 getViewMatrix();

    void setPosCamara(glm::vec3 pos) { m_posCamara = pos; }
    void setPosTarget(glm::vec3 target) { m_posTarget = target; }
    void setVecUp(glm::vec3 vecUp) { m_vecUp = vecUp; }
    glm::vec3 getPosCamara() { return m_posCamara; }
    glm::vec3 getPosTarget() { return m_posTarget; }
    glm::vec3 getVecUp() { return m_vecUp; }
    void setYaw(float yaw) { m_yaw = yaw; }
    void setPitch(float pitch) { m_pitch = pitch; }
    void setZoom(float fov) { m_fov = fov; }
    float getYaw() { return m_yaw; }
    float getPitch() { return m_pitch; }
    float getZoom() { return m_fov; }
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cCamara.h
\*========================================================================*/
