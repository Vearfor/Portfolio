/*========================================================================*\
|* cCamara.h
\*========================================================================*/


#include "cCamara.h"
#include <GLM/gtx/transform.hpp>
#include "input/cRaton.h"


#pragma region cCamara
//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cCamara::cCamara(glm::vec3 position, float yaw, float pitch)
    : m_posCamara(position)
    , m_posTarget(0.0f, 0.0f, 0.0f)
    // Vector Up predominante
    , m_vecUp(VecUP_Y)
    // Angulos Euler
    , m_yaw(yaw)
    , m_pitch(pitch)
    // De la perspectiva pero afecta a la camara
    , m_fov(DEFAULT_FOV)
{
}

cCamara::cCamara(
    glm::vec3 position,
    glm::vec3 target,
    glm::vec3 vecUp
)
    : m_posCamara(position)
    , m_posTarget(target)
    , m_vecUp(vecUp)
{
}

cCamara::~cCamara()
{
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
void cCamara::mousemove()
{

}

void cCamara::arriba()
{
    //_posCamara.y = _posCamara.y + 0.5f;
}


void cCamara::derecha()
{
    //_posCamara.x = _posCamara.x + 0.5f;
}


void cCamara::abajo()
{
    //_posCamara.y = _posCamara.y - 0.5f;
}

void cCamara::izquierda()
{
    //_posCamara.x = _posCamara.x - 0.5f;
}


//--------------------------------------------------------------------------
// Update: aplica los cambios, eventos ocurridos antes 
//--------------------------------------------------------------------------
void cCamara::update()
{
    // Hacemos que el view se cambie fuera del render:
    m_view = glm::lookAt(m_posCamara, m_posTarget, m_vecUp);
}
//--------------------------------------------------------------------------


glm::mat4 cCamara::getViewMatrix()
{
    return m_view;
}


//--------------------------------------------------------------------------
//void cCamara::setWorldUp(const glm::vec3& vec)
//{
//    _vectorWorldUp = vec;
//}
//
//
//void cCamara::setZoom(float zoom)
//{
//    _fZoom = zoom;
//}
//
//
//void cCamara::setYaw(float yaw)
//{
//    _yaw = yaw;
//}
//
//
//void cCamara::setPitch(float pitch)
//{
//    _pitch = pitch;
//}
//
//
//const glm::vec3& cCamara::getPosition(void) const
//{
//    return _posCamara;
//}
//
//
//float cCamara::getZoom(void)
//{
//    return _fZoom;
//}
//
//const glm::vec3& cCamara::getLook() const
//{
//    return _look;
//}
//
//
//const glm::vec3& cCamara::getRight() const
//{
//    return _right;
//}
//
//
//const glm::vec3& cCamara::getUp() const
//{
//    return _vectorUp;
//}
//
//
//void cCamara::resetZoom()
//{
//    _fZoom = DEFAULT_FOV;
//}
//
//
//void cCamara::arriba()
//{
//    //_posCamara.y = _posCamara.y + 0.5f;
//}
//
//
//void cCamara::derecha()
//{
//    //_posCamara.x = _posCamara.x + 0.5f;
//}
//
//
//void cCamara::abajo()
//{
//    //_posCamara.y = _posCamara.y - 0.5f;
//}
//
//void cCamara::izquierda()
//{
//    //_posCamara.x = _posCamara.x - 0.5f;
//}
//
//
////--------------------------------------------------------------------------
//#pragma endregion
//
//
//#pragma region cFpsCamara
////--------------------------------------------------------------------------
//// Constructor & Destructor
////--------------------------------------------------------------------------
//cFpsCamara::cFpsCamara(glm::vec3 position, float yaw, float pitch)
//    : cCamara(position, yaw, pitch)
//{
//}
//
//cFpsCamara::~cFpsCamara()
//{
//}
////--------------------------------------------------------------------------
//
//
////--------------------------------------------------------------------------
//// Resto de metodos
////--------------------------------------------------------------------------
//void cFpsCamara::keyboard(sWindow* window, float dDeltaTime)
//{
//    bool bCambia = false;
//    glm::vec3 offsetPos = glm::vec3(0.0f, 0.0f, 0.0f);
//
//    // Forward/Backward
//    //if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
//    //{
//    //    offsetPos = cRaton::getSpeed() * dDeltaTime * getLook();
//    //    bCambia = true;
//    //}
//    //else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
//    //{
//    //    offsetPos = -cRaton::getSpeed() * dDeltaTime * getLook();
//    //    bCambia = true;
//    //}
//
//    // Left/Right
//    //if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
//    //{
//    //    offsetPos = -cRaton::getSpeed() * dDeltaTime * getRight();
//    //    bCambia = true;
//    //}
//    //else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
//    //{
//    //    offsetPos = cRaton::getSpeed() * dDeltaTime * getRight();
//    //    bCambia = true;
//    //}
//
//    // Up/Down
//    //if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
//    //{
//    //    offsetPos = cRaton::getSpeed() * dDeltaTime * getUp();
//    //    bCambia = true;
//    //}
//    //else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
//    //{
//    //    offsetPos = -cRaton::getSpeed() * dDeltaTime * getUp();
//    //    bCambia = true;
//    //}
//
//    if (bCambia)
//    {
//        move(offsetPos);
//    }
//}
//
//
//void cFpsCamara::mouseScroll(sWindow* pWindow, double deltaX, double deltaY)
//{
//    double fov = getZoom() + deltaY * ZOOM_SENSITIVITY;
//    fov = glm::clamp(fov, 1.0, 120.0);
//    setZoom((float)fov);
//}
//
//
//void cFpsCamara::mouseMove(cRaton* pRaton)
//{
//    if (pRaton->LeftButton())
//    {
//        glm::vec2 incAngle = pRaton->getDelta() * cRaton::getSensitibity();
//
//        rotate(incAngle.x, incAngle.y);
//    }
//}
//
//
//void cFpsCamara::rotate(float incYaw, float incPitch)
//{
//    _yaw -= incYaw;
//    _pitch -= incPitch;
//
//    _yaw =
//        (_yaw > 360.0f) ? _yaw - 360.0f :
//        (_yaw < 0.0f) ? _yaw + 360.0f :
//        _yaw;
//
//    _pitch = glm::clamp(_pitch, -90.0f + 0.1f, 90.0f - 0.1f);
//    updateCamaraVectors();
//}
//
//
//void cFpsCamara::move(const glm::vec3& offsetPos)
//{
//    _posCamara += offsetPos;
//    updateCamaraVectors();
//}
//
//
//void cFpsCamara::updateCamaraVectors()
//{
//    float yawRadians = glm::radians(_yaw);
//    float pitchRadians = glm::radians(_pitch);
//
//    glm::vec3 look;
//    look.x = cosf(pitchRadians) * sinf(yawRadians);
//    look.y = sinf(pitchRadians);
//    look.z = cosf(pitchRadians) * cosf(yawRadians);
//    _look = glm::normalize(look);
//
//    _right = glm::normalize(glm::cross(_look, _vectorWorldUp));
//    _vectorUp = glm::normalize(glm::cross(_right, _look));
//
//    _posTarget = _posCamara + _look;
//}
////--------------------------------------------------------------------------
//#pragma endregion
//
//
//#pragma region cOrbitCamara
////--------------------------------------------------------------------------
//// Constructor & Destructor
////--------------------------------------------------------------------------
//cOrbitCamara::cOrbitCamara(glm::vec3 position, float yaw, float pitch)
//    : cCamara(position, yaw, pitch)
//    , _radius(10.0f)
//{
//}
//
//cOrbitCamara::~cOrbitCamara()
//{
//}
////--------------------------------------------------------------------------
//
//
////--------------------------------------------------------------------------
//// Resto de metodos
////--------------------------------------------------------------------------
//void cOrbitCamara::setRadius(float radius)
//{
//    _radius = glm::clamp(radius, 2.0f, 80.0f);
//}
//
//
//void cOrbitCamara::rotate(float yaw, float pitch)
//{
//    _yaw = yaw;
//    _pitch = pitch;
//
//    //_yaw = glm::radians(yaw);
//    //_pitch = glm::clamp(glm::radians(pitch), -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);
//    _yaw =
//        (_yaw > 360.0f) ? _yaw - 360.0f :
//        (_yaw < 0.0f) ? _yaw + 360.0f :
//        _yaw;
//
//    _pitch = glm::clamp(_pitch, -90.0f + 0.1f, 90.0f - 0.1f);
//
//    updateCamaraVectors();
//}
//
//
//void cOrbitCamara::updateCamaraVectors()
//{
//    float yawRadians = glm::radians(_yaw);
//    float pitchRadians = glm::radians(_pitch);
//
//    _posCamara.x = _posTarget.x + _radius * cosf(pitchRadians) * sinf(yawRadians);
//    _posCamara.y = _posTarget.y + _radius * sinf(pitchRadians);
//    _posCamara.z = _posTarget.z + _radius * cosf(pitchRadians) * cosf(yawRadians);
//}
//
//
//void cOrbitCamara::mouseMove(cRaton* pRaton)
//{
//    bool bHayCambio = false;
//
//    if (pRaton->LeftButton())
//    {
//        glm::vec2 inc = pRaton->getDelta() * cRaton::getSensitibity();
//
//        _yaw -= inc.x;
//        _pitch += inc.y;
//
//        _yaw =
//            (_yaw > 360.0f) ? _yaw - 360.0f :
//            (_yaw < 0.0f) ? _yaw + 360.0f :
//            _yaw;
//
//        _pitch = glm::clamp(_pitch, -90.0f + 0.1f, 90.0f - 0.1f);
//
//        bHayCambio = true;
//    }
//
//    if (pRaton->RighttButton())
//    {
//        glm::vec2 inc = pRaton->getDelta() * 0.01f;
//
//        _radius += inc.x - inc.y;
//        _radius = glm::clamp(_radius, 2.0f, 80.0f);
//
//        bHayCambio = true;
//    }
//
//    if (bHayCambio)
//    {
//        updateCamaraVectors();
//    }
//}
////--------------------------------------------------------------------------
//#pragma endregion


/*========================================================================*\
|* Fin de cCamara.h
\*========================================================================*/
