#pragma once

#include "ResourceMaterial.hpp"
#include "ResourceMesh.hpp"
#include "ResourceShader.hpp"
#include "ResourceTexture.hpp"
#include <vector>
#include <memory>

namespace LVGL{
    class ResourceManager{
        public:
            static ResourceManager* GetInstance();

            //GETERS
            const ResourceMaterial*    getMaterial(std::string) const;
                  ResourceMaterial*    getMaterial(std::string);

            const ResourceMesh*        getMesh(std::string) const;
                  ResourceMesh*        getMesh(std::string);

                  ResourceShader*      getShader()     { return shader.get(); }

            const ResourceTexture*     getTexture(std::string) const;
                  ResourceTexture*     getTexture(std::string);

            std::vector<ResourceMesh*> GetResourceAnimation(std::string file, std::string texture, int numKeyFrames);
                        
            //DELETES
            void deleteMaterial(std::string);
            void deleteMesh(std::string);
            void deleteTexture(std::string);
            //void deleteShader();

            // TOOLS
            std::vector<std::string> parsePath(std::string, char);
            std::string         getNameByPath(std::string);

        private:
            ResourceManager();
            inline static ResourceManager* m_Instance { 0 };
            
            std::shared_ptr<ResourceShader>                  shader;
            std::vector<std::shared_ptr<ResourceMaterial>>   materials;
            std::vector<std::shared_ptr<ResourceMesh>>       meshes;
            std::vector<std::shared_ptr<ResourceTexture>>    textures;
    };
}