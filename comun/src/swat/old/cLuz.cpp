/*========================================================================*\
|* cLuz.cpp
\*========================================================================*/


#include "cLuz.h"
#include "cShader.h"


//--------------------------------------------------------------------------
// Constructor & Destructor
//--------------------------------------------------------------------------
cLuz::cLuz(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
    : _ambient(ambient)
    , _diffuse(diffuse)
    , _specular(specular)
    , _bLightOn(true)
    , _type(TypeLight::VectorIntensity)
{
    _diffuseInt = (GLfloat)_diffuse.length();
    _ambientInt = (GLfloat)_ambient.length();
    _specularInt = (GLfloat)_specular.length();
    // El principal es el diffuse:
    _lightColor = glm::normalize(_diffuse);
}
//--------------------------------------------------------------------------
cLuz::cLuz(glm::vec3 color, GLfloat ambientI, GLfloat difusseI, GLfloat specularInt)
    : _lightColor(color)
    , _ambientInt(ambientI)
    , _diffuseInt(difusseI)
    , _specularInt(specularInt)
    , _bLightOn(true)
    , _type(TypeLight::ScalarIntensity)
{
    _ambient = glm::normalize(_lightColor * _ambientInt);
    _diffuse = glm::normalize(_lightColor * _diffuseInt);
    _specular = glm::normalize(_lightColor * _specularInt);
}
//--------------------------------------------------------------------------
cLuz::~cLuz()
{
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Resto de metodos
//--------------------------------------------------------------------------
void cLuz::use()
{
    switch (_type)
    {
        case TypeLight::VectorIntensity:
            glUniform3f(_ambientLoc, _ambient.x, _ambient.y, _ambient.z);
            glUniform3f(_diffuseLoc, _diffuse.x, _diffuse.y, _diffuse.z);
            glUniform3f(_specularLoc, _specular.x, _specular.y, _specular.z);
            break;

        case TypeLight::ScalarIntensity:
            glUniform3f(_lightColorLoc, _lightColor.x, _lightColor.y, _lightColor.z);
            glUniform1f(_ambientIntLoc, _ambientInt);
            glUniform1f(_diffuseIntLoc, _diffuseInt);
            glUniform1f(_specularIntLoc, _specularInt);
            break;
        default:
            break;
    }
    glUniform1i(_bLightOnLoc, _bLightOn ? 1 : 0);
}


//--------------------------------------------------------------------------
void cLuz::setAmbientLoc(cShader* pShader, const char* pcNameLoc)
{
    _ambientLoc = pShader->getLocation(pcNameLoc);
}


void cLuz::setDiffuceLoc(cShader* pShader, const char* pcNameLoc)
{
    _diffuseLoc = pShader->getLocation(pcNameLoc);
}


void cLuz::setSpecularLoc(cShader* pShader, const char* pcNameLoc)
{
    _specularLoc = pShader->getLocation(pcNameLoc);
}


//--------------------------------------------------------------------------
void cLuz::setLightColorLoc(cShader* pShader, const char* pcNameLoc)
{
    _lightColorLoc = pShader->getLocation(pcNameLoc);
}


void cLuz::setAmbientIntLoc(cShader* pShader, const char* pcNameLoc)
{
    _ambientIntLoc = pShader->getLocation(pcNameLoc);
}


void cLuz::setDiffuseIntLoc(cShader* pShader, const char* pcNameLoc)
{
    _diffuseIntLoc = pShader->getLocation(pcNameLoc);
}


void cLuz::setSpecularIntLoc(cShader* pShader, const char* pcNameLoc)
{
    _specularIntLoc = pShader->getLocation(pcNameLoc);
}


//--------------------------------------------------------------------------
void cLuz::setLightOnLoc(cShader* pShader, const char* pcNameLoc)
{
    _bLightOnLoc = pShader->getLocation(pcNameLoc);
}
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
//void cLuz::setShadowMapLoc(cShader* pShader, const char* pcNameLoc)
//{
//    // como estabamos sugiriendo, puede que sea mejor que el shadowMap
//    // contenga su propia location
//    _pShadowMap->setShadowMapLoc(pShader, pcNameLoc);
//}
// 
// Este tiene que ser el del shader principal, que no tiene que
// coincidir con nuestro projection
// 
//void cLuz::setLightProjectionLoc(cShader* pShader, const char* pcNameLoc)
//{
//    _lightProjectionLoc = pShader->getLocation(pcNameLoc);
//}
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cLuz.cpp
\*========================================================================*/
