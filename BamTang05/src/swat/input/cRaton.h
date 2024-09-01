/*========================================================================*\
|* cRaton.h
\*========================================================================*/

#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include "../../tool/nComun.h"
#include <GLM/glm.hpp>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
struct sWindow;
class cRaton
{
    // Posiblemente este valor debiera ser de una clase cMouse/Raton
    static float MOUSE_SENSITIVITY;
    static float MOUSE_SPEED;

    int _screenWidth;
    int _screenHeight;
    glm::vec2 m_currentMousePos;
    glm::vec2 m_lastMousePos;
    glm::vec2 m_deltaPos;

    int	m_iDelta{ 0 };
    int _iDobleClick{ 0 };
    bool _firstMove;
    bool _leftButtonDown;
    bool _rightButtonDown;
    bool _leftButtonUp;
    bool _rightButtonUp;

    bool m_queremos{ true };
    bool m_bAtrapado{ false };
    RECT m_rcOldClip;
    RECT m_rcClip;
    bool m_bVisible{ true };

public:

    cRaton(int width, int height);
    ~cRaton();

    long onMouseEvent(uint message, WPARAM wparam, LPARAM lparam);
    void OnSize(int width, int height);
    void update(double xPos, double yPos);
    glm::vec2 getLastPos();
    glm::vec2 getDelta();


    void setScreen(int width, int height);
    bool LeftButtonPressed();
    bool RighttButtonPressed();
    bool LeftButtonReleased();
    bool RighttButtonReleased();
    bool DobleClick();
    int getDeltaWheel();
    void reset();

    static float getSensitivity();
    static float getSpeed();

    void atrapaCursor(sWindow* pWindow);
    void liberaCursor();

    void setCursorCentro();
    void setCursorPos(int xPos, int yPos);
    void mostrar();
    void ocultar();

    void updateCursor();
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cRaton.h
\*========================================================================*/
