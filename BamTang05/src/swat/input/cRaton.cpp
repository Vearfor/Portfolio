/*========================================================================*\
|* cRaton.cpp
\*========================================================================*/


#include "cRaton.h"


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
float cRaton::MOUSE_SENSITIVITY = 0.1f;
float cRaton::MOUSE_SPEED = 5.0f;
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cRaton::cRaton(int width, int height)
    : _screenWidth(width)
    , _screenHeight(height)
    , _currentMousePos(0.0f, 0.0f)
    , _lastMousePos(0.0f, 0.0f)
    , _delta(0.0f, 0.0f)
    , _firstMove(true)
    , _leftButton(false)
    , _rightButton(false)
{
}

cRaton::~cRaton()
{
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
glm::vec2 cRaton::getLastPos()
{
    return _lastMousePos;
}


glm::vec2 cRaton::getDelta()
{
    return _delta;
}


void cRaton::setScreen(int width, int height)
{
    _screenWidth = width;
    _screenHeight = height;
}


void cRaton::setLeftButton(bool estado)
{
    _leftButton = estado;
}


void cRaton::setRighttButton(bool estado)
{
    _rightButton = estado;
}


void cRaton::update(double xPos, double yPos)
{
    if (_firstMove)
    {
        _lastMousePos = glm::vec2((float)xPos, (float)yPos);
        _firstMove = false;
    }
    _currentMousePos = glm::vec2((float)xPos, (float)yPos);
    _delta = _currentMousePos - _lastMousePos;
    _lastMousePos = _currentMousePos;
}


bool cRaton::LeftButton()
{
    return _leftButton;
}


bool cRaton::RighttButton()
{
    return _rightButton;
}


float cRaton::getSensitibity()
{
    return MOUSE_SENSITIVITY;
}


float cRaton::getSpeed()
{
    return MOUSE_SPEED;
}
//--------------------------------------------------------------------------


/*========================================================================*\
|* cRaton.h
\*========================================================================*/
