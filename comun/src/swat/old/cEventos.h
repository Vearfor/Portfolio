/*========================================================================*\
|* cEventos.h
\*========================================================================*/


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <GLFW/glfw3.h>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cCamera;
class cLesson;
//--------------------------------------------------------------------------
class cEventos
{
    // Camera
    static cLesson* _lesson;
    static cCamera* _camera;
    static GLfloat _lastX;
    static GLfloat _lastY;
    static bool _keys[1024];
    static bool _firstMouse;
    static bool _bLeftButtonMousePressed;
    static bool _bRightButtonMousePressed;
    static bool _bWireframe;
    static bool _bFuturama;

public:

    cEventos(cCamera *camera, int width, int height);

    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void OnSizeCallback(GLFWwindow* window, int width, int height);
    static void MouseCallback(GLFWwindow* window, double xPos, double yPos);
    static void MouseClickCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);

    void setCamera(cCamera*);
    cCamera* getCamera();
    bool* getKeys();

    bool EsFuturama();
};
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Prototipos
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cEventos.h
\*========================================================================*/

