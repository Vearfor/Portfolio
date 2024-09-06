/*------------------------------------------------------------------------*\
|* sGame.cpp
\*------------------------------------------------------------------------*/

#include "sGame.h"
#include "sGameWindow.h"
#include "sGlobal.h"
#include "sOrigen.h"
#include "sCollisionSystem.h"
#include "sPhysicsSystem.h"
#include <swat/input/cTeclado.h>
#include <swat/sOpenGL.h>
#include <tool/consola/cConsola.h>
#include <tool/sMath.h>
#include <tool/cTool.h>


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
    delete m_pWindow;
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
//    Actualmente se quedan quietos pero no mueren. Estan en colision
//    permanente.
//  - la parada solo se esta permitiendo en suelo: pos.y < radio + 1.0
//    quitarlo ?
//  - usar raton para arrastrar el origen de disparos.
//

#pragma region Init
//--------------------------------------------------------------------------
// Init de Game
//--------------------------------------------------------------------------
int sGame::init()
{
    m_pWindow = new sGameWindow(m_width, m_height);

    miError(m_pWindow->crea(
        0, 0,
        m_width, m_height,
        0.01f, 100.0f, 45.0f,
        32, 32,
        false,
        eEstiloW::eCaption,
        cConsola::getNombreProceso(),
        "OpenGL Window Class",
        nullptr
    ));

    m_pWindow->muestraVentana();

    auto& rect = m_pWindow->getCurrentRect();

    m_pCollision = new sCollisionSystem(rect.getAncho(), rect.getAlto());
    m_pPhysics = new sPhysicsSystem();
    // Para que el systema de collision pueda actualizar sus limites:
    m_pWindow->m_pCollision = m_pCollision;

    miError(sOpenGL::initOpenGL());

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    cLog::print("\n");

    createOrigin();

    m_bIsRunning = true;
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
    cTeclado* pTeclado = m_pWindow->m_pTeclado;
    if (pTeclado)
    {
        /*----------------------------------------------------------------*\
        |* There is the call of asked function
        \*----------------------------------------------------------------*/
        if (pTeclado->isUp(VK_SPACE))
        {
            shootBall();
        }
        /*----------------------------------------------------------------*/

        if (pTeclado->isDown(VK_ESCAPE))
        {
            m_pWindow->destruyeVentana();
        }

        if (pTeclado->isUp('P'))
        {
            m_esPausa = !m_esPausa;
        }

        if (pTeclado->isUp('G'))
        {
            sBall::m_hayGravedad = !sBall::m_hayGravedad;
        }

        if (pTeclado->isUp('F'))
        {
            sBall::m_hayFriccion = !sBall::m_hayFriccion;
        }

        if (pTeclado->isUp('T'))
        {
            m_esTest = !m_esTest;
        }

        /*----------------------------------------------------------------*\
        |* Directions Keys: to move origin object
        \*----------------------------------------------------------------*/
        if (pTeclado->isDown(VK_LEFT))
        {
            m_pOrigen->izquierda(fDeltaTime);
        }
        if (pTeclado->isDown(VK_RIGHT))
        {
            m_pOrigen->derecha(fDeltaTime);
        }
        if (pTeclado->isDown(VK_UP))
        {
            m_pOrigen->arriba(fDeltaTime);
        }
        if (pTeclado->isDown(VK_DOWN))
        {
            m_pOrigen->abajo(fDeltaTime);
        }
        /*----------------------------------------------------------------*/
        if (pTeclado->isDown(VK_ADD))
        {
            m_pOrigen->cambiaDir(eIncrGrados::eSuma);
        }
        if (pTeclado->isDown(VK_SUBTRACT))
        {
            m_pOrigen->cambiaDir(eIncrGrados::eResta);
        }
        /*----------------------------------------------------------------*/

        pTeclado->reset();
    }

    /*--------------------------------------------------------------------*/
    m_pCollision->update(this, fDeltaTime);
    m_pPhysics->update(this, fDeltaTime);
    /*--------------------------------------------------------------------*/
    setCaption(fDeltaTime);
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
    m_pWindow->clean();

    m_pWindow->begin();     // Escritura/dibujado en Primer plano
    {
        for (auto* pBall : m_vecBolas)
        {
            pBall->render();
        }
    }
    m_pWindow->end();       // Termina la escritura en primer plano

    m_pWindow->swap();

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


void sGame::createOrigin()
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
}


void sGame::shootBall()
{
    if (m_vecBolas.size() < sGlobal::m_limBolas)
    {
        sBall* pBall = new sBall();
        pBall->m_radio = sGlobal::m_fRadio;

        float xPos = 2.0f * (sGlobal::m_fRadio * cos(deg2rad(m_pOrigen->m_fdir)));
        float yPos = 2.0f * (sGlobal::m_fRadio * sin(deg2rad(m_pOrigen->m_fdir)));

        float xVel = sGlobal::m_fVelocidad * cos(deg2rad(m_pOrigen->m_fdir));
        float yVel = sGlobal::m_fVelocidad * sin(deg2rad(m_pOrigen->m_fdir));

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
    if (m_pWindow)
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
        SetWindowText(m_pWindow->getWindow(), wcTitulo);
    }
}
//--------------------------------------------------------------------------
#pragma endregion


/*------------------------------------------------------------------------*\
|* Fin de sGame.h
\*------------------------------------------------------------------------*/
