/*------------------------------------------------------------------------*\
|* sGame.cpp
\*------------------------------------------------------------------------*/

#include "sGame.h"
#include "sGameWindow.h"
#include "sGlobal.h"
#include "sOrigen.h"
#include "sys/sRenderSystem.h"
#include "sys/sInputSystem.h"
#include "sys/sCollisionSystem.h"
#include "sys/sPhysicsSystem.h"
#include <swat/input/cTeclado.h>
#include <swat/sOpenGL.h>
#include <swat/texturas/cGestorTexturas.h>
#include <swat/texturas/cGestorFuentes.h>
#include <tool/consola/cConsola.h>
#include <tool/sMath.h>
#include <tool/cTool.h>
#include <tool/cRandom.hpp>


/*------------------------------------------------------------------------*\
|* sGame.cpp
\*------------------------------------------------------------------------*/
sGame* sGame::m_instancia = nullptr;
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Constructor & Destructor
\*------------------------------------------------------------------------*/
sGame::sGame()
{
    m_width = sGlobal::m_iWidth;
    m_height = sGlobal::m_iHeight;
    // No puede haber mas de una instancia
    if (m_instancia)
    {
        cLog::error(" Error: No puede haber mas de una instancia de juego\n");
    }
    m_instancia = this;
}

sGame::~sGame()
{
    releaseBalls();
    delete m_pCollision;
    delete m_pPhysics;
    delete m_pInput;
    delete m_pRender;
    delete m_pMotor;
    m_instancia = nullptr;

    cLog::print(" Destructor sGame\n");
}
/*------------------------------------------------------------------------*/

//
// Problemas:
//  - como representamos el cambio de la velocidad inicial
//  - poner las letras: Aries ?
//    mostrar numero de bolas actuales
//    mostrar numero de bolas destruidas
//  - mostrar ayudas, transparentes.
//  - ajustar la parada, si las colisiones hacen que no se mueva
//    modificar velocidades.
//    Estan en colision permanente. Se mueven poco a poco, y se ponen
//    detras(o delante) sin marcar la collision.
//
//  - la parada se esta permitiendo en suelo: pos.y < radio + 1.0
//    y tambien si esta el flag de m_estaColisionando
//

#pragma region Init
//--------------------------------------------------------------------------
// Init de Game
//--------------------------------------------------------------------------
int sGame::init()
{
    m_pMotor = new cRandom();
    m_pRender = new sRenderSystem(m_width, m_height);
    m_pInput = new sInputSystem();
    m_pCollision = new sCollisionSystem();
    m_pPhysics = new sPhysicsSystem();

    miError(
        m_pMotor->inicia()      ||
        m_pRender->init()       ||
        sOpenGL::initOpenGL()
    );

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    cLog::print("\n");

    miError(
        cGestorTexturas::Instancia()->CargaTextura(kTextura.c_str(), kDirTexturas.c_str()) ||
        (m_pTextura = cGestorTexturas::Instancia()->GetTextura(kTextura.c_str())) == nullptr ||
        createFuentes(m_pRender->getMainWindow()) ||
        (m_pFonArialMin = cGestorFuentes::Instancia()->getFuente(static_cast<int>(eFuentes::ArialMin))) == nullptr ||
        (m_pFonArialMax = cGestorFuentes::Instancia()->getFuente(static_cast<int>(eFuentes::ArialMax))) == nullptr ||
        (m_pFonDef = cGestorFuentes::Instancia()->getFuente(static_cast<int>(eFuentes::Def))) == nullptr ||
        (m_pFonAgulon = cGestorFuentes::Instancia()->getFuente(static_cast<int>(eFuentes::Agulon))) == nullptr ||
        createOrigin() ||
        m_pRender->finishInit(this)
    );

    m_bIsRunning = true;
    return 0;
}


//--------------------------------------------------------------------------
// Creacion de las fuentes para escribir
//--------------------------------------------------------------------------
int sGame::createFuentes(sGameWindow* pWindow)
{
    cGestorFuentes* pGestorFuentes = cGestorFuentes::Instancia();

    // He especificado todos los parametros:
    miError(
        pGestorFuentes->cargaFuente(
            static_cast<int>(eFuentes::ArialMin),
            kFuenteArial.c_str(),
            6,
            20,
            0,
            eTipoFuentes::eFN01,
            eOldTipoTextura::eTNoReg,
            static_cast<int>(ePesoFuentes::ePEXTRA_LIGERO),
            false,
            false,
            false,
            false,
            false,
            kDirTexturas.c_str(),
            ANSI_CHARSET,
            false,
            1.0f,
            14.0f,
            1
        ) ||
        pGestorFuentes->cargaFuente(
            static_cast<int>(eFuentes::ArialMax),
            kFuenteArial.c_str(),
            0,
            0,
            0,
            eTipoFuentes::eFN01,
            eOldTipoTextura::eTNoReg,
            static_cast<int>(ePesoFuentes::ePEXTRA_LIGERO),
            false,
            false,
            false,
            false,
            false,
            kDirTexturas.c_str(),
            ANSI_CHARSET,
            false,
            1.0f,
            12.0f,
            1
        ) ||
        pGestorFuentes->cargaFuente(
            static_cast<int>(eFuentes::Def),
            kFuenteDef.c_str(),
            16,
            16,
            0,
            eTipoFuentes::eFT02,
            eOldTipoTextura::eFT2,
            static_cast<int>(ePesoFuentes::ePBOLD),
            false,
            false,
            false,
            false,
            false,
            kDirTexturas.c_str(),
            ANSI_CHARSET,
            false,
            1.0f,
            0.0f,
            1
        ) ||
        pGestorFuentes->cargaFuente(
            static_cast<int>(eFuentes::Agulon),             // identificador fuente (no es el de cItem)
            kFuenteAgulon.c_str(),                          // pcNombre
            16,                                             // ancho
            16,                                             // alto
            0,                                              // grupo
            eTipoFuentes::eFT02,                            // tipo fuentes
            eOldTipoTextura::eFT2,                          // old tipo textura
            static_cast<int>(ePesoFuentes::ePBOLD),         // peso
            false,                                          // italic
            false,                                          // underline
            false,                                          // strike out
            false,                                          // outline
            false,                                          // sombra
            kDirTexturas.c_str(),                           // dir texturas
            ANSI_CHARSET,                                   // charset
            false,                                          // full
            1.0f,                                           // fEsc
            0.0f,                                           // fOffset
            1                                               // inc sombra
        )
    );

    miError(pGestorFuentes->genera(pWindow));

    return 0;
}


//--------------------------------------------------------------------------
// Creacion del circulo origen de los disparos
// - hacemos tambien que el circulo origen tambien colisione con el resto
//   de criculos.
//--------------------------------------------------------------------------
int sGame::createOrigin()
{
    if (m_vecBolas.size() < sGlobal::m_limBolas)
    {
        m_pOrigen = new sOrigen();
        m_pOrigen->m_posicion = sGlobal::m_posOrigen;
        m_pOrigen->m_radio = sGlobal::m_fRadioOrigen;
        m_pOrigen->m_color = sGlobal::m_colorOrigen;
        m_pOrigen->m_vColorFlecha = sGlobal::m_colorFlecha;
        m_pOrigen->m_fdir = 40.0f;
        m_pOrigen->m_vecVelocidad = { 0.0f, 0.0f };

        cLog::print(" Origin: %3ld:  FDir: %6.3f:     Vel: [ %6.3f, %6.3f ]\n",
            m_pOrigen->m_bolaId,
            m_pOrigen->m_fdir,
            m_pOrigen->m_vecVelocidad.x, m_pOrigen->m_vecVelocidad.y);

        m_vecBolas.push_back(m_pOrigen);
    }
    return 0;
}


//--------------------------------------------------------------------------
#pragma endregion


#pragma region Eventos
//--------------------------------------------------------------------------
// Eventos de Game
//--------------------------------------------------------------------------
int sGame::eventos()
{
    m_bIsRunning = !mensajes();

    return 0;
}
//--------------------------------------------------------------------------
#pragma endregion


#pragma region Update
//--------------------------------------------------------------------------
// Update de Game
//--------------------------------------------------------------------------
int sGame::update(float fDeltaTime)
{
    /*--------------------------------------------------------------------*/
    // setCaption(fDeltaTime);
    /*--------------------------------------------------------------------*/
    m_pInput->update(this, fDeltaTime);
    m_pCollision->update(this, fDeltaTime);
    m_pPhysics->update(this, fDeltaTime);
    m_pRender->update(this, fDeltaTime);
    /*--------------------------------------------------------------------*/

    /*--------------------------------------------------------------------*/
    return 0;
}
//--------------------------------------------------------------------------
#pragma endregion


#pragma region Render
//--------------------------------------------------------------------------
// Render de Game
//--------------------------------------------------------------------------
int sGame::render()
{
    //
    // Lo dejo, para que se vea que estaba,
    // por cuestiones organizativas, pero ya aqui no vamos a hacer nada
    // - saber donde estan las cosas es lo mas importante
    //
    // el codigo se ha llevado al update del render system
    //

    return 0;
}
//--------------------------------------------------------------------------
#pragma endregion


#pragma region Rest of Elements
//--------------------------------------------------------------------------
// Rest of Elements
//--------------------------------------------------------------------------
bool sGame::isRunning()
{
    return m_bIsRunning;
}


cstatic sGame* sGame::instancia()
{
    return m_instancia;
}


void sGame::shootBall()
{
    if (m_vecBolas.size() < sGlobal::m_limBolas)
    {
        sBall* pBall = new sBall();
        pBall->m_radio = sGlobal::m_fRadio;

        float xPos = 2.0f * (sGlobal::m_fRadio * cos(deg2rad(m_pOrigen->m_fdir)));
        float yPos = 2.0f * (sGlobal::m_fRadio * sin(deg2rad(m_pOrigen->m_fdir)));

        float xVel = sGlobal::m_fVelocidadInicial * cos(deg2rad(m_pOrigen->m_fdir));
        float yVel = sGlobal::m_fVelocidadInicial * sin(deg2rad(m_pOrigen->m_fdir));

        pBall->m_posicion = m_pOrigen->m_posicion + glm::vec2(xPos, yPos);
        pBall->m_vecVelocidad = { xVel, yVel };

        float fVel = sMath::modulo(pBall->m_vecVelocidad);

        std::string sColor = "";
        if (sGlobal::m_colorBolas.r == 1.0f && sGlobal::m_colorBolas.g == 0.0f && sGlobal::m_colorBolas.b == 0.0f)
        {
            sColor = "Cyan";
            sGlobal::m_colorBolas.r = 0.0f;
            sGlobal::m_colorBolas.g = 1.0f;
            sGlobal::m_colorBolas.b = 1.0f;
        }
        else if (sGlobal::m_colorBolas.r == 0.0f && sGlobal::m_colorBolas.g == 1.0f && sGlobal::m_colorBolas.b == 1.0f)
        {
            sColor = "Yellow";
            sGlobal::m_colorBolas.r = 1.0f;
            sGlobal::m_colorBolas.g = 1.0f;
            sGlobal::m_colorBolas.b = 0.0f;
        }
        else if (sGlobal::m_colorBolas.r == 1.0f && sGlobal::m_colorBolas.g == 1.0f && sGlobal::m_colorBolas.b == 0.0f)
        {
            sColor = "Red";
            sGlobal::m_colorBolas.r = 1.0f;
            sGlobal::m_colorBolas.g = 0.0f;
            sGlobal::m_colorBolas.b = 0.0f;
        }
        pBall->m_color = sGlobal::m_colorBolas;

        cLog::print(" %6.6s  %3ld:  FDir: %6.3f:     Vel: [ %6.3f, %6.3f ] = %6.3f\n", 
            sColor.c_str(),
            pBall->m_bolaId,
            pBall->m_fdir,
            xVel, yVel, fVel);

        m_vecBolas.push_back(pBall);
    }
}


void sGame::releaseBalls()
{
    if (m_vecBolas.size() > 0)
    {
        for (auto* pBall : m_vecBolas)
        {
            delete pBall;
        }

        m_vecBolas.clear();
    }
}


void sGame::setCaption(float fDeltaTime)
{
    if (m_pRender->getMainWindow())
    {
        char vcFps[32];
        float fps = 1 / fDeltaTime;
        sprintf_s(vcFps, sizeof(vcFps) - 1, "%6.3f", fps);

        std::string sTitulo = cConsola::getNombreProceso();
        sTitulo += "    ";
        sTitulo += vcFps;
        sTitulo += "  frames/segundo";

        WCHAR wcTitulo[LON_BUFF / 8];
        cTool::copiaMultibyteToUnicode(sTitulo, wcTitulo, sizeof(wcTitulo));
        SetWindowText(m_pRender->getMainWindow()->getWindow(), wcTitulo);
    }
}


bool sGame::selectedOrigin(glm::vec2 posRaton)
{
    bool selected = false;
    if (m_pOrigen)
    {
        glm::vec2 myRaton = { posRaton.x, m_height - posRaton.y };
        glm::vec2 vdiff = m_pOrigen->m_posicion - myRaton;
        float dist = sMath::modulo(vdiff) - sGlobal::m_windowCaptionSize;
        selected = (dist < m_pOrigen->m_radio);
    }
    return selected;
}

//--------------------------------------------------------------------------
#pragma endregion


/*------------------------------------------------------------------------*\
|* Fin de sGame.h
\*------------------------------------------------------------------------*/
