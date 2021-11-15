#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "LVEntity.hpp"

namespace LVGL{

    class LVLight : public LVEntity {      // Abstract class


        public:
        
            inline static int nDirectionals { 0 };
            inline static int nPoints       { 0 };
            inline static int nSpots        { 0 };

            LVLight() {}
            LVLight(int, bool, ResourceShader*);
            ~LVLight();

            // Getters
            glm::vec3 getPosition() const { return position; }
            glm::vec3 getDirection() const { return direction; }
            glm::vec3 getAmbient() const { return ambient; }
            glm::vec3 getDiffuse() const { return diffuse; }
            glm::vec3 getSpecular() const { return specular; }
            float getAttenuationCte() const { return attenuationCte; }
            float getAttenuationLineal() const { return attenuationLineal; }
            float getAttenuationCuadrat() const { return attenuationCuadrat; }
            float getInnerCutOff() const { return innerCutOff; }
            float getOuterCutOff() const { return outerCutOff; }
            bool getLightOn() const { return lightOn; }
            
            // Setters
            void setPosition(glm::vec3 pos, ResourceShader* s);
            void setDirection(glm::vec3 dir, ResourceShader* s);
            void setAmbient(glm::vec3 amb, ResourceShader* s);
            void setDiffuse(glm::vec3 diff, ResourceShader* s);
            void setSpecular(glm::vec3 spec, ResourceShader* s);
            void setAttenuationCte(float attCte, ResourceShader* s);
            void setAttenuationLineal(float attLin, ResourceShader* s);
            void setAttenuationCuadrat(float attCuad, ResourceShader* s);
            void setInnerCutOff(float cutOff, ResourceShader* s);
            void setOuterCutOff(float cutOff, ResourceShader* s);
            void setLightOn(bool bl, ResourceShader* s);

            //Setters for specific type of lights
            void setDirectionParam(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, bool on, ResourceShader* s);
            void setPunctualParam(glm::vec3 pos, float constant, float linear, float quadratic, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, bool on, ResourceShader* s);
            void setFocalParam(glm::vec3 pos, glm::vec3 dir, float constant, float linear, float quadratic, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float innerCut, float outerCut, bool on, ResourceShader* s);

            //Setters for specify the shader whether increment the size of each light array
            void setDirShaderArray(ResourceShader* s);
            void setPointShaderArray(ResourceShader* s);
            void setSpotShaderArray(ResourceShader* s);
            
            // Functions
            void Render(ResourceShader*, glm::mat4) override;

        private:
            
            bool lightOn { false };
            int id = -1;

            enum typeLight {
                directional,             // Only 1 direction
                punctual,                // Only 1 position
                focal                   // 1 position and 1 position
            };
            
            typeLight type;
            std::string lightInArray;

            // To type light focal: how light fades with distance
            float attenuationCte, attenuationLineal, attenuationCuadrat, innerCutOff, outerCutOff;
            glm::vec3 position, direction, ambient, diffuse, specular;
    };
    
}