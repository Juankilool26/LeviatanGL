 #include "LVLight.hpp"

namespace LVGL {

    LVLight::LVLight(int typeL, bool isOn, ResourceShader* s) : lightOn{ isOn }
    {
        type = static_cast<typeLight>(typeL);
        switch (type)
        {
            case directional:
                id = nDirectionals++;
                lightInArray = "directionalLights[" + std::to_string(id) + "]";
                setDirShaderArray(s);
                break;
            case punctual:
                id = nPoints++;
                lightInArray = "pointLights[" + std::to_string(id) + "]";
                setPointShaderArray(s);
                break; 
            case focal:
                id = nSpots++;
                lightInArray = "spotLights[" + std::to_string(id) + "]";
                setSpotShaderArray(s);
                break; 
            default:
                break;
        }
    }

    LVLight::~LVLight(){}
    void LVLight::Render(ResourceShader*, glm::mat4){} //Just to avoid vtable issue

    void LVLight::setPosition(glm::vec3 pos, ResourceShader* s)
    {
        position = pos;
        s->use();
        if((type == punctual || type == focal) && id > -1)
            s->setVec3(lightInArray + ".position", pos);
    }

    void LVLight::setDirection(glm::vec3 dir, ResourceShader* s)
    {
        direction = dir;
        s->use();
        if(type == directional && id > -1)
            s->setVec3(lightInArray + ".direction", dir);
    }

    void LVLight::setAmbient(glm::vec3 amb, ResourceShader* s)
    {
        ambient = amb;
        s->use();
        if(id > -1)
            s->setVec3(lightInArray + ".ambient", amb);
    }

    void LVLight::setDiffuse(glm::vec3 diff, ResourceShader* s)
    {
        diffuse = diff;
        s->use();
        if(id > -1)
            s->setVec3(lightInArray + ".diffuse", diff);
    }

    void LVLight::setSpecular(glm::vec3 spec, ResourceShader* s)
    {
        specular = spec;
        s->use();
        if(id > -1)
            s->setVec3(lightInArray + ".specular", spec);
    }

    void LVLight::setAttenuationCte(float attCte, ResourceShader* s)
    {
        attenuationCte = attCte;
        s->use();
        if((type == punctual || type == focal) && id > -1)
            s->setFloat(lightInArray + ".constant", attCte);
    }

    void LVLight::setAttenuationLineal(float attLin, ResourceShader* s)
    { 
        attenuationLineal = attLin; 
        s->use();
        if((type == punctual || type == focal) && id > -1)
            s->setFloat(lightInArray + ".linear", attLin);
    }

    void LVLight::setAttenuationCuadrat(float attCuad, ResourceShader* s)
    {
        attenuationCuadrat = attCuad;
        s->use();
        if((type == punctual || type == focal) && id > -1)
            s->setFloat(lightInArray + ".quadratic", attCuad);
    }

    void LVLight::setInnerCutOff(float cutOff, ResourceShader* s)
    {
        if(type == focal && id > -1)
            s->setFloat(lightInArray + ".cutOff", glm::cos(glm::radians(cutOff)));
    }

    void LVLight::setOuterCutOff(float cutOff, ResourceShader* s)
    {
        if(type == focal && id > -1)
            s->setFloat(lightInArray + ".outerCutOff", glm::cos(glm::radians(cutOff))); 
    }

    void LVLight::setLightOn(bool bl, ResourceShader* s)
    {
        lightOn = bl;
        s->use();
        if(id > -1)
            s->setBool(lightInArray + ".isOn", bl);
    }


    void LVLight::setDirectionParam(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, bool on, ResourceShader* s)
    {
        direction = dir; ambient = amb; diffuse = diff; specular = spec; lightOn = on;
        s->setVec3(lightInArray + ".direction", dir);
        s->setVec3(lightInArray + ".ambient", amb);
        s->setVec3(lightInArray + ".diffuse", diff);
        s->setVec3(lightInArray + ".specular", spec);
        s->setBool(lightInArray + ".isOn", on);
    }

   void LVLight::setPunctualParam(glm::vec3 pos, float constant, float linear, float quadratic, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, bool on, ResourceShader* s)
   {
        position = pos; attenuationCte = constant; attenuationLineal = linear; attenuationCuadrat = quadratic;
        ambient = amb; diffuse = diff; specular = spec; lightOn = on;
        s->setVec3(lightInArray + ".position", pos);
        s->setFloat(lightInArray + ".constant", constant);
        s->setFloat(lightInArray + ".linear", linear);
        s->setFloat(lightInArray + ".quadratic", quadratic); 
        s->setVec3(lightInArray + ".ambient", amb);
        s->setVec3(lightInArray + ".diffuse", diff);
        s->setVec3(lightInArray + ".specular", spec);
        s->setBool(lightInArray + ".isOn", on);
   }

   void LVLight::setFocalParam(glm::vec3 pos, glm::vec3 dir, float constant, float linear, float quadratic, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float innerCut, float outerCut, bool on, ResourceShader* s)
   {
        direction = dir; innerCutOff = innerCut; outerCutOff = outerCut;
        s->setFloat(lightInArray + ".cutOff", glm::cos(glm::radians(innerCut)));
        s->setFloat(lightInArray + ".outerCutOff", glm::cos(glm::radians(outerCut))); 
        setPunctualParam(pos, constant, linear, quadratic, amb, diff, spec, on, s);
   }

   void LVLight::setDirShaderArray(ResourceShader* s)
   {
       s->setInt("actual_dLights", nDirectionals);
   }

   void LVLight::setPointShaderArray(ResourceShader* s)
   {
       s->setInt("actual_pLights", nPoints);
   }

   void LVLight::setSpotShaderArray(ResourceShader* s)
   {
       s->setInt("actual_sLights", nSpots);
   }
    
}