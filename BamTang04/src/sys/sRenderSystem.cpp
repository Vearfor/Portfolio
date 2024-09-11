/*------------------------------------------------------------------------*\
|* sRenderSystem.cpp
\*------------------------------------------------------------------------*/

#include "sRenderSystem.h"
#include "../sGameWindow.h"
#include "../sGame.h"
#include "../sBall.h"
#include "../sGlobal.h"
#include "../sOrigen.h"
#include "../sExplosion.h"
#include <tool/consola/cConsola.h>
#include <tool/sMath.h>
#include <swat/cColor.h>
#include <swat/texturas/cGestorFuentes.h>


/*------------------------------------------------------------------------*/
sRenderSystem::sRenderSystem(int width, int height)
    : m_initWidth(width)
    , m_initHeight(height)
{
}

sRenderSystem::~sRenderSystem()
{
    delete m_pWindow;
    cLog::print(" Destructor sRenderSystem\n");
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
int sRenderSystem::init()
{
    m_pWindow = new sGameWindow(m_initWidth, m_initHeight);

    miError(m_pWindow->crea(
        0, 0,
        m_initWidth, m_initHeight,
        0.01f, 100.0f, 45.0f,
        32, 32,
        false,
        eEstiloW::eCaption,
        cConsola::getNombreProceso(),
        "OpenGL Window Class",
        nullptr
    ));

    m_pWindow->muestraVentana();
    return 0;
}


/*------------------------------------------------------------------------*/
int sRenderSystem::finishInit(sGame * pGame)
{
    m_pOrigen = pGame->getMainOrigin();
    m_pTexturaTest = pGame->getTextura();
    m_pFonArialMin = pGame->getFonArialMin();
    m_pFonArialMax = pGame->getFonArialMax();
    m_pFonDef = pGame->getFonDef();
    m_pFonAgulon = pGame->getFonAgulon();

    return 0;
}


/*------------------------------------------------------------------------*/
int sRenderSystem::update(sGame* pGame, float fDeltaTime)
{
    m_pWindow->clean();
    m_pWindow->begin();     // Escritura/dibujado en Primer plano
    {
        showFps(m_pFonAgulon, fDeltaTime);
        showOrigin(pGame, m_pFonArialMin);
        showTest(pGame);

        auto& vecBolas = pGame->getVecBolas();
        for (auto* pBall : vecBolas)
        {
            pBall->render(fDeltaTime);
        }

        showHelp(pGame,m_pFonArialMax, m_pFonArialMin);
    }
    m_pWindow->end();       // Termina la escritura en primer plano
    m_pWindow->swap();

    return 0;
}


/*------------------------------------------------------------------------*/
void sRenderSystem::showOrigin(sGame* pGame, cFuente* pFon)
{
    if (!sGlobal::m_bmostrarInfo)
        return;

    if (pGame->hayHelp())
        return;

    int x = 20;
    int y = static_cast<int>(getHeight()-30);

    mDo(pFon)->escribe(
        m_pWindow,
        glm::vec3(x, y, 0.0f),
        1.0f,
        cColor::vBlanco,
        "Informacion:"
    );

    mDo(pFon)->detalleTitulo(
        m_pWindow,
        mNextLine(0),
        1.0f, 
        cColor::vVerde,
        cColor::vAmarillo,
        "Velocidad:",
        "%6.3f", sGlobal::m_fVelocidadInicial);

    float fdir = m_pOrigen->m_fdir;

    mDo(pFon)->detalleTitulo(
        m_pWindow,
        mNextLine(0),
        1.0f,
        cColor::vVerde,
        cColor::vAmarillo,
        "Dir:",
        "%6.3f", fdir);

    char vcValor[32];

    if (sGlobal::m_hayGravedad)
    {
        sprintf_s(vcValor, sizeof(vcValor), "%6.3f (solo eje Y)", sGlobal::m_fGravedad);
    }
    else
        mCopia(vcValor, "---");

    mDo(pFon)->detalleTitulo(
        m_pWindow,
        mNextLine(0),
        1.0f,
        cColor::vCyan,
        cColor::vAmarillo,
        (sGlobal::m_hayGravedad)?"Hay gravedad:": "No hay gravedad",
        vcValor);

    if (sGlobal::m_hayFriccion)
    {
        sprintf_s(vcValor, sizeof(vcValor), "%6.3f (en los dos ejes)", sGlobal::m_fFriccionAire);
    }
    else
        mCopia(vcValor, "---");

    mDo(pFon)->detalleTitulo(
        m_pWindow,
        mNextLine(0),
        1.0f,
        cColor::vCyan,
        cColor::vBlanco,
        (sGlobal::m_hayFriccion) ? "Hay friccion por aire:" : "sin friccion",
        vcValor);

    // Velocidad del Origen de disparos:
    float vel = sMath::modulo(m_pOrigen->m_vecVelocidad);
    mDo(pFon)->detalleTitulo(
        m_pWindow,
        mNextLine(0),
        1.0f,
        cColor::vCyan,
        cColor::vBlanco,
        "Velocidad del Origen de Disparos:",
        "%6.3f", vel);

    mDo(pFon)->escribe(m_pWindow,
        mNextLine(0),
        1.0f,
        cColor::vBlanco,
        (sGlobal::m_hayChoqueOrigen) ?
        "Le afectan los choques, la gravedad, y la friccion" :
        "No le afectan, ni los choques, ni la gravedad, ni la friccion"
    );

    mDo(pFon)->escribe(m_pWindow,
        mNextLine(0),
        1.0f,
        cColor::vBlanco,
        "Cada rebote en las paredes le quita %4.2f de velocidad", (1.0f - sGlobal::m_fElasticidad)
    );

    mDo(pFon)->escribe(m_pWindow,
        mNextLine(0),
        1.0f,
        cColor::vBlanco,
        "Debajo de %4.2f de velocidad, se aplica un factor de frenado", sGlobal::m_fVelParada
    );

    mDo(pFon)->escribe(m_pWindow,
        mNextLine(0),
        1.0f,
        cColor::vBlanco,
        "si estamos colisionando o estamos en el suelo.", sGlobal::m_fVelParada
    );

    size_t numBolas = pGame->getVecBolas().size();

    mDo(pFon)->detalleTitulo(
        m_pWindow,
        mNextLine(0),
        1.0f,
        cColor::vVerde,
        cColor::vAmarillo,
        "Bolas presentes:",
        "%d", numBolas);

    mDo(pFon)->detalleTituloSiguiente(
        m_pWindow,
        0, 0,
        1.0f,
        cColor::vVerde,
        cColor::vAmarillo,
        "  posibles:",
        "%d", sGlobal::m_limBolas);

    mDo(pFon)->detalleTitulo(
        m_pWindow,
        mNextLine(0),
        1.0f,
        cColor::vVerde,
        cColor::vRojo,
        "Cuando se paran, las bolas mueren en:",
        "%4.2f segundos", sGlobal::m_fTiempoDestruccion);

    mDo(pFon)->detalleTitulo(
        m_pWindow,
        mNextLine(0),
        1.0f,
        cColor::vVerde,
        cColor::vRojo,
        "Y cuando explotan, desaparecen en:",
        "%4.2f segundos", sGlobal::m_fTiempoExplosion);

    sBall* pSelected = pGame->getSelected();
    if (pSelected)
    {
        if (pSelected->m_bolaId != 0)
        {
            // Velocidad del Seleccionado:
            float vel = sMath::modulo(pSelected->m_vecVelocidad);
            mDo(pFon)->detalleTitulo(
                m_pWindow,
                mNextLine(0),
                1.0f,
                cColor::vCyan,
                cColor::vBlanco,
                "Seleccionado:",
                "%ld   %6.3f", pSelected->m_bolaId, vel);

            if (pSelected->m_tiempo > 0.0f)
            {
                mDo(pFon)->detalleTitulo(
                    m_pWindow,
                    mNextLine(0),
                    1.0f,
                    cColor::vVerde,
                    cColor::vAmarillo,
                    "Tiempo:", "%6.3f", pSelected->m_tiempo);

                if (pSelected->m_pExplosion && pSelected->m_pExplosion->m_tiempo)
                {
                    mDo(pFon)->detalleTitulo(
                        m_pWindow,
                        mNextLine(0),
                        1.0f,
                        cColor::vAzul,
                        cColor::vRojo,
                        "Tiempo Explosion:", "%6.3f", pSelected->m_pExplosion->m_tiempo);
                }
            }
        }
    }
}


/*------------------------------------------------------------------------*/
void sRenderSystem::showFps(cFuente* poFon, float fDeltaTime)
{
    if (!sGlobal::m_bmostrarFps)
        return;

    sOpenGL::Act_blend();

    char vcFps[32];
    float fps = 1 / fDeltaTime;
    sprintf_s(vcFps, sizeof(vcFps) - 1, "%6.3f", fps);

    int Y = static_cast<int>(getHeight() - 25);
    poFon->escribe(
        m_pWindow,
        glm::ivec3(getWidth() - 220, Y, 0),
        1.0f,
        cColor::vRojo,
        "Fps:");

    poFon->escribe(
        m_pWindow,
        glm::ivec3(getWidth() - 135, Y, 0),
        1.0f,
        cColor::vAmarillo,
        vcFps);

    sOpenGL::Des_blend();
}


/*------------------------------------------------------------------------*/
void sRenderSystem::showTest(sGame * pGame)
{
    if (pGame->hayTest())
    {
        float x = getWidth() / 4.0f;
        float y = (getHeight() / 4.0f) * 3.0f;
        float fancho = getWidth() / 2.0f;
        float falto = getHeight() / 2.0f;

        //------------------------------------------------------------------
        // Porque el color en curso es fundamental ?
        //------------------------------------------------------------------
        sOpenGL::color(cColor::vBlanco);
        //------------------------------------------------------------------
        sOpenGL::textura(x, y, 0.0f, fancho, falto, m_pTexturaTest);
    }
}


/*------------------------------------------------------------------------*/
#define mEscribe \
    poFonResto->escribe(m_pWindow, mNextLine(0), 1.0f,
/*------------------------------------------------------------------------*/
//#define mSiguiente \
//    poFonResto->detalleTituloSiguiente(m_pWindow, 30, 0, 1.0f,
/*------------------------------------------------------------------------*/
void sRenderSystem::showHelp(sGame* pGame, cFuente* poFonTitulo, cFuente* poFonResto)
{
    if (pGame->hayHelp())
    {
        float fx = (getWidth() / 8.0f) * 2.0f;
        float fy = (getHeight() / 8.0f) * 6.0f;

        float fMargen = 20.0f;
        float xrect = fx - fMargen;
        float yrect = fy + fMargen;
        float fancho = (getWidth() / 2.0f) + (fMargen * 2.0f);
        float falto = (getHeight() / 2.0f) + (fMargen * 2.0f);
        // Profundidad del rectangulo transparente:
        float fDepth = -0.1f;

        // Textura blanca transparente: o imagen blanca transparente:
        sOpenGL::rectangulo(eCoordRectangulo::eTopLeft, GL_FILL, glm::vec3(xrect, yrect, fDepth), fancho, falto, cColor::vAzul_T);

        int x = static_cast<int>(fx);
        int y = static_cast<int>(fy);

        poFonTitulo->escribe(
            m_pWindow,
            glm::ivec3(x, y, 0),
            1.0f,
            cColor::vAmarillo, "- Ayuda -");

        /*----------------------------------------------------------------*/
        poFonResto->detalleTitulo(m_pWindow,
            glm::ivec3(x, y - 35, 0),
            1.0f,
            cColor::vAmarillo,
            cColor::vBlanco,
            "F1:", "");

        poFonResto->detalleTituloSiguiente(m_pWindow, 30, 0, 1.0f,
            cColor::vBlanco,
            cColor::vAmarillo,
            "Salir de la ayuda", ""
        );
        /*----------------------------------------------------------------*/
        mEscribe cColor::vAmarillo, "F2");
        poFonResto->detalleTituloSiguiente(m_pWindow, 30, 0, 1.0f, cColor::vBlanco, cColor::vAmarillo, ".quita/muestra FPS.", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vAmarillo, "Esc");
        poFonResto->detalleTituloSiguiente(m_pWindow, 30, 0, 1.0f, cColor::vBlanco, cColor::vAmarillo, "Salir del programa.", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vAmarillo, "I");
        poFonResto->detalleTituloSiguiente(m_pWindow, 50, 0, 1.0f, cColor::vBlanco, cColor::vAmarillo, "Mostrar/Ocultar Info.", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vAmarillo, "Space");
        poFonResto->detalleTituloSiguiente(m_pWindow, 10, 0, 1.0f, cColor::vBlanco, cColor::vAmarillo, "Dispara una bola..", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vAmarillo, "P");
        poFonResto->detalleTituloSiguiente(m_pWindow, 40, 0, 1.0f, cColor::vBlanco, cColor::vAmarillo, ".Activa/Desactiva pausa.", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vRojo, "G");
        poFonResto->detalleTituloSiguiente(m_pWindow, 40, 0, 1.0f, cColor::vCyan, cColor::vAmarillo, ".Activa/Desactiva GRAVEDAD.", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vVerde, "F");
        poFonResto->detalleTituloSiguiente(m_pWindow, 40, 0, 1.0f, cColor::vCyan, cColor::vAmarillo, ".Activa/Desactiva Friccion.", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vCyan      , "O          .Activa/Desactiva Colisiones Origen de Disparos.");
        mEscribe cColor::vCyan      , "             Colisiona con las Bolas, le afecta la Gravedad, le afecta la Friccion.");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vAmarillo, "R");
        poFonResto->detalleTituloSiguiente(m_pWindow, 40, 0, 1.0f, cColor::vBlanco, cColor::vAmarillo, ".Reset del Origen de Disparos: posicion y direccion.", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vAmarillo, "S");
        poFonResto->detalleTituloSiguiente(m_pWindow, 40, 0, 1.0f, cColor::vBlanco, cColor::vAmarillo, ".Stop Velocidad del Origen de Disparos.", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vVerde, "A");
        poFonResto->detalleTituloSiguiente(m_pWindow, 40, 0, 1.0f, cColor::vCyan, cColor::vAmarillo, ".Aumenta la velocidad inicial del 'shotBall'.", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vVerde, "D");
        poFonResto->detalleTituloSiguiente(m_pWindow, 40, 0, 1.0f, cColor::vCyan, cColor::vAmarillo, ".Decrementa la velocidad inicial del 'shotBall'.", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vVerde, "+ (Add)");
        poFonResto->detalleTituloSiguiente(m_pWindow, 40, 0, 1.0f, cColor::vAmarillo, cColor::vAmarillo, ".Sube la direccion del 'shotBall'.", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vVerde, "- (Subtract)");
        poFonResto->detalleTituloSiguiente(m_pWindow, 15, 0, 1.0f, cColor::vAmarillo, cColor::vAmarillo, ".Baja la direccion del 'shotBall'.", "");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vBlanco, "Flechas Direccion:");
        poFonResto->detalleTituloSiguiente(m_pWindow, 10, 0, 1.0f, cColor::vAmarillo, cColor::vAmarillo, ".Left-Right -- Up-Down.", "");
        mEscribe cColor::vAmarillo, "                                 Mueve el Origen de Disparos");
        /*----------------------------------------------------------------*/
        mEscribe cColor::vVerde, "Si mantenemos 'pinchado' el Origen de Disparos con el raton, se puede mover");
        mEscribe cColor::vBlanco, "T       .Tecla de Test.");
    }
    else
    {
        int x = static_cast<int>((getWidth() / 8.0f)*3.0f);         // ?
        int y = static_cast<int>((getHeight() - 30));

        // Textura blanca transparente
        poFonResto->detalleTitulo(
            m_pWindow,
            glm::ivec3(x, y, 0),
            1.0f,
            cColor::vAmarillo,
            cColor::vBlanco,
            "F1  ", "  Mostramos la Ayuda");
    }
}
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sRenderSystem.cpp
\*------------------------------------------------------------------------*/
