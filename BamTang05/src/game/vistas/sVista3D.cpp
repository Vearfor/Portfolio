/*========================================================================*\
|* sVista3D.cpp
\*========================================================================*/
 
#include "sVista3D.h"
#include "../sGlobal.h"
#include "../laberinto/sMyMaze.h"
#include "../sGameWindow.h"
#include <tool/cLog.h>
#include <tool/consola/cConsola.h>
#include <tool/consola/cConio.h>
#include <swat/input/cTeclado.h>
#include <swat/input/cRaton.h>
#include <swat/buffers/cMalla.h>
#include <swat/texturas/cGestorTexturas.h>
#include <swat/shaders/cGestorShaders.h>
#include <swat/sOpenGL.h>
#include <swat/cCamara.h>
#include <swat/sRenderObject.h>
#include <GLM/glm.hpp>

GLfloat trianguloVertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
};

GLfloat cuadroVertices[] = {
    // front face
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
};

// An array of vertex data for a cube with index buffer data
GLfloat cubeVertices[] = {
    // position		    // tex coords

    // front face
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

    // back face
    -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

    // left face
    -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
    -1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
    -1.0f, -1.0f,  1.0f, 1.0f, 0.0f,

     // right face
      1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
      1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
      1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
      1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
      1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
      1.0f, -1.0f, -1.0f, 1.0f, 0.0f,

    // top face
    -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
     1.0f,  1.0f,  1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
    -1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f,  1.0f, 0.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f, 0.0f,

    // bottom face
    -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
};


//--------------------------------------------------------------------------
// Cosntructor & Destructor
//--------------------------------------------------------------------------
sVista3D::sVista3D(int width, int height)
    : m_width(width)
    , m_height(height)
{
}

sVista3D::~sVista3D()
{
    cGestorShaders::Delete();
    cGestorTexturas::Delete();
    delete m_pTriangulo;
    delete m_pCuadro;
    delete m_pCubo;
    delete m_pCamara;
    // Destruccion de la ventana:
    // - implica destruccion del Render Context, ya no son necesarias funciones OpenGL.
    delete m_mainWindow;
}
//--------------------------------------------------------------------------


#pragma region Inicia la Vista
//--------------------------------------------------------------------------
// Inicia la Vista 3D
//--------------------------------------------------------------------------
int sVista3D::creaWindow()
{
    sGameWindow* pWin = new sGameWindow(m_pLaberinto, m_width, m_height);
    if (!pWin)
    {
        cLog::error(" Error: sRenderSystem:init: error en la creacion de la ventana");
        return -1;
    }

    miError(pWin->crea(
        (int)ePosControl::eCENTER,
        (int)ePosControl::eCENTER,
        m_width, m_height,
        0.01f, 1000.0f, 45.0f, 32, 32, false,
        eEstiloW::eWindow,
        cConsola::getNombreProceso(), "Window OpenGL", nullptr));

    m_mainWindow = pWin;

    return 0;
}


int sVista3D::inicia(sLaberinto* lab)
{
    m_pLaberinto = lab;

    miError(creaWindow())

    // Una vez que la ventana es creada, se utiliza el contexto de renderizado de la ventana
    // para las inicializaciones de openGL
    // Debería ya estar puesto, en el OnCreate, pero LO PONEMOS NOSOTROS PARA QUE SE VEA:
    m_mainWindow->setRenderContext();
    // Si no fuera así las sentencias de OpenGL fallarian: ... 

    miError(sOpenGL::initOpenGL());

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(0.0, 1.0, 1.0, 1.0);

    m_pCubo = new cMalla();
    miError(m_pCubo->createMalla(cubeVertices, sizeof(cubeVertices), nullptr, 0, eVertexType::eVer_PosTex));

    m_pCuadro = new cMalla();
    miError(m_pCuadro->createMalla(cuadroVertices, sizeof(cuadroVertices), nullptr, 0, eVertexType::eVer_PosTex));

    m_pTriangulo = new cMalla();
    miError(m_pTriangulo->createMalla(trianguloVertices, sizeof(trianguloVertices), nullptr, 0, eVertexType::eVer_Pos));

    cGestorShaders* pGestorShaders = cGestorShaders::Instancia();

    cLog::print("\n Carga de Shaders:\n");
    miError(
        (pGestorShaders->CargaShader(kNombreShader.c_str(), kVertexFile.c_str(), nullptr, kFragmentFile.c_str(), kDirShaders.c_str())) ||
        ((m_pMainShader = pGestorShaders->GetShader(kNombreShader.c_str())) == nullptr)
    );

    cGestorTexturas* pGestorTexturas = cGestorTexturas::Instancia();

    cLog::print("\n Carga de Texturas:\n");
    miError(
        pGestorTexturas->CargaTextura(kTexSuelo.c_str(), kDirTexturas.c_str()) ||
        pGestorTexturas->CargaTextura(kTexMuro.c_str(), kDirTexturas.c_str()) ||
        pGestorTexturas->CargaTextura(kTexMuro2.c_str(), kDirTexturas.c_str()) ||
        pGestorTexturas->CargaTextura(kTexLetraA.c_str(), kDirTexturas.c_str()) ||
        pGestorTexturas->CargaTextura(kTexLetraB.c_str(), kDirTexturas.c_str()) ||
        pGestorTexturas->CargaTextura(kTexPunto.c_str(), kDirTexturas.c_str()) ||
        pGestorTexturas->CargaTextura(kTexCamara.c_str(), kDirTexturas.c_str()) ||
        pGestorTexturas->listarTexturas()
    );

    miError(
        ((m_pTexSuelo = pGestorTexturas->GetTextura(kTexSuelo.c_str())) == nullptr) ||
        ((m_pTexMuro = pGestorTexturas->GetTextura(kTexMuro.c_str())) == nullptr) ||
        ((m_pTexMuro2 = pGestorTexturas->GetTextura(kTexMuro2.c_str())) == nullptr) ||
        ((m_pTexLetraA = pGestorTexturas->GetTextura(kTexLetraA.c_str())) == nullptr) ||
        ((m_pTexLetraB = pGestorTexturas->GetTextura(kTexLetraB.c_str())) == nullptr) ||
        ((m_pTexPunto = pGestorTexturas->GetTextura(kTexPunto.c_str())) == nullptr) ||
        ((m_pTexCamara = pGestorTexturas->GetTextura(kTexCamara.c_str())) == nullptr)
    );

    // Las locations de las variables uniform de los shaders:
    cLog::print("\n Las Locations:\n");

    // Si nos guardamos los loc, nos podemos quitar los find de map locations en cada frame.
    m_loc_model = m_pMainShader->getLocation(kModel.c_str());
    m_loc_view = m_pMainShader->getLocation(kView.c_str());
    m_loc_projection = m_pMainShader->getLocation(kProjection.c_str());
    // No lo estamos usando
    m_loc_texSampler1 = m_pMainShader->getLocation(kTexSampler1.c_str());

    // Cube and floor positions
    m_cubePos = glm::vec3(-1.0f, 0.0f, 0.0f);
    m_floorPos = glm::vec3(0.0f, -1.0f, 0.0f);

    //----------------------------------------------------------------------
    // Camara: posiciones iniciales
    //----------------------------------------------------------------------
    // Recuerda: Up esta con (0.0, 1.0, 0.0): la Y (1.0) es la altura
    //----------------------------------------------------------------------
    //m_pCamara = new cCamara(
    //    glm::vec3(0.0f, 50.0f, -15.0f),
    //    0.0f,
    //    -70.0f
    //);
    //----------------------------------------------------------------------
    m_pCamara = new cCamara(
        glm::vec3(0.0f, 50.0f, 20.0f),
        180.0f,
        -70.0f
    );
    //----------------------------------------------------------------------

    sRenderObject* pPunto = m_pLaberinto->getObjectPunto();
    pPunto->m_pTextura = m_pTexPunto;
    // pPunto->m_pTextura = m_pTexCamara;
    pPunto->m_escala = glm::vec3(0.65, 0.65, 0.65);
    pPunto->m_pMalla = m_pCubo;

    int size = m_pLaberinto->getSize();
    float escala = (float)size * 1.5f;

    sRenderObject* pSuelo = m_pLaberinto->getObjectSuelo();
    pSuelo->m_pTextura = m_pTexSuelo;
    pSuelo->m_escala = glm::vec3(escala, 0.01f, escala);
    pSuelo->m_pMalla = m_pCubo;

    sRenderObject* pMuro = m_pLaberinto->getObjectMuro();
    pMuro->m_pTextura = m_pTexMuro;
    pMuro->m_escala = glm::vec3(1.0, 1.0, 1.0);
    pMuro->m_pMalla = m_pCubo;

    sRenderObject* pInicio = m_pLaberinto->getObjectInicio();
    pInicio->m_pTextura = m_pTexLetraA;
    pInicio->m_escala = glm::vec3(0.5, 0.5, 0.5);
    pInicio->m_pMalla = m_pCubo;

    sRenderObject* pFin = m_pLaberinto->getObjectFin();
    pFin->m_pTextura = m_pTexLetraB;
    pFin->m_escala = glm::vec3(0.5, 0.5, 0.5);
    pFin->m_pMalla = m_pCubo;

    sRenderObject* pObjCamara = m_pLaberinto->getObjectCamara();
    pObjCamara->m_pTextura = m_pTexCamara;
    pObjCamara->m_escala = glm::vec3(1.0f, 1.0f, 1.0f);
    pObjCamara->m_pMalla = m_pCuadro;

    return 0;
}
//--------------------------------------------------------------------------
#pragma endregion


#pragma region Eventos de la Vista
//--------------------------------------------------------------------------
// Eventos de la Vista 3D
//--------------------------------------------------------------------------
int sVista3D::eventos()
{
    int iRes = mensajes();

    return !iRes;
}
//--------------------------------------------------------------------------
#pragma endregion


#pragma region Update de la Vista
//--------------------------------------------------------------------------
// Update de la Vista 3D
//--------------------------------------------------------------------------
int sVista3D::update(float fDeltaTime)
{
    sGameWindow* pGameWindow = dynamic_cast<sGameWindow*>(m_mainWindow);
    miNulo(pGameWindow);

    cTeclado* pTeclado = pGameWindow->m_pTeclado;
    if (pTeclado)
    {
        if (pTeclado->isDown(VK_ESCAPE))
        {
            m_mainWindow->destruyeVentana();
        }

        //--------------------------------------------------------------
        // Movimientos dentro del laberinto: teclas AWSD
        //--------------------------------------------------------------
        if (pTeclado->isUp('A'))
        {
            m_pLaberinto->izquierda();
        }
        if (pTeclado->isUp('W'))
        {
            m_pLaberinto->arriba();
        }
        if (pTeclado->isUp('D'))
        {
            m_pLaberinto->derecha();
        }
        if (pTeclado->isUp('S'))
        {
            m_pLaberinto->abajo();
        }
        //--------------------------------------------------------------
        if (pTeclado->isUp(VK_SPACE))
        {
            //bPulsaste = true;
            //contador++;
            //cLog::print(" Contador Laberinto: %3d", contador);
        }
        //--------------------------------------------------------------

        //--------------------------------------------------------------
        // Movimientos de la Camara: teclas de Direccion:
        // - left, up, right, down 
        //--------------------------------------------------------------
        if (pTeclado->isDown(VK_LEFT))
        {
            m_pCamara->izquierda(fDeltaTime);
        }
        if (pTeclado->isDown(VK_UP))
        {
            m_pCamara->adelante(fDeltaTime);
        }
        if (pTeclado->isDown(VK_RIGHT))
        {
            m_pCamara->derecha(fDeltaTime);
        }
        if (pTeclado->isDown(VK_DOWN))
        {
            m_pCamara->atras(fDeltaTime);
        }
        //--------------------------------------------------------------
        if (pTeclado->isDown(VK_HOME))
        {
            m_pCamara->abajo(fDeltaTime);
        }
        if (pTeclado->isDown(VK_END))
        {
            m_pCamara->arriba(fDeltaTime);
        }
        //--------------------------------------------------------------

        //--------------------------------------------------------------
        if (pTeclado->isUp('R'))
        {
            m_pCamara->reset();
        }
        if (pTeclado->isUp('Q'))
        {
            sOpenGL::toggle_Line_Fill();
        }
        //--------------------------------------------------------------

        //--------------------------------------------------------------
        // Keys for Demo
        //--------------------------------------------------------------
        if (pTeclado->isUp('P'))
        {
            m_pLaberinto->togglePausa();
        }
        //--------------------------------------------------------------
        if (pTeclado->isUp('T'))
        {
            m_pLaberinto->stopDemo(pGameWindow);
        }
        //--------------------------------------------------------------

        // Hay que hacer reset para el siguiente control de eventos
        pTeclado->reset();
    }

    cRaton* pRaton = pGameWindow->m_pRaton;
    if (pRaton)
    {
        if (pRaton->LeftButtonPressed())
        {
            m_pCamara->direccion(pRaton->getDelta(), fDeltaTime);
        }

        if (pRaton->getDeltaWheel() > 0)
        {
            m_pCamara->mouseScroll((float) pRaton->getDeltaWheel(), cRaton::getSensitivity());
        }

        // Hacemos reset para el siguiente frame de eventos
        pRaton->reset();
    }

    m_pCamara->update(fDeltaTime);

    controlFin();

    return 0;
}
//--------------------------------------------------------------------------
#pragma endregion


#pragma region Render de la Vista
//--------------------------------------------------------------------------
// Render de la Vista 3D
//--------------------------------------------------------------------------
int sVista3D::render()
{
    m_mainWindow->clean();

    {
        // Shader:
        //------------------------------------------------------------------
        m_pMainShader->use();


        // Set View
        m_view = m_pCamara->getViewMatrix();
        m_pMainShader->SetUniform(m_loc_view, m_view);

        // Set projection perspective
        m_perspProjection = m_mainWindow->getPerspProjection();
        m_pMainShader->SetUniform(m_loc_projection, m_perspProjection);

        //------------------------------------------------------------------
        // Suelo
        //------------------------------------------------------------------
        m_pLaberinto->getObjectSuelo()->render(m_pMainShader, m_loc_model, m_floorPos);
        //------------------------------------------------------------------

        //------------------------------------------------------------------
        // Laberinto
        //------------------------------------------------------------------
        renderLaberinto();
        //------------------------------------------------------------------

        //------------------------------------------------------------------
        // Object Camara
        //------------------------------------------------------------------
        // Set View
        m_view = glm::mat4(1.0f);
        m_pMainShader->SetUniform(m_loc_view, m_view);

        // Set projection ortho
        m_orthoProjection = m_mainWindow->getOrthoProjection();
        m_pMainShader->SetUniform(m_loc_projection, m_orthoProjection);

        m_pLaberinto->getObjectCamara()->render(m_pMainShader, m_loc_model, m_floorPos);
        //------------------------------------------------------------------
    }

    m_mainWindow->swap();

    return 0;
}


int sVista3D::renderLaberinto()
{
    int size = m_pLaberinto->getSize();
    cRect<float> rectDest;

    char** matriz = m_pLaberinto->getMatriz();
    char valor = 0;
    for (int fila = -1; fila < size + 1; fila++)
    {
        for (int columna = -1; columna < size + 1; columna++)
        {
            if (fila<0 || fila>size - 1 || columna<0 || columna>size - 1)
            {
                valor = kVacio;
            }
            else
            {
                int iValor = matriz[fila][columna];
                valor = static_cast<char>(iValor);

                calculaRect(fila, columna, size, &rectDest);
                renderChar(valor, &rectDest);
            }
        }
    }

    sRenderObject* pPunto = m_pLaberinto->getObjectPunto();
    calculaRect(pPunto->m_fila, pPunto->m_columna, size, &rectDest);
    pPunto->render(m_pMainShader, m_loc_model, &rectDest);

    return 0;
}


void sVista3D::calculaRect(int fila, int columna, int size, cRect<float>* pOutRect)
{
    float diffx = - (float(size * kCelda) / 2.0f);
    float diffy = - (float(size * kCelda) / 2.0f);
    pOutRect->left = diffx + columna * kCelda + 1;
    pOutRect->top = diffy + fila * kCelda + 1;
    float ancho, alto;
    ancho = alto = static_cast<float>(kCelda);
    pOutRect->right = pOutRect->left + ancho;
    pOutRect->bottom = pOutRect->top + alto;
}


void sVista3D::renderChar(char car, cRect<float>* pRectDest)
{
    sRenderObject* pRender = { nullptr };
    switch (car)
    {
        case kNulo:
        case kVacio:
            break;

        case kMuro:
            pRender = m_pLaberinto->getObjectMuro();
            break;

        case kInicio:
            pRender = m_pLaberinto->getObjectInicio();
            break;

        case kFin:
            pRender = m_pLaberinto->getObjectFin();
            break;
    }
    
    mDo(pRender)->render(m_pMainShader, m_loc_model, pRectDest);
}
//--------------------------------------------------------------------------
#pragma endregion


//--------------------------------------------------------------------------
// Ayuda Vista 3D
//--------------------------------------------------------------------------
int sVista3D::ayudaVista3D()
{
    cConio::SetColor(eTextColor::eTexVerde);
    cLog::print("\n");
    cLog::print(" Que la Ventana 3D tenga el foco\n");
    cLog::print("\n");
    cLog::print(" Para salir pulsa Esc\n");
    cLog::print(" Utiliza las teclas: A, W, D y S, para moverte por el laberinto.\n");
    cLog::print("\n");
    cLog::print(" Con las teclas de direccion, moveras la camara:\n");
    cLog::print("  - Up  : hacia adelante   (vector look)\n");
    cLog::print("  - Down: hacia atras      (vector look)\n");
    cLog::print("  - Izda: izquierda        (vector side)\n");
    cLog::print("  - Dcha: derecha          (vector side)\n");
    cLog::print("\n");
    cLog::print("  - inicio/home:  bajas    (vector up)\n");
    cLog::print("  - Fin/End    :  subes    (vector up)\n");
    cLog::print("\n");
    cLog::print("  - Click Izquierdo Raton pulsado: cambias direccion camara\n");
    cLog::print("\n");
    cLog::print("  - R:  reset, posicion inicial. Por si pierdes la referencia\n");
    cLog::print("\n");
    cLog::print("  Modo Demo:\n");
    cLog::print("  - P:  Activamos/Desactivamos pausa\n");
    cLog::print("  - T:  Stop a la Demo\n");
    cLog::print("\n");
    return 0;
}
//--------------------------------------------------------------------------


void sVista3D::controlFin()
{
    if (m_pLaberinto->estaEnElFin() && !m_hemosLlegado)
    {
        m_pLaberinto->limpiaMarcas();
        m_pLaberinto->setPlayingDemo(false);
        m_hemosLlegado = true;
        sGameWindow* pWin = dynamic_cast<sGameWindow*>(m_mainWindow);
        mDo(pWin)->OnSetFocus(m_pLaberinto);
        cConio::SetColor(eTextColor::eTexBlanco);
        cLog::print("\n");
        cLog::print("     Hemos llegado al Final\n");
        cLog::print("\n");
        cConio::SetColor(eTextColor::eTexNormal);
    }
}


/*========================================================================*\
|* Fin de sVista3D.cpp
\*========================================================================*/
