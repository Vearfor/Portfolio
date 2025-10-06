/*========================================================================*\
|* cCamara.h
\*========================================================================*/


#include "cCamara.h"
#include <GLM/gtx/transform.hpp>
#include "input/cRaton.h"
#include "../tool/sMath.h"


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
    // Cuidado: tenemos dos Fov: cual utilizamos ?
    // - quizas este, el de la camara
    // - y que el Fov de la ventana sea el de la camara ?
    // - ...................... por decidir ToDo !!!
    // ...
    //    ¿Camara y ventana estan intrinsecamnete unidos? La camara es de una escena, las ventanas reproducen muchas escenas !!!
    //    El Fov pertenece a la escena, y la camara es de una escena ...
    //    Lo mismo ocurre con el Frutrum final: plano cercano y plano lejano ...
    //    La view sea 3D (perspectiva), o 2D (ortho, primer plano) es de la escena.
    // ...
    , m_fov(DEFAULT_FOV)
{
    setValoresIniciales(m_posCamara, m_yaw, m_pitch, m_fov);
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
// El que toque con el vector look
//--------------------------------------------------------------------------
void cCamara::adelante(float fDeltaTime)
{
    m_offsetPos = cRaton::getSpeed() * fDeltaTime * getVecLook();
}

void cCamara::atras(float fDeltaTime)
{
    m_offsetPos = - cRaton::getSpeed() * fDeltaTime * getVecLook();
}
//--------------------------------------------------------------------------
// El que toque con el vector side
//--------------------------------------------------------------------------
void cCamara::izquierda(float fDeltaTime)
{
    m_offsetPos = - cRaton::getSpeed() * fDeltaTime * getVecSide();
}

void cCamara::derecha(float fDeltaTime)
{
    m_offsetPos = cRaton::getSpeed() * fDeltaTime * getVecSide();
}
//--------------------------------------------------------------------------
// El que toque con el vector Up (o el Z o el Y: segun se decida)
//--------------------------------------------------------------------------
void cCamara::arriba(float fDeltaTime)
{
    m_offsetPos = cRaton::getSpeed() * fDeltaTime * getVecUp();
}

void cCamara::abajo(float fDeltaTime)
{
    m_offsetPos = - cRaton::getSpeed() * fDeltaTime * getVecUp();
}
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Direccion dadas por yaw y pitch
//--------------------------------------------------------------------------
void cCamara::direccion(glm::vec2 vecDelta, float fDeltaTime)
{
    m_incAngle = vecDelta * cRaton::getSensitivity() * fDeltaTime;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Funciones de update
//--------------------------------------------------------------------------
void cCamara::move(glm::vec3 offsetPos)
{
    if (!sMath::isZero(offsetPos))
        m_posCamara += offsetPos;
}


void cCamara::rotate(float offsetx, float offsety)
{
    if (offsetx != 0 || offsety!=0)
    {
        m_yaw -= offsetx;
        m_pitch -= offsety;

        m_yaw =
            (m_yaw > 360.0f) ? m_yaw - 360.0f :
            (m_yaw < 0.0f) ? m_yaw + 360.0f :
            m_yaw;

        m_pitch = glm::clamp(m_pitch, -90.0f + 0.1f, 90.0f - 0.1f);
    }
}


void cCamara::mouseScroll(float deltaWheel, float mouseSensivity)
{
    float fov = getZoom() + deltaWheel * mouseSensivity;
    fov = glm::clamp(fov, 1.0f, 120.0f);
    setZoom(fov);
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Update: aplica los cambios, eventos ocurridos antes 
//--------------------------------------------------------------------------
void cCamara::update(float fDeltaTime)
{
    // aplicamos m_offsetPos si toca
    move(m_offsetPos);
    float fFactorO = sMath::getFactorReduccion(3.0f, fDeltaTime);
    m_offsetPos *= fFactorO;

    // aplicamos m_incAngle si toca
    rotate(m_incAngle.x, m_incAngle.y);
    float fFactorA = sMath::getFactorReduccion(3.0f, fDeltaTime);
    m_incAngle *= fFactorA;

    float yawRadians = glm::radians(m_yaw);
    float pitchRadians = glm::radians(m_pitch);

    glm::vec3 look;
    look.x = cosf(pitchRadians) * sinf(yawRadians);
    look.y = sinf(pitchRadians);
    look.z = cosf(pitchRadians) * cosf(yawRadians);
    m_vecLook = sMath::normalize(look);

    glm::vec3 side;
    side = glm::cross(m_vecLook, WORL_UP);
    m_vecSide = sMath::normalize(side);

    glm::vec3 up;
    up = glm::cross(m_vecSide, m_vecLook);
    m_vecUp = sMath::normalize(up);

    m_posTarget = m_posCamara + m_vecLook;

    // Hacemos que el view se cambie fuera del render:
    m_view = glm::lookAt(m_posCamara, m_posTarget, m_vecUp);
}
//--------------------------------------------------------------------------


glm::mat4 cCamara::getViewMatrix()
{
    return m_view;
}


//--------------------------------------------------------------------------
// Restablece valores iniciales, por si perdemos las referencias.
//--------------------------------------------------------------------------
void cCamara::reset()
{
    setPosCamara(m_posInicial);
    setYaw(m_yawInicial);
    setPitch(m_pitchInicial);
    // ToDo: rehacer perspectiva ?
    m_fov = m_fovInicial;

    m_incAngle = glm::vec2( 0.0f );
    m_offsetPos = glm::vec3( 0.0f );
    incZoom = 0.0f;
}


void cCamara::setValoresIniciales(glm::vec3 pos, float yaw, float pitch, float fov)
{
    m_posInicial = pos;
    m_yawInicial = yaw;
    m_pitchInicial = pitch;
    m_fovInicial = fov;
}
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cCamara.h
\*========================================================================*/
