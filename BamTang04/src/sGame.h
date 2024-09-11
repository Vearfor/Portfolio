/*------------------------------------------------------------------------*\
|* sGame.h
\*------------------------------------------------------------------------*/

#pragma once
/*------------------------------------------------------------------------*\
|* Includes
\*------------------------------------------------------------------------*/
#include <GLM/glm.hpp>
#include <tool/iMensajes.h>
#include <vector>
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Constantes
\*------------------------------------------------------------------------*/
const std::string kDirTexturas = "../comun/textures";
const std::string kTextura = "aries01.jpg";
const std::string kFuenteArial = "Arial";
const std::string kFuenteDef = "def.tga";
const std::string kFuenteAgulon = "agulon.tga";
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Identificadores via Enumerados
\*------------------------------------------------------------------------*/
enum class eFuentes : int
{
    ArialMin = 0,
    ArialMax,
    Def,
    Agulon
};
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* struct sGame
\*------------------------------------------------------------------------*/
struct sGameWindow;
struct sBall;
struct sOrigen;
struct sPhysicsSystem;
struct sCollisionSystem;
struct sInputSystem;
struct sRenderSystem;
class cRandom;
class cTextura;
class cFuente;
/*------------------------------------------------------------------------*/
struct sGame
    : public iMensajes
{
    sGame();
    ~sGame();

    int init();
    int eventos();
    int update(float fDeltaTime);
    int render();
    bool isRunning();

    static sGame* instancia();

    int createFuentes(sGameWindow * pWindow);
    int createOrigin();
    void shootBall();
    void releaseBalls();
    void setCaption(float fDeltaTime);
    bool select(glm::vec2 posRaton);
    sBall* selectBall(glm::vec2 posRaton);
    void destruirSeleccionado(sBall* pBall);

    std::vector<sBall*>& getVecBolas() { return m_vecBolas; }

    void toggleHelp() { m_esHelp = !m_esHelp; }
    bool hayHelp() { return m_esHelp; }

    void toggleTest() { m_esTest = !m_esTest; }
    bool hayTest() { return m_esTest; }

    void togglePausa() { m_esPausa = !m_esPausa; }
    bool hayPausa() { return m_esPausa; }

    void setDestruccion(bool hay) { m_hayDestruccion = hay; }
    bool hayDestruccion() { return m_hayDestruccion; }
    
    sOrigen* getMainOrigin() { return m_pOrigen; }
    sRenderSystem* getRender() { return m_pRender; }
    cRandom* getMotor() { return m_pMotor; }
    cTextura* getTextura() { return m_pTextura; }
    cFuente* getFonArialMin() { return m_pFonArialMin; }
    cFuente* getFonArialMax() { return m_pFonArialMax; }
    cFuente* getFonDef() { return m_pFonDef; }
    cFuente* getFonAgulon() { return m_pFonAgulon; }
    sBall* getSelected() { return m_pSelected; }

private:

    cRandom* m_pMotor{ nullptr };
    sRenderSystem* m_pRender{ nullptr };
    sInputSystem* m_pInput{ nullptr };
    sCollisionSystem* m_pCollision{ nullptr };
    sPhysicsSystem* m_pPhysics{ nullptr };
    bool m_bIsRunning{ false };
    bool m_esPausa{ false };
    bool m_esTest{ false };
    bool m_esHelp{ false };
    bool m_hayDestruccion{ false };
    int m_width{ 0 };
    int m_height{ 0 };

    // Origen de los disparos
    sOrigen* m_pOrigen{ nullptr };
    sBall* m_pSelected{ nullptr };
    std::vector<sBall*> m_vecBolas{};
    cTextura* m_pTextura{ nullptr };
    cFuente* m_pFonArialMax{ nullptr };
    cFuente* m_pFonArialMin{ nullptr };
    cFuente* m_pFonDef{ nullptr };
    cFuente* m_pFonAgulon{ nullptr };

    static sGame* m_instancia;
};
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sGame.h
\*------------------------------------------------------------------------*/
