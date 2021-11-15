#pragma once

#include "Resource.hpp"
#include "ResourceMaterial.hpp"
#include "ResourceTexture.hpp"
#include "../scene/Tree/LVVertex.hpp"
#include <memory>

namespace LVGL{
    class ResourceMesh : public Resource{


        public:
            
            ResourceMesh() : Resource() {};
            ~ResourceMesh() { glDeleteVertexArrays(1, &VAO),glDeleteBuffers(1, &VBO),glDeleteBuffers(1, &EBO); }

            void loadFile(std::string,std::string) override;
            void setTexture(std::string);
            void setMaterial(std::string, ResourceShader*);

            std::vector<Vertex>       vertices;
            std::vector<unsigned int> indices;
            std::unique_ptr<ResourceTexture>  texture;
            std::unique_ptr<ResourceMaterial> material;

            const GLint getVAO() const { return VAO; }
            const GLint getVBO() const { return VBO; }
            const GLint getEBO() const { return EBO; }
            
        private:
            unsigned int VAO, VBO, EBO;
    };
}