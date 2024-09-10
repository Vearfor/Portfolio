/*------------------------------------------------------------------------*\
|* sRenderSystem.cpp
\*------------------------------------------------------------------------*/

#include "sRenderSystem.h"
#include "../sGameWindow.h"
#include "../sGame.h"
#include "../sBall.h"
#include "../sGlobal.h"
#include "../sOrigen.h"
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
}


/*------------------------------------------------------------------------*/
void sRenderSystem::showFps(cFuente* poFon, float fDeltaTime)
{
    char vcFps[32];
    float fps = 1 / fDeltaTime;
    sprintf_s(vcFps, sizeof(vcFps) - 1, "%6.3f", fps);

    int Y = static_cast<int>(getHeight() - 25);
    poFon->escribe(
        m_pWindow,
        glm::ivec3(getWidth() - 220, Y, 0),
        1.0f,
        cColor::vBlanco,
        "Fps:");

    poFon->escribe(
        m_pWindow,
        glm::ivec3(getWidth() - 135, Y, 0),
        1.0f,
        cColor::vBlanco,
        vcFps);
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
void sRenderSystem::showHelp(sGame* pGame, cFuente* poFonTitulo, cFuente* poFonResto)
{
    if (pGame->hayHelp())
    {
        float x = (getWidth() / 8.0f) * 4.0f;
        float y = (getHeight() / 8.0f) * 7.0f;
        float fancho = getWidth() / 2.0f;
        float falto = getHeight() / 2.0f;

        // Textura blanca transparente
        poFonTitulo->escribe(
            m_pWindow,
            glm::ivec3(static_cast<int>(x), static_cast<int>(y), 0),
            1.0f,
            cColor::vAmarillo, "- Ayuda -");

        poFonResto->escribe(m_pWindow,
            glm::ivec3(static_cast<int>(x), static_cast<int>(y - 35), 0),
            1.0f,
            cColor::vBlanco, "F1: Salir de la ayuda");

        mEscribe cColor::vBlanco, "Esc.    .Salir del programa.");
        mEscribe cColor::vBlanco, "I    .Mostrar/Ocultar Info.");
        mEscribe cColor::vBlanco    , "Espace  .Dispara una bola.");
        mEscribe cColor::vBlanco    , "P       .Activa/Desactiva pausa.");
        mEscribe cColor::vRojo      , "G       .Activa/Desactiva GRAVEDAD.");
        mEscribe cColor::vVerde     , "F       .Activa/Desactiva Friccion.");
        mEscribe cColor::vCyan      , "O       .Activa/Desactiva Colisiones Origen de Disparos.");
        mEscribe cColor::vCyan      , "          Colisiona con las Bolas, le afecta la Gravedad, le afecta la Friccion.");
        mEscribe cColor::vBlanco, "R       .Reset del Origen de Disparos: posicion y direccion.");
        mEscribe cColor::vBlanco, "S       .Stop Velocidad del Origen de Disparos.");
        mEscribe cColor::vVerde, "A       .Aumenta la velocidad inicial del 'shotBall'");
        mEscribe cColor::vVerde, "D       .Decrementa la velocidad inicial del 'shotBall'");
        mEscribe cColor::vVerde, "+ (Add)       .Sube la direccion del 'shotBall'");
        mEscribe cColor::vVerde, "- (Subtract)  .Baja la direccion del 'shotBall'");
        mEscribe cColor::vBlanco, "Flechas Direccion: Left-Right: Izquierda-Derecha: mueve el Origen de los Disparos.");
        mEscribe cColor::vBlanco, "Flechas Direccion: Up-Down: Arriba-Abajo: mueve el Origen de los Disparos.");
        mEscribe cColor::vBlanco, "Si mantenemos 'pinchado' el Origen de Disparos con el raton, se puede mover");
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
