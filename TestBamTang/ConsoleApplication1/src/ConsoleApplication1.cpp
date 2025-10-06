// ConsoleApplication1.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "sConsoleWindow.h"
#include <tool/nComun.h>
#include <tool/consola/cConsola.h>
#include <tool/cTime.h>
#include <swat/input/cTeclado.h>
#include <swat/sOpenGL.h>
#include <swat/buffers/cMalla.h>
#include <swat/shaders/cGestorShaders.h>
#include <swat/texturas/cGestorTexturas.h>

const int kWidth = 800;
const int kHeight = 600;
const std::string kNombreShader = "MainShader";
const std::string kVertexFile = "vertex_triangulo.glsl";
const std::string kFragmentFile = "fragment_triangulo.glsl";
const std::string kDirShaders = "../comun/shaders";
const std::string kTextura = "aries01.jpg";
const std::string kDirTexturas = "../comun/textures";

sConsoleWindow* g_pMainWindow{ nullptr };
cMalla* g_pTriangulo{ nullptr };
cShader* g_pMainShader{ nullptr };
cTextura* g_pTextura{ nullptr };

int init();
int update(float fDeltaTime);
int render();

int main()
{
    cConsola::setNombreProceso("Shader Triangulo   " TITLE "   Para Terminar pulsa Esc");
    std::cout << "Hello World!\n";

    g_pMainWindow = new sConsoleWindow();
    miError(g_pMainWindow->crea(
        (int)ePosControl::eCENTER,
        (int)ePosControl::eCENTER,
        kWidth, kHeight,
        0.1f, 100.0f, 45.0f, 32, 32, false,
        eEstiloW::eWindow,
        cConsola::getNombreProceso(), "Window OpenGL", nullptr)
    );

    g_pMainWindow->muestraVentana();

    cTime time(60);
    for (bool salir = init(); !salir; salir = g_pMainWindow->mensajes())
    {
        update(time.getDeltaTime());
        render();

        time.espera();
    }

    cGestorTexturas::Delete();
    cGestorShaders::Delete();
    delete g_pTriangulo;
    delete g_pMainWindow;
    cMonitor::release();

    return 0;
}


GLfloat trianguloVertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f,
};

int init()
{
    // Una vez que la ventana es creada, se utiliza el contexto de renderizado de la ventana
    g_pMainWindow->setRenderContext();

    // Si no fuera así las sentencias de OpenGL fallarian: ... 
    miError(sOpenGL::initOpenGL());

    glClearColor(0.0, 1.0, 1.0, 1.0);

    miError(
        (g_pTriangulo = new cMalla()) == nullptr ||
        g_pTriangulo->createMalla(trianguloVertices, sizeof(trianguloVertices), nullptr, 0, eVertexType::eVer_Pos)
    );

    miError(
        (cGestorShaders::Instancia()->CargaShader(kNombreShader.c_str(), kVertexFile.c_str(), nullptr, kFragmentFile.c_str(), kDirShaders.c_str())) ||
        ((g_pMainShader = cGestorShaders::Instancia()->GetShader(kNombreShader.c_str())) == nullptr)
    );

    miError(
        (cGestorTexturas::Instancia()->CargaTextura(kTextura.c_str(), kDirTexturas.c_str())) ||
        ((g_pTextura = cGestorTexturas::Instancia()->GetTextura(kTextura.c_str())) == nullptr)
    );

    return 0;
}


int update(float fDeltaTime)
{
    if (g_pMainWindow)
    {
        cTeclado* pTeclado = g_pMainWindow->m_pTeclado;
        if (pTeclado)
        {
            if (pTeclado->isDown(VK_ESCAPE))
            {
                g_pMainWindow->destruyeVentana();
            }
        }
    }
    return 0;
}


int render()
{
    g_pMainWindow->clean();
    {
        // Uso moderno con shader
        g_pMainShader->use();
        g_pTriangulo->drawMalla();
        cShader::use(0);

        // Uso clasico con textura en primer plano:
        g_pMainWindow->begin();
        {
            sOpenGL::textura(
                kWidth / 8.0f,
                (kHeight / 8.0f) * 3.0f, 0.0f, 
                kWidth / 4.0f, 
                kHeight / 4.0f,
                g_pTextura);
        }
        g_pMainWindow->end();
    }
    g_pMainWindow->swap();
    return 0;
}


// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
