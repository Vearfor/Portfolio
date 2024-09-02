/*========================================================================*\
|* cLuz.h
\*========================================================================*/


#pragma once
//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <GL/glew.h>
#include <GLM/glm.hpp>
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enums
//--------------------------------------------------------------------------
enum class TypeLight
{
    VectorIntensity,    // La intensidad la determina un vector
    ScalarIntensity,    // La intensidad la determina un valor escalar float
};
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// Class
//--------------------------------------------------------------------------
class cShader;
//--------------------------------------------------------------------------
class cLuz
{
public:

    virtual void use();

    void Switch() { _bLightOn = !_bLightOn; }
    bool EstaOn() { return _bLightOn; }

    void setAmbientLoc(cShader* pShader, const char* pcNameLoc);
    void setDiffuceLoc(cShader* pShader, const char* pcNameLoc);
    void setSpecularLoc(cShader* pShader, const char* pcNameLoc);

    void setLightColorLoc(cShader* pShader, const char* pcNameLoc);
    void setAmbientIntLoc(cShader* pShader, const char* pcNameLoc);
    void setDiffuseIntLoc(cShader* pShader, const char* pcNameLoc);
    void setSpecularIntLoc(cShader* pShader, const char* pcNameLoc);

    void setLightOnLoc(cShader* pShader, const char* pcNameLoc);

protected:

    cLuz(glm::vec3 color, GLfloat ambientI, GLfloat difusseI, GLfloat specularInt);
    cLuz(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    ~cLuz();

    // Valores VectorIntensity
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;

    // Valores ScalarIntensity
    glm::vec3 _lightColor;
    GLfloat _ambientInt;
    GLfloat _diffuseInt;
    GLfloat _specularInt;

    // Tipo
    TypeLight _type;

    // Activa/No Activa
    bool _bLightOn;

    //----------------------------------------------------------------------
    // Locations
    //----------------------------------------------------------------------
    GLint _ambientLoc = -1;
    GLint _diffuseLoc = -1;
    GLint _specularLoc = -1;

    GLint _lightColorLoc = -1;
    GLint _ambientIntLoc = -1;
    GLint _diffuseIntLoc = -1;
    GLint _specularIntLoc = -1;

    GLint _bLightOnLoc = -1;

    // Aunque el location de la shadowMap tiene mas sentido en la propia ShadowMap.
    GLint _lightProjectionLoc = -1;
    //----------------------------------------------------------------------
};
//--------------------------------------------------------------------------


/*========================================================================*\
|* Fin de cLuz.h
\*========================================================================*/
