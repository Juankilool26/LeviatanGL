#pragma once

#include "LVCamera.hpp"
#include "LVNode.hpp"
#include "LVMesh.hpp"
#include "LVLight.hpp"
#include "LVSkybox.hpp"
#include "../../manager/ResourceManager.hpp"
#include "../../manager/ResourceShader.hpp"
#include <vector>
#include <memory>
#include <string>

#define nLights 15

namespace LVGL{

    class SceneManager{
        public:
            static SceneManager* GetInstance();

            //GETERS                        
            LVNode* getNodeById(int id);

            //METHODS
            LVNode* addSceneNodeMesh(std::string);
            LVNode* addSceneDirectionalNodeLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, bool on);
            LVNode* addScenePointNodeLight(glm::vec3 pos, float constant, float linear, float quadratic, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, bool on);
            LVNode* addSceneSpotNodeLight(glm::vec3 pos, glm::vec3 dir, float constant, float linear, float quadratic, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float innerCut, float outerCut, bool on);
            LVNode* addSceneNodeCameraFPS(float,float);
            void    addSkybox(std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back);
            void    deleteLights();
            LVCamera* getCamera() { return camera.get(); }
            ResourceManager& getResourceManager(){ return *rMan; }

            void update(GLFWwindow* window, double, double);
            void act();
            void clean();
            std::vector<std::shared_ptr<LVNode>>  getNodes(){ return nodes;}
            
        private:
            SceneManager();
            inline static SceneManager* m_Instance { 0 };
            //std::unique_ptr<LVNode> parent {nullptr};
            ResourceManager* rMan;
            std::vector<std::shared_ptr<LVNode>>  nodes;
            std::vector<std::shared_ptr<LVLight>> directionalLights;
            std::vector<std::shared_ptr<LVLight>> pointLights;
            std::vector<std::shared_ptr<LVLight>> spotLights;
            std::shared_ptr<LVCamera> camera;
            std::unique_ptr<LVSkybox> skybox;

    };
}