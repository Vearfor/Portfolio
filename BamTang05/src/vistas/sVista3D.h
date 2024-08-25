/*========================================================================*\
|* sVista3D.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Include
//--------------------------------------------------------------------------
#include "sVista.h"
#include "../tool/iMensajes.h"
#include <cstdint>
#include <string>
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Constantes
//--------------------------------------------------------------------------
//const uint16_t kWidth = 800;                // 1024;
//const uint16_t kHeight = 600;               // 768;
//const uint16_t kCelda = 25;                 //
//--------------------------------------------------------------------------
// const std::string kPathLetraA = "../BamTang03/recursos/letraA_negro.png";
//--------------------------------------------------------------------------
//const std::string kPathLetraA = "../BamTang03/recursos/letraA_negro.png";
//const std::string kPathLetraB = "../BamTang03/recursos/letraB_negro.png";
//const std::string kPathMuro = "../BamTang03/recursos/muro.png";
//const std::string kPathVacio = "../BamTang03/recursos/negro.png";
//const std::string kPathPunto = "../BamTang03/recursos/punto_negro.png";
//const std::string kPathPuntoAmarillo = "../BamTang03/recursos/marca_circulo.png";
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
// Struct
//--------------------------------------------------------------------------
struct sWindow;
//--------------------------------------------------------------------------
struct sVista3D
    : public sVista
    , public iMensajes
{
    sVista3D();
    ~sVista3D();

    virtual int inicia(sLaberinto* lab) override;
    virtual int eventos() override;
    virtual int update() override;
    virtual int render() override;

    int creaWindow(int width, int height);

    sWindow* m_mainWindow{ nullptr };

    //--------------------------------------------------------------------------
    //void dibujaChar(char car, SDL_FRect* pRectDest);
    //void calculaRect(int fila, int columna, int size, SDL_FRect* pOutRect);

    void laVentanaGanaFoco();
    void laVentanaPierdeFoco();
    //----------------------------------------------------------------------

    //// Hay que tener en cuenta que lo estamos haciendo mal, hay que separar de las vistas
    //// los contenidos/datos centrales del Laberinto
    //sRenderObject* m_pPunto{ nullptr };
    //// El render object sera el personaje que esta dentro del laberinto y debería estar fuera de la vista !!!

    //cTextura* m_pLetraA{ nullptr };
    //cTextura* m_pLetraB{ nullptr };
    //cTextura* m_pMuro{ nullptr };
    //cTextura* m_pVacio{ nullptr };
    //cTextura* m_pMarca{ nullptr };
    //----------------------------------------------------------------------
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de sVista3D.h
\*========================================================================*/
