//--------------------------------------------------------------------------
// cGestorFuentes.cpp
//--------------------------------------------------------------------------


#include "cGestorFuentes.h"
#include "../cColor.h"
#include "../windows/sWindow.h"
#include <tool/nComun.h>
#include <tool/cTool.h>
#include <tool/strings/sString.h>


//--------------------------------------------------------------------------
// Variables
//--------------------------------------------------------------------------
sItem vInfCharSet [] =
{
	{	DEFAULT_CHARSET		, "DEFAULT_CHARSET"		, NULL	},
	{	ANSI_CHARSET		, "ANSI_CHARSET"		, NULL	},
	{	JOHAB_CHARSET		, "JOHAB_CHARSET"		, NULL	},
	{	HEBREW_CHARSET		, "HEBREW_CHARSET"		, NULL	},
	{	ARABIC_CHARSET		, "ARABIC_CHARSET"		, NULL	},
	{	GREEK_CHARSET		, "GREEK_CHARSET"		, NULL	},
	{	TURKISH_CHARSET		, "TURKISH_CHARSET"		, NULL	},
	{	VIETNAMESE_CHARSET	, "VIETNAMESE_CHARSET"	, NULL	},
	{	THAI_CHARSET		, "THAI_CHARSET"		, NULL	},
	{	EASTEUROPE_CHARSET	, "EASTEUROPE_CHARSET"	, NULL	},
	{	RUSSIAN_CHARSET		, "RUSSIAN_CHARSET"		, NULL	},
	{	MAC_CHARSET			, "MAC_CHARSET"			, NULL	},
	{	BALTIC_CHARSET		, "BALTIC_CHARSET"		, NULL	},
	{	-1					, "CharSet Desconocido"	, NULL	},
};
//--------------------------------------------------------------------------
sItem vInfFontType [] =
{
	{	DEVICE_FONTTYPE		, "DEVICE_FONTTYPE"		, NULL	},
	{	RASTER_FONTTYPE		, "RASTER_FONTTYPE"		, NULL	},
	{	TRUETYPE_FONTTYPE	, "TRUETYPE_FONTTYPE"	, NULL	},
};
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Statics
//--------------------------------------------------------------------------
int cGestorFuentes::iFuentes = 0;
int cGestorFuentes::iInicio = 0;	// Para visualizar Fuentes: PruebaFuentes
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Busca la cadena que identifica el CharSet
//--------------------------------------------------------------------------
cstatic char* cGestorFuentes::BuscaCharSet(int iCharSet)
{
	int i;
	bool bFound;
	int iLong;
	char* pcFound = NULL;

	iLong = sizeof(vInfCharSet) / sizeof(sItem);
	for (bFound = false, i = 0; i < iLong && !bFound; i++)
	{
		pcFound = vInfCharSet[i].vcNombre;
		if (vInfCharSet[i].iIndex == iCharSet)
		{
			bFound = true;
		}
	}

	return pcFound;
}


//--------------------------------------------------------------------------
// Busca la cadena que identifica el Tipo de fuente, devuelve la combinacion
// final del tipo dado.
//--------------------------------------------------------------------------
cstatic bool cGestorFuentes::BuscaTipoFuente(int iTipoFuente, std::string& oFound)
{
	int i;
	bool bFound;
	int iLong;

	iLong = sizeof(vInfFontType) / sizeof(sItem);
	for (bFound = false, oFound = "", i = 0; i < iLong; i++)
	{
		if (vInfFontType[i].iIndex & iTipoFuente)
		{
			if (bFound)
			{
				oFound += "";
			}
			oFound += vInfFontType[i].vcNombre;
			bFound = true;
		}
	}
	return bFound;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------------
cGestorFuentes::cGestorFuentes()
{
}

cGestorFuentes::~cGestorFuentes()
{
    mSingletonNull;
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// Instancia: Singleton.
//--------------------------------------------------------------------------
mSingleton(cGestorFuentes);
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Carga individual de una unica fuente.
// Se carga con un identificador dado, si este ya existe con ese
// identificador no se carga, porque se da por cargada
// 
// No se considera error si ya estuviera cargada.
// 
//--------------------------------------------------------------------------
int cGestorFuentes::cargaFuente(
	int iIdent,					// Necesario para identificarlo en la lista
								// del Gestor junto con el nombre.
	const char* pcNombre,
	int iAncho,
	int iAlto,
	int iGrupo,
	eTipoFuentes tipoFuente,
	eOldTipoTextura tipoTextura,
	int peso,
	bool bItalic,
	bool bUnderLine,
	bool bStrikeOut,
	bool sombra,
	//---------------------------
	bool bOutLine,
	const char* pcDirFuentes,
	int iCharSet,
	bool full,
	float fEsc,
	float fOffset,
	int iIncSombra)
{
	int iRes = 0;

	// Primero se comprueba si ya lo tenemos:
	cFuente* poFuente = getFuente(iIdent);
	if (poFuente)
	{
		cLog::print(" - Fuente: %2d  [%s]  Ya cargada.\n", iIdent, pcNombre);
		return iRes;
	}

	// Si lo tenemos lo introducimos
	sFuente tFuente;
	
	tFuente.reset();
	
	tFuente.idFuente = iIdent;
	
	mCopia(tFuente.vcNombre, pcNombre);
	
	if (pcDirFuentes)
	{
		sprintf_s(tFuente.vcFichero, sizeof(tFuente.vcFichero), "%s/%s", pcDirFuentes, tFuente.vcNombre);
	}
	else
	{
		mCopia(tFuente.vcFichero, pcNombre);
	}

	tFuente.iAlto = iAlto;				// iAlto/Tamaño base
	tFuente.iAncho = iAncho;			// iAncho/Espacio base
	tFuente.iGrupo = iGrupo;            // Grupo: 0 :1 
	tFuente.eTipoFon = tipoFuente;      // Tipo Fuente
	tFuente.eTipoTex = tipoTextura;     // Tipo Textura
	tFuente.iPeso = peso;				// Peso, bold, y todo eso.
	tFuente.bItalic = bItalic;
	tFuente.bUnderLine = bUnderLine;
	tFuente.bStrikeOut = bStrikeOut;
	tFuente.bOutLine = bOutLine,
	tFuente.bSombra = sombra;			// Sombra / Relieve
	tFuente.iCharSet = iCharSet;		// Charset
	tFuente.fEsc = fEsc;                // Escala, generalmente a 1
	tFuente.fOffset = fOffset;          // Offset corrector altura, generalmente a 0.
	tFuente.iIncSombra = iIncSombra;

	poFuente = new cFuente();
	if (poFuente)
	{
		poFuente->inicia(tFuente);

		// No lo voy a hacer, porque el que modifica es el iIdent de cItem, y por ahora, no tiene que ver con el de tFuente.idFuente
		// poFuente->setIdent(tFuente.idFuente);

		// Para indicar si son fuentes redimensionables, con Full Desktop.
		poFuente->setFull(full);
		// Si no se ha podifo insertar, se borra la fuente:
		if (insertar(poFuente))
		{
			delete poFuente;
			iRes = -1;
		}
	}
	return iRes;
}


int cGestorFuentes::genera(sWindow * pWindow)
{
	int iRes = 0;

	if (m_mapFuentes.size() > 0)
	{
		for (auto& par : m_mapFuentes)
		{
			cFuente* fuente = par.second;
			if (fuente)
			{
				iRes = (fuente->Carga(pWindow));
			}
		}
	}

	return iRes;
}


void cGestorFuentes::deleteFuentes()
{
	if (m_mapFuentes.size() > 0)
	{
		for (auto iter = m_mapFuentes.begin(); iter != m_mapFuentes.end(); iter++)
		{
			cFuente* fuente = iter->second;
			if (fuente)
			{
				delete fuente;
			}
		}
		m_mapFuentes.clear();
	}
}


//--------------------------------------------------------------------------
// Insertar la fuente en el mapa
// - por ahora no seria error el que encontrasemos fuente
//   no informamos con un mensaje
//   pero si devuelvo -1
//--------------------------------------------------------------------------
int cGestorFuentes::insertar(cFuente* pFuente)
{
	// comprobamos si ya existe la Fuente con el identificador dado.
	cFuente* pHayFuentePorId = getFuente(pFuente->getIdent());
	if (pHayFuentePorId)
	{
		return -1;
	}

	std::pair<int, cFuente*> par{ pFuente->DarIdFuente(), pFuente };
	m_mapFuentes.insert(par);

	return 0;
}

//--------------------------------------------------------------------------
// Busquedas por identicador de la fuente
//--------------------------------------------------------------------------
cFuente* cGestorFuentes::getFuente(int iFuente)
{
	cFuente* pFuente = nullptr;

	if (m_mapFuentes.size() > 0)
	{
		auto iter = m_mapFuentes.find(iFuente);
		if (iter != m_mapFuentes.end())
		{
			pFuente = iter->second;
		}
	}

	return pFuente;
}


//--------------------------------------------------------------------------
// Busquedas por nombre de la fuente: la quitamos.
// - no podemos hacer fuentes con el mismo nombre y distintos parametros
// - quitamos que la clave sea por nombre
// - introducimos un identificador original para cada fuente
//--------------------------------------------------------------------------
//cFuente* cGestorFuentes::getFuente(const std::string& sFuente)
//{
//	cFuente* pFuente = nullptr;
//
//	if (m_mapFuentes.size() > 0)
//	{
//		auto iter = m_mapFuentes.find(sFuente);
//		if (iter != m_mapFuentes.end())
//		{
//			pFuente = iter->second;
//		}
//	}
//
//	return pFuente;
//}


//--------------------------------------------------------------------------
//  Carga las fuentes y las Encola a la lista de texturas.
//--------------------------------------------------------------------------
//int cGestorFuentes::genera(bool bFuerzaCarga, const char* pcOrigen)
//{
//	// 
//	return generaCR(bFuerzaCarga, pcOrigen, true);
//}

// CR Contexto Renderizado ?
//int cGestorFuentes::generaCR(bool bFuerzaCarga, const char * pcOrigen, bool bSinContexto)
//{
//		//int iNumElementos;
//		//iNumElementos = m_mapFuentes.size();
//		//if (iNumElementos>0)
//		//{
//  //          cNodo<cFuente *> * poNodo;
//		//    cFuente	* poFuente;
//		//	char vcMensaje	[LON_BUFF];
//
//  //          sprintf_s(vcMensaje, sizeof(vcMensaje), "Genera Fuentes  %s", ((pcOrigen) ? pcOrigen : ""));
//
//  //          cLog::traza(m_iTraza, "\n");
//  //          cLog::traza(m_iTraza, " %s\n", vcMensaje);
//
//  //          // Pensada para que este la ventana de carga ...
//		//	poInterfaz->Escribe(eMENS_CARGA1,vcMensaje);
//
//  //          for (poNodo = Begin(); poNodo; poNodo = getNodoSiguiente())
//		//	{
//  //              poFuente = *poNodo->getDato();
//  //              if	(poFuente)
//		//		{
//  //                  cLog::traza(m_iTraza, " - '%s'\n", poFuente->DarNombre());
//  //                  if (!poFuente->estaActivada())
//  //                  {
//  //                      sprintf_s(vcMensaje, sizeof(vcMensaje), "%s", poFuente->DarNombre());
//  //                      poInterfaz->Escribe(eMENS_CARGA2, vcMensaje);			// ¡¡¡ Fuera de la seccion crititca !!!
//
//  //                      poWindow->entraSeccion();
//		//				{
//		//					poWindow->setRenderContext(bSinContexto);
//		//					//poWindow->keepRenderContext();
//		//					poFuente->Carga(poWindow);
//		//					//poWindow->unKeepRenderContext();
//		//					poWindow->unSetRenderContext(bSinContexto);
//		//				}
//  //                      poWindow->saleSeccion();
//  //                      poInterfaz->EnviaInc();
//  //                  }
//  //                  else
//  //                  {
//  //                      cLog::traza(m_iTraza, " - '%s' ya generada.\n", poFuente->DarNombre());
//  //                  }
//  //              }
//		//	}
//		//    m_bCargadas = true;
//		//}
//	}
//
//	return 0;
//}


//--------------------------------------------------------------------------
// Construccion de fuente sin fichero de textura
//--------------------------------------------------------------------------
//int	cGestorFuentes::ConsFuente	( sWindow * poWindow, int iIdTex, char * pcNombre, int iAncho, int iAlto, int iCharSet )
//{
//	//sFuente tFuente;
//	//cFuente * poFuen;
//
//	//poFuen = new cFuente();
//	//memset((void *) &tFuente, 0, sizeof(tFuente));
//	//tFuente.idTextura = iIdTex;
//	//tFuente.eTipoFon = eTipoFuentes::eFN01;
//	//tFuente.fEsc = 1.0f;
//	//mCopia(tFuente.vcNombre, pcNombre);
//	//tFuente.iAlto = iAlto;
//	//tFuente.iAncho = iAncho;
//	//poFuen->Inicia(&tFuente);
//	//if (poFuen->Carga(poWindow, iCharSet))
//	//{
//	//	oErr.mensaje("- No tenemos fuente [%s]", pcNombre);
//	//	delete poFuen;
//	//}
// //   Insertar(poFuen);
//
//	return 0;
//}


//--------------------------------------------------------------------------
// Crea una fuente igual a la fuente identificada por iFuente
//--------------------------------------------------------------------------
//cFuente* cGestorFuentes::creaFuente(int iFuente)
//{
//	cFuente* poNewFuente = NULL;
//	//if (iFuente > -1)
//	//{
//	//	cFuente* poFuente = getFuente(iFuente);
//	//	if (poFuente)
//	//	{
//	//		cFuente* poFound;
//	//		sFuente tFuente;
//	//		sFuente* ptFuente;
//	//		int iNewIdent;
//
//	//		iNewIdent = iFuente * 100;
//	//		for (poFound = getFuente(iNewIdent); poFound; iNewIdent++)
//	//		{
//	//			poFound = getFuente(iNewIdent);
//	//		}
//
//	//		ptFuente = poFuente->DarRegFuente();
//	//		tFuente.idTextura = iNewIdent;
//	//		tFuente.iAlto = ptFuente->iAlto;            // iAlto/Tamaño base
//	//		tFuente.iAncho = ptFuente->iAncho;          // iAncho/Espacio base
//	//		tFuente.iGrupo = ptFuente->iGrupo;          // Grupo: 0 :1 
//	//		tFuente.eTipoFon = ptFuente->eTipoFon;      // Tipo Funete ?
//	//		tFuente.eTipoTex = ptFuente->eTipoTex;      // Tipo Textura
//	//		tFuente.fEsc = ptFuente->fEsc;              // Escala, generalmente a 1
//	//		tFuente.fOffset = ptFuente->fOffset;        // Offset corrector altura, generalmente a 0.
//	//		tFuente.iPeso = ptFuente->iPeso;            // Peso, bold, y todo eso.
//	//		tFuente.bSombra = ptFuente->bSombra;        // Sombra / Relieve
//	//		tFuente.iIncSombra = 1;
//	//		strcpy_s(tFuente.vcNombre, sizeof(tFuente.vcNombre), ptFuente->vcNombre);
//	//		strcpy_s(tFuente.vcFichero, sizeof(tFuente.vcFichero), ptFuente->vcFichero);
//	//		//----------------------------------------------------------
//	//		int j, k;
//	//		for (j = 0; j < 128; j++)
//	//		{
//	//			for (k = 0; k < 4; k++)
//	//				tFuente.fCoord[j][k] = 0.0f;
//	//		}
//	//		//----------------------------------------------------------
//	//		poNewFuente = new cFuente();
//	//		if (poNewFuente)
//	//		{
//	//			poNewFuente->setIdent(tFuente.idTextura);
//	//			poNewFuente->setFull(poFuente->getFull());
//	//			poNewFuente->Inicia(&tFuente);
//	//			Insertar(poNewFuente);
//	//		}
//	//	}
//	//}
//	return poNewFuente;
//}


//--------------------------------------------------------------------------
// Enumeramos las fuentes
//--------------------------------------------------------------------------
/*------------------------------------------------------------------------*\
|* Para visualizar las Fuentes en primer plano con PruebaFuentes
|* Las funciones de arriba y abajo habria que asociarlas a las teclas
|*   - vk_up
|*   - vk_down 
|*
|* Me falta un mecanismo para poder hacer esta asociacion de manera
|* sencilla.
|*
|*  TODO   pendiente de hacer.
|*
\*------------------------------------------------------------------------*/
cstatic void cGestorFuentes::arriba()
{
	if (iInicio - cGestorFuentes::KNUM_FUENTES_TEST > -1)
		iInicio -= cGestorFuentes::KNUM_FUENTES_TEST;
}

cstatic void cGestorFuentes::abajo()
{
	if (iInicio + cGestorFuentes::KNUM_FUENTES_TEST < static_cast<int>(m_poInstancia->m_mapFuentes.size()))
		iInicio += cGestorFuentes::KNUM_FUENTES_TEST;
}

cstatic void cGestorFuentes::PruebaFuentes(sWindow* poWindow, int iFuenteTest)
{
	int iLineas = 0;
	sString frase;
	float x = 20;
	float y = 30;
	int iNum = static_cast<int>(m_poInstancia->m_mapFuentes.size());

	cFuente* poFuenteDef = cGestorFuentes::m_poInstancia->getFuente(iFuenteTest);
	if (!poFuenteDef)
	{
		cLog::error(" Error: no encuentra la fuente asociada a [%d]\n", iFuenteTest);
		return;
	}
	poFuenteDef->escribe(poWindow, glm::vec3(x, y, 0.0f), 1.0f, cColor::vRojo, "Hay %d fuentes", iNum);
	x = poFuenteDef->getX();
	y = poFuenteDef->getY() + 30;

	for (int iIndex = iInicio; iIndex < iNum && iLineas < KNUM_FUENTES_TEST; iIndex++, iLineas++)
	{
		if (iInicio != 0)
		{
			int mibreak = 0;
		}
		// Por diseño, al cargar y crear los fuentes, el identificador es el Indice
		cFuente* poFuente = cGestorFuentes::m_poInstancia->getFuente(iIndex);
		if (poFuente)
		{
			frase.format(":%3d:%s: Con eñe ¡valores! y ¿Funciona?", iIndex, poFuente->getNombre());
			poFuente->setX(x);
			poFuente->setY(y);
			poFuente->escribe(poWindow, mNextLine(0.0f), 1.0f, cColor::vAmarillo, frase.c_str());
			x = poFuente->getX();
			y = poFuente->getY() + 30.0f;
		}
	}
	poFuenteDef->escribe(
		poWindow,
		glm::vec3(-1.0f, RISK_HEIGHT - 20.0f, 0.0f),
		1.0f,
		cColor::vRojo,
		"Pulsa Arr.  Abj.  para cambiar.     Esc para terminar ... ");
}
/*------------------------------------------------------------------------*/


//--------------------------------------------------------------------------
// CargaTest
// - llama en cGestorFuentes::EnumeraFuentes:
// - a EnumFontFamExProc.
//--------------------------------------------------------------------------
cstatic void cGestorFuentes::CargaTest(sWindow* poWindow, cLog::eTraza eNivelTraza, const char* pcFontName)
{
	if (cLog::siTraza(eNivelTraza))
	{
		iFuentes = 0;
		EnumeraFuentes(poWindow, cLog::eTraza::min, pcFontName, DEFAULT_CHARSET);
		cLog::print(" Hemos encontrado %d fuentes\n", iFuentes);
		cLog::print("\n");
		iFuentes = 0;
	}
}


//--------------------------------------------------------------------------
// EnumFontFamExProc
// - la tenemos definida antes de donde se esta llamando:
// - en cGestorFuentes::EnumeraFuentes
//--------------------------------------------------------------------------
int CALLBACK EnumFontFamExProc(
							    ENUMLOGFONTEX *lpelfe,    // logical-font data
							    NEWTEXTMETRICEX *lpntme,  // physical-font data
							    DWORD FontType,           // type of font
							    LPARAM lParam             // application-defined data
							  )
{
	cGestorFuentes * poGFuentes = (cGestorFuentes *) lParam;

	std::string	oFound;

	poGFuentes->BuscaTipoFuente(FontType,oFound);
	cLog::print(" + -- Font: %2d [%s]   [%s]  [%s]\n",
		cGestorFuentes::iFuentes+1,
		lpelfe->elfFullName,
		lpelfe->elfStyle,
		lpelfe->elfScript);
	cLog::print("     Tipo Fuente: [%s]\n", oFound.c_str());
	cLog::print("     .   CharSet: %d [%s]\n", lpelfe->elfLogFont.lfCharSet, poGFuentes->BuscaCharSet(lpelfe->elfLogFont.lfCharSet));
	cLog::print("     .     Ancho: %d\n", lpelfe->elfLogFont.lfWidth);
	cLog::print("     .      Alto: %d\n", lpelfe->elfLogFont.lfHeight);
	cLog::print("     .      Peso: %d\n", lpelfe->elfLogFont.lfWeight);
	cLog::print("     .    Italic: %s  %d\n", msTrue(lpelfe->elfLogFont.lfItalic), lpelfe->elfLogFont.lfItalic);
	cLog::print("     . UnderLine: %s  %d\n", msTrue(lpelfe->elfLogFont.lfUnderline), lpelfe->elfLogFont.lfUnderline);
	cLog::print("     . StrikeOut: %s  %d\n", msTrue(lpelfe->elfLogFont.lfStrikeOut), lpelfe->elfLogFont.lfStrikeOut);
	cLog::print("     .    Escape: %d\n", lpelfe->elfLogFont.lfEscapement);

	cLog::print("     ==Datos Fisicos== ...\n");
	cLog::print("     Alto (Asc+Des): %d\n", lpntme->ntmTm.tmHeight);
	cLog::print("          -  Ascent: %d\n", lpntme->ntmTm.tmAscent);
	cLog::print("          - Descent: %d\n", lpntme->ntmTm.tmDescent);
	cLog::print("        Ancho (Avg): %d\n", lpntme->ntmTm.ntmAvgWidth);
	cLog::print("        Ancho (Max): %d\n", lpntme->ntmTm.tmMaxCharWidth);
	cLog::print("        Peso       : %d\n", lpntme->ntmTm.tmWeight);
	cLog::print("         First Char: %d %c\n", lpntme->ntmTm.tmFirstChar,
		(isprint(lpntme->ntmTm.tmFirstChar))? lpntme->ntmTm.tmFirstChar: '#');
	cLog::print("          Last Char: %d %c\n", lpntme->ntmTm.tmLastChar,
		(isprint(lpntme->ntmTm.tmLastChar)) ? lpntme->ntmTm.tmLastChar : '#');
	cLog::print("         Break Char: %d %c\n", lpntme->ntmTm.tmBreakChar,
		(isprint(lpntme->ntmTm.tmBreakChar)) ? lpntme->ntmTm.tmBreakChar : '#');
	cLog::print("\n");

	cGestorFuentes* gestor = cGestorFuentes::Instancia();

	gestor->cargaFuente(
		cGestorFuentes::iFuentes,
		(const char *) lpelfe->elfFullName,
		lpelfe->elfLogFont.lfWidth,
		lpelfe->elfLogFont.lfHeight,
		0,
		eTipoFuentes::eFT01,
		eOldTipoTextura::eFT2,
		lpelfe->elfLogFont.lfWeight,
		lpelfe->elfLogFont.lfItalic,
		lpelfe->elfLogFont.lfUnderline,
		lpelfe->elfLogFont.lfStrikeOut,
		false,			
		false,
		nullptr,
		lpelfe->elfLogFont.lfCharSet);

	cGestorFuentes::iFuentes++;

	return 1;
}


cstatic int	cGestorFuentes::EnumeraFuentes(sWindow* poWindow, cLog::eTraza eNivelTraza, const char* pcFaceName, int iCharset)
{
	if (cLog::siTraza(eNivelTraza))
	{
		HDC hDc = poWindow->getDeviceContext();
		LOGFONT tLogFont;
       
		memset((void*)&tLogFont, 0, sizeof(tLogFont));
		tLogFont.lfCharSet = iCharset;
		mInicio(tLogFont.lfFaceName);
		if (pcFaceName)
		{
			cTool::copiaMultibyteToUnicode(pcFaceName, tLogFont.lfFaceName, sizeof(tLogFont.lfFaceName));
		}
		tLogFont.lfPitchAndFamily = 0;
		cLog::print("\n");
		cLog::print(" Fuentes: [%s]\n", (pcFaceName)? pcFaceName: "Todas");
		EnumFontFamiliesEx(hDc, &tLogFont, (FONTENUMPROC) EnumFontFamExProc, (LPARAM) m_poInstancia, 0);
		cLog::print("\n");
	}

	return 0;
}


//--------------------------------------------------------------------------
// Fin de cGestorFuentes.cpp
//--------------------------------------------------------------------------
