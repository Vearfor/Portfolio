/*========================================================================*\
|* sRenderSystem.cpp
\*========================================================================*/


#include "sRenderSystem.h"
#include "vistas/sVistaConsola.h"
#include "vistas/sVista3D.h"
#include <swat/windows/sWindow.h>
#include <tool/nComun.h>
#include <tool/consola/cConsola.h>


//--------------------------------------------------------------------------
sRenderSystem::sRenderSystem()
{
}

sRenderSystem::~sRenderSystem()
{
    delete m_pVista3D;
    delete m_pVistaConsola;
}
//--------------------------------------------------------------------------


int sRenderSystem::init(sLaberinto* pLab, int width, int height)
{
    m_pVistaConsola = new sVistaConsola();
    miError(
        m_pVistaConsola->inicia(pLab)
    );

    m_pVista3D = new sVista3D(width, height);
    miError(
        m_pVista3D->inicia(pLab)
    );

    // La primera vista que utilizamos:
    m_pVista = m_pVistaConsola;

    cLog::print("\n");
    cConsola::PulsaTecla(" Pulsa tecla para continuar ");

    return 0;
}


int sRenderSystem::eventos()
{
    return m_pVista->eventos();
}


int sRenderSystem::update(float fDeltaTime)
{
    int iRes = m_pVista->update(fDeltaTime);
    if (iRes)
    {
        // Cambiamos a la vista 3D:
        m_pVista = m_pVista3D;
        m_pVista3D->m_mainWindow->muestraVentana();
        m_pVista3D->ayudaVista3D();
    }

    return 0;
}


int sRenderSystem::render()
{
    return m_pVista->render();
}


/*========================================================================*\
|* Fin de sRenderSystem.cpp
\*========================================================================*/
