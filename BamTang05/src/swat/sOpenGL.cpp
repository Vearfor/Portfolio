/*------------------------------------------------------------------------*\
|* sOpenGL.cpp
\*------------------------------------------------------------------------*/

#include "sOpenGL.h"
#include "../tool/cLog.h"
#include "../tool/nComun.h"


//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
// bool sOpenGL::m_bGlFinish = false;
//--------------------------------------------------------------------------
bool sOpenGL::m_bIniciado = false;
//--------------------------------------------------------------------------


cstatic void sOpenGL::syncro(eSync eSincr)
{
	switch (eSincr)
	{
		case eSync::Finish:
			glFinish();
			break;

		case eSync::Flush:
			glFlush();
			break;
	}
}


cstatic int sOpenGL::initOpenGL()
{
	if (!m_bIniciado)
	{
		// Initialize GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			cLog::error(" Error: sOpenGL::initOpenGL: glewInit: Fallo la inicializacion de Glew\n");
		}

		// Al menos establecemos el estado inicial de OpenGL
		glEnable(GL_DEPTH_TEST);

		m_bIniciado = true;
	}

	return 0;
}


/*------------------------------------------------------------------------*\
|* Fin de sOpenGL.h
\*------------------------------------------------------------------------*/
