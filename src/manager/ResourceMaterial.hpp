#pragma once

#include "Resource.hpp"
#include "ResourceShader.hpp"

namespace LVGL{
    class ResourceMaterial : public Resource{
        public:
            ResourceMaterial() : Resource(){};
            ResourceMaterial(std::string, ResourceShader*);
            ~ResourceMaterial() = default;
            void loadFile(std::string,std::string = "") override;

            void setAmbient(glm::vec3, ResourceShader*);
            void setDiffuse(ResourceShader*);
            void setSpecular(glm::vec3, ResourceShader*);
            void setShininess(float, ResourceShader*);

            const glm::vec3 getAmbient()  const { return ambient; }
            const glm::vec3 getSpecular() const { return specular; }
            const float getShininess()    const { return shininess; }
            const std::string getMaterialName()    const { return materialName; }

        private:
            glm::vec3 ambient, specular;
            float shininess;
            std::string materialName;
    };
}