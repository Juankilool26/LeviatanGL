#include "ResourceManager.hpp"
#include <algorithm>

#define initialize 1000

namespace LVGL{

    
    ResourceManager::ResourceManager(){
        materials.reserve(initialize);
        textures.reserve(initialize);
        meshes.reserve(initialize);
        shader = std::make_shared<ResourceShader>("lib/LeviatanGL/src/scene/Shader/shader.vert","lib/LeviatanGL/src/scene/Shader/shader.frag");
    }


    ResourceManager* ResourceManager::GetInstance() {
        if (!m_Instance){
            m_Instance = new ResourceManager();
        }

        return m_Instance;
    }

    const ResourceMaterial*
    ResourceManager::getMaterial(std::string name) const {
       for(unsigned int i=0;i<materials.size();i++){
            if(name==materials[i]->getNameFile()){
                return materials[i].get();
            }
        }
        return nullptr;
    }

    ResourceMaterial*
    ResourceManager::getMaterial(std::string name) {
        auto mptr = const_cast<const ResourceManager *>(this)->getMaterial(name);
        std::string pathMaterial = "src/scene/Material/";

        if(!mptr){
            std::shared_ptr<ResourceMaterial> test=std::make_shared<ResourceMaterial>();
            materials.push_back(test);
            test->loadFile(pathMaterial+name+".material", name);
            mptr= test.get();
        }
        
        return const_cast<ResourceMaterial *>(mptr);
    }

    const ResourceMesh*
    ResourceManager::getMesh(std::string name) const {
        for(unsigned int i=0;i<meshes.size();i++){
            if(name==meshes[i]->getNameFile()){
                return meshes[i].get();
            }
        }
        return NULL;
    }

    ResourceMesh*
    ResourceManager::getMesh(std::string path) {
        auto mptr = const_cast<const ResourceManager *>(this)->getMesh(getNameByPath(path));

        if( path == "media/models/vidaEnemigo.obj"  ||  !mptr ){
            std::shared_ptr<ResourceMesh> test=std::make_shared<ResourceMesh>();
            meshes.push_back(test);
            test->loadFile(path, getNameByPath(path));
            //GETTER
            test->setMaterial("media/materials/basicmaterial.txt", shader.get());
            mptr= test.get();
        }
        return const_cast<ResourceMesh *>(mptr);
    }

    // const ResourceShader*
    // ResourceManager::getShader() const {
    //     if(shader!=NULL){
    //         return shader.get();
    //     }
    //     else{
    //         return NULL;
    //     }
    // }

    // ResourceShader*
    // ResourceManager::getShader() {
    //     std::string vertexPath = "src/scene/Shader/shader.vert";
    //     std::string fragmentPath = "src/scene/Shader/shader.frag";
        
    //     if(shader==NULL){
    //         shader = std::make_shared<ResourceShader>();
    //         shader->loadFile(vertexPath,fragmentPath);
    //     }
        
    //     return shader.get();
    // }

   /*  const ResourceTexture*
    ResourceManager::getTexture(std::string name) const {
        for(unsigned int i=0;i<textures.size();i++){
            if(name==textures[i]->getNameFile()){
                return textures[i].get();
            }
        }
        return NULL;
    } */

    /* ResourceTexture*
    ResourceManager::getTexture(std::string name) {
        auto mptr = const_cast<const ResourceManager *>(this)->getTexture(name);
        std::string pathTexture = "src/scene/Mesh/";

        if(!mptr){
            std::shared_ptr<ResourceTexture> test=std::make_shared<ResourceTexture>();
            textures.push_back(test);
            test->loadFile(pathTexture+name+".png", name);
            mptr= test.get();
        }
        
        return const_cast<ResourceTexture *>(mptr);
    } */


    std::vector<ResourceMesh*> ResourceManager::GetResourceAnimation(std::string path, std::string texture, int numKeyFrames) {
        std::vector<ResourceMesh*> keyFrames;
        keyFrames.reserve(numKeyFrames);
        for(int i = 0; i<numKeyFrames; i++){
            ResourceMesh* resource = getMesh(path + std::to_string(i + 1) + ".obj");
            resource->setTexture(texture);
            keyFrames.push_back(resource);
        }

        return keyFrames;    
    }
    


    void ResourceManager::deleteMaterial(std::string name){
        for(unsigned int i=0;i<materials.size();i++){
            if(name.compare(materials[i]->getNameFile())){
                materials.erase(materials.begin()+i);
                break;
            }
        }
    }

    void ResourceManager::deleteMesh(std::string name){
        for(unsigned int i=0;i<meshes.size();i++){
            if(name.compare(meshes[i]->getNameFile())){
                meshes.erase(meshes.begin()+i);
                break;
            }
        }
    }

    /* void ResourceManager::deleteTexture(std::string name){
        for(unsigned int i=0;i<textures.size();i++){
            if(name.compare(textures[i]->getNameFile())){
                textures.erase(textures.begin()+i);
                break;
            }
        }
    } */
    
    std::vector<std::string> ResourceManager::parsePath(std::string str, char pattern){
        int inicio = 0;
        int now = 0;
        std::string splitted;
        std::vector<std::string> trozos;
        
        while(now >= 0){
            now = str.find(pattern, inicio);
            splitted = str.substr(inicio, now - inicio);
            inicio = now + 1;
            trozos.push_back(splitted); 
        }
        return trozos;
    }
    std::string ResourceManager::getNameByPath(std::string str){
        char pattern = '/';
        char pattern2   = '.';        
        std::vector<std::string> pre = parsePath(str, pattern);
        std::string name = parsePath(pre[pre.size()-1],pattern2)[0];
        
        return name;
    }
}