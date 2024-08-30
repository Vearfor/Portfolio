/*========================================================================*\
|* cRaton.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <GLM/glm.hpp>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cRaton
{

    // Posiblemente este valor debiera ser de una clase cMouse/Raton
    static float MOUSE_SENSITIVITY;
    static float MOUSE_SPEED;

    int _screenWidth;
    int _screenHeight;
    glm::vec2 _currentMousePos;
    glm::vec2 _lastMousePos;
    glm::vec2 _delta;

    bool _firstMove;
    bool _leftButton;
    bool _rightButton;

public:

    cRaton(int width, int height);
    ~cRaton();

    void update(double xPos, double yPos);
    glm::vec2 getLastPos();
    glm::vec2 getDelta();

    void setScreen(int width, int height);
    void setLeftButton(bool estado);
    void setRighttButton(bool estado);
    bool LeftButton();
    bool RighttButton();

    static float getSensitibity();
    static float getSpeed();
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cRaton.h
\*========================================================================*/
