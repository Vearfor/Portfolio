/*------------------------------------------------------------------------*\
|* sGame.h
\*------------------------------------------------------------------------*/


#pragma once
/*------------------------------------------------------------------------*\
|* Includes
\*------------------------------------------------------------------------*/
#include <tool/iMensajes.h>
#include <vector>
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* struct sGame
\*------------------------------------------------------------------------*/
struct sGameWindow;
struct sBall;
struct sOrigen;
struct sPhysicsSystem;
struct sCollisionSystem;
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
    void createOrigin();
    void shootBall();
    void releaseBalls();
    void setCaption(float fDeltaTime);

    std::vector<sBall*>& getVecBolas() { return m_vecBolas; }
    void togglePausa() { m_esPausa = !m_esPausa; }
    bool hayPausa() { return m_esPausa; }
    void setDestruccion(bool hay) { m_hayDestruccion = hay; }
    bool hayDestruccion() { return m_hayDestruccion; }

private:

    sPhysicsSystem* m_pPhysics{ nullptr };
    sCollisionSystem* m_pCollision{ nullptr };
    bool m_bIsRunning{ false };
    sGameWindow* m_pWindow{ nullptr };
    int m_width{ 0 };
    int m_height{ 0 };
    // Origen de los disparos con textura rotable que indique la direccion de los disparos.
    sOrigen* m_pOrigen{ nullptr };
    std::vector<sBall*> m_vecBolas{};
    bool m_esPausa{ false };
    bool m_esTest{ false };
    bool m_hayDestruccion{ false };

    static sGame* m_instancia;
};
/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*\
|* Fin de sGame.h
\*------------------------------------------------------------------------*/
