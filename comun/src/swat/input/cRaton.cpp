/*========================================================================*\
|* cRaton.cpp
\*========================================================================*/


#include "cRaton.h"
#include "../windows/sWindow.h"
#include <tool/cLog.h>
#include <windowsx.h>


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
//float cRaton::MOUSE_SENSITIVITY = 0.1f;     // m_incAngle
//float cRaton::MOUSE_SPEED = 5.0f;           // m_offsetPos
//--------------------------------------------------------------------------
float cRaton::MOUSE_SENSITIVITY = 10.0f;    // m_incAngle
float cRaton::MOUSE_SPEED = 50.0f;          // m_offsetPos
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cRaton::cRaton(int width, int height)
    : _screenWidth(width)
    , _screenHeight(height)
    , m_currentMousePos(0.0f, 0.0f)
    , m_lastMousePos(0.0f, 0.0f)
    , m_deltaPos(0.0f, 0.0f)
    , m_iDelta(0)
    , _iDobleClick(0)
    , _firstMove(true)
    , _leftButtonDown(false)
    , _rightButtonDown(false)
    , _leftButtonUp(false)
    , _rightButtonUp(false)
    //bool m_queremos{ true };
    //bool m_bAtrapado{ false };
    //RECT m_rcOldClip;
    //RECT m_rcClip;
    //bool m_bVisible{ true };
{
}

cRaton::~cRaton()
{
    liberaCursor();
    cLog::print(" Destructor de cRaton\n");
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
long cRaton::onMouseEvent(uint message, WPARAM wparam, LPARAM lparam)
{
    // Por lo que se ve recomiendan no utilizar LOWORD
    // y HIWORD, porque devuelven valores incorrectos en sistemas de
    // monitores multiples. Estos sistemas admiten valores negativos
    // y LOWORD y HIWORD tratan los valores como unsigned.
    short xPos = GET_X_LPARAM(lparam);     // LOWORD
    short yPos = GET_Y_LPARAM(lparam);     // HIWORD

    switch (message)
    {
        case WM_SIZE:
            OnSize(LOWORD(lparam), HIWORD(lparam));
            break;

        case WM_MOUSEMOVE:
            update(xPos, yPos);
            break;

        case WM_LBUTTONDOWN:
            update(xPos, yPos);
            _leftButtonDown = true;
            _iDobleClick++;
            break;

        case WM_LBUTTONUP:
            update(xPos, yPos);
            _leftButtonDown = false;
            _leftButtonUp = true;
            break;

        case WM_LBUTTONDBLCLK:
            _iDobleClick++;
            break;

        case WM_RBUTTONDOWN:
            update(xPos, yPos);
            _rightButtonDown = true;
            _iDobleClick++;
            break;

        case WM_RBUTTONUP:
            update(xPos, yPos);
            _rightButtonDown = false;
            _rightButtonUp = true;
            break;

        case WM_RBUTTONDBLCLK:
            _iDobleClick++;
            break;

        case WM_MOUSEWHEEL:
            {
                // No se si poner el boton central a true, es lo correcto
                // Quien lo pone a false
                // Estaba con 0, en realidad no lo hacia
                //    short fwKeys = LOWORD(wParam);         // Es lo mismo que: GET_KEYSTATE_WPARAM(wParam);, pero no lo estamos usando.
                //    short zDelta = (short)HIWORD(wParam);  // Es lo mismo que: GET_WHEEL_DELTA_WPARAM(wParam);
                short zDelta = (short)HIWORD(wparam);  // Es lo mismo que: GET_WHEEL_DELTA_WPARAM(wParam);
                m_iDelta = zDelta;
            }
            break;

        default:
            break;
    }
    return 0;
}


void cRaton::OnSize(int width, int height)
{
    // Por lo que le toca al raton: saber las nuevas dimensiones de la ventana:
    setScreen(width, height);
}


//--------------------------------------------------------------------------
// Intentamos recoger todos los eventos del raton
//--------------------------------------------------------------------------
glm::vec2 cRaton::getLastPos()
{
    return m_lastMousePos;
}


glm::vec2 cRaton::getDelta()
{
    return m_deltaPos;
}


void cRaton::setScreen(int width, int height)
{
    _screenWidth = width;
    _screenHeight = height;
}


void cRaton::update(double xPos, double yPos)
{
    if (_firstMove)
    {
        m_lastMousePos = glm::vec2((float)xPos, (float)yPos);
        _firstMove = false;
    }
    m_currentMousePos = glm::vec2((float)xPos, (float)yPos);
    m_deltaPos = m_currentMousePos - m_lastMousePos;
    m_lastMousePos = m_currentMousePos;

    //if (m_deltaPos.x > 0 || m_deltaPos.y > 0)
    //{
    //    printf("  deltaX: %3d", (int)m_deltaPos.x);
    //    printf("  deltaY: %3d", (int)m_deltaPos.y);
    //    printf("\n");
    //}
}


bool cRaton::LeftButtonPressed()
{
    return _leftButtonDown;
}


bool cRaton::RighttButtonPressed()
{
    return _rightButtonDown;
}


bool cRaton::LeftButtonReleased()
{
    return _leftButtonUp;
}


bool cRaton::RighttButtonReleased()
{
    return _rightButtonUp;
}


bool cRaton::DobleClick()
{
    // Probando si alguna vez funciona:
    return (_iDobleClick > 1);
}


int cRaton::getDeltaWheel()
{
    return m_iDelta;
}


//--------------------------------------------------------------------------
// Reset de los eventos registrados para un frame, se reinician para el
// siguiente frame
//--------------------------------------------------------------------------
void cRaton::reset()
{
    if (_leftButtonUp)
        _leftButtonDown = false;

    if (_rightButtonUp)
        _rightButtonDown = false;

    _leftButtonUp = false;
    _rightButtonUp = false;

    _iDobleClick = 0;
    m_iDelta = 0;
}


//--------------------------------------------------------------------------
cstatic float cRaton::getSensitivity()
{
    return MOUSE_SENSITIVITY;
}


cstatic float cRaton::getSpeed()
{
    return MOUSE_SPEED;
}
//--------------------------------------------------------------------------


void cRaton::updateCursor()
{
    CURSORINFO sCursorInfo;
    sCursorInfo.cbSize = sizeof(CURSORINFO);
    if (!GetCursorInfo(&sCursorInfo))
    {
    }
}


void cRaton::atrapaCursor(sWindow* pWindow)
{
    if (m_queremos)
    {
        if (!m_bAtrapado)
        {
            GetClipCursor(&m_rcOldClip);
            GetWindowRect(pWindow->getWindow(), &m_rcClip);
            // Confine the cursor to the application's window
            ClipCursor(&m_rcClip);
            m_bAtrapado = true;
        }
    }
}


void cRaton::liberaCursor()
{
    if (m_queremos)
    {
        if (m_bAtrapado)
        {
            ClipCursor(&m_rcOldClip);
            m_bAtrapado = false;
        }
    }
}


void cRaton::setCursorCentro()
{
    setCursorPos(_screenWidth / 2, _screenHeight / 2);
}


void cRaton::setCursorPos(int xPos, int yPos)
{
    SetCursorPos(xPos, yPos);
}


void cRaton::mostrar()
{
    if (!m_bVisible)
    {
        int val = ShowCursor(TRUE);
        m_bVisible = (val > -1);
    }
}


void cRaton::ocultar()
{
    if (m_bVisible)
    {
        int val = ShowCursor(FALSE);
        m_bVisible = (val > -1);
    }
}


/*========================================================================*\
|* cRaton.h
\*========================================================================*/
