/*========================================================================*\
|* Eventos.cpp
\*========================================================================*/


#include <iostream>
//#include "cCamera.h"
#include "cEventos.h"


//--------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------
cCamera* cEventos::_camera = nullptr;
GLfloat cEventos::_lastX = 100;
GLfloat cEventos::_lastY = 100;
bool cEventos::_keys[1024];
bool cEventos::_firstMouse = true;
bool cEventos::_bLeftButtonMousePressed = false;
bool cEventos::_bRightButtonMousePressed = false;
bool cEventos::_bWireframe = false;
bool cEventos::_bFuturama = false;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cEventos::cEventos(cCamera * camera, int width, int height)
{
    //_camera = camera;
    _lastX = (GLfloat)width / 2.0f;
    _lastY = (GLfloat)height / 2.0f;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto Funciones
//--------------------------------------------------------------------------
// Is called whenever a key is pressed/released via GLFW
void cEventos::KeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        _bWireframe = !_bWireframe;
        if (_bWireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        _bFuturama = !_bFuturama;
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            _keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            _keys[key] = false;
        }
    }
}


//-----------------------------------------------------------------------------
// Is called when the window is resized
//-----------------------------------------------------------------------------
void cEventos::OnSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

}


void cEventos::MouseCallback(GLFWwindow* pWindow, double xPos, double yPos)
{
    if (_firstMouse)
    {
        _lastX = (GLfloat)xPos;
        _lastY = (GLfloat)yPos;
        _firstMouse = false;
    }

    GLfloat xOffset = (GLfloat)xPos - _lastX;
    GLfloat yOffset = _lastY - (GLfloat)yPos;  // Reversed since y-coordinates go from bottom to left

    _lastX = (GLfloat)xPos;
    _lastY = (GLfloat)yPos;

    if (_bLeftButtonMousePressed)
    {
        //_camera->ProcessMouseMovement(xOffset, yOffset);
    }
}


void cEventos::MouseClickCallback(GLFWwindow* pWindow, int button, int action, int mods)
{
    // std::cout << "Button: [" << button << "]   Action: [" << action << "]   Mods: [" << mods << "]" << std::endl;
    _bLeftButtonMousePressed = (button == 0 && action == 1);
    _bRightButtonMousePressed = (button == 1 && action == 1);
}


void cEventos::ScrollCallback(GLFWwindow* pWindow, double xOffset, double yOffset)
{
    //_camera->ProcessMouseScroll((GLfloat)yOffset);
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
void cEventos::setCamera(cCamera* miCamera)
{
    //if (_camera)
    //{
    //    delete _camera;
    //}

    //_camera = miCamera;
}


cCamera* cEventos::getCamera()
{
    return _camera;
}


bool* cEventos::getKeys()
{
    return _keys;
}


bool cEventos::EsFuturama()
{
    return _bFuturama;
}
//--------------------------------------------------------------------------



/*========================================================================*\
|* Fin de Eventos.cpp
\*========================================================================*/

