/*========================================================================*\
|* sRenderSystem.cpp
\*========================================================================*/


#include "sRenderSystem.h"
#include "../vistas/sVistaConsola.h"
#include "../vistas/sVista3D.h"
#include "../swat/sWindow.h"
#include "../tool/nComun.h"


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
    miError(m_pVistaConsola->inicia(pLab));

    m_pVista3D = new sVista3D();
    miError(
        m_pVista3D->inicia(pLab) ||
        m_pVista3D->creaWindow(width, height)
    );

    m_pVista = m_pVistaConsola;
    return 0;
}


int sRenderSystem::eventos()
{
    return m_pVista->eventos();
}


int sRenderSystem::update()
{
    int iRes = m_pVista->update();
    if (iRes)
    {
        // Cambiamos la vista:
        m_pVista = m_pVista3D;
        m_pVista3D->m_mainWindow->muestraVentana();
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
