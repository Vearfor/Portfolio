/*========================================================================*\
|* cCamara.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
// #include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cCamara
{
protected:

    cCamara(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    ~cCamara();

    glm::vec3 _posCamara;   // posicion
    glm::vec3 _posTarget;   // posicion
    glm::vec3 _vectorUp;    // vector final calculado

    glm::vec3 _look;        // vector forward
    glm::vec3 _right;       // vector side

    // Vector Up fijo predominante, con el que se calculan los demas
    glm::vec3 _vectorWorldUp;

    // Eulers angles (in radians) en origen:
    // al final sexagesimales
    float _yaw;
    float _pitch;

    // De la matriz de perspectiva afecta a la camara
    // Lo dejamos tambien aqui:
    const float ZOOM_SENSITIVITY = -3.0;
    const float DEFAULT_FOV = 45.0f;
    float _fZoom;

    static float YAW;
    static float PITCH;

    virtual void updateCamaraVectors() {};

public:

    glm::mat4 getViewMatrix() const;

    virtual void rotate(float yaw, float pitch) {}
    virtual void move(const glm::vec3& offsetPos) {}

    void setPosition(const glm::vec3& pos);
    // void setTarget(const glm::vec3& pos);
    void setUp(const glm::vec3& vec);
    void setWorldUp(const glm::vec3& vec);

    const glm::vec3& getLook() const;
    const glm::vec3& getRight() const;
    const glm::vec3& getUp() const;

    void setYaw(float yaw);
    void setPitch(float pitch);

    const glm::vec3& getPosition(void) const;

    void setZoom(float zoom);
    float getZoom(void);
    void resetZoom();
    glm::vec3 getTarget();

    void arriba();
    void derecha();
    void abajo();
    void izquierda();
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
struct sWindow;
class cRaton;
//--------------------------------------------------------------------------
class cFpsCamara : public cCamara
{
    virtual void updateCamaraVectors() override;

public:

    cFpsCamara(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    ~cFpsCamara();

    virtual void rotate(float incYaw, float incPitch);
    virtual void move(const glm::vec3& offsetPos);

    void keyboard(sWindow* window, float dDeltaTime);
    void mouseMove(cRaton* pRaton);
    void mouseScroll(sWindow* pWindow, double deltaX, double deltaY);
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cRaton;
//--------------------------------------------------------------------------
class cOrbitCamara : public cCamara
{
    float _radius;

    virtual void updateCamaraVectors();

public:

    cOrbitCamara(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    ~cOrbitCamara();

    virtual void rotate(float yaw, float pitch);
    void setRadius(float rad);

    void mouseMove(cRaton* pRaton);
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cCamara.h
\*========================================================================*/
