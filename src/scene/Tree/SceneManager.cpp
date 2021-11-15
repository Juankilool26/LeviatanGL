#include "SceneManager.hpp"

namespace LVGL{

    SceneManager::SceneManager():rMan{ ResourceManager::GetInstance()}{
        
        rMan->getShader()->use();

        nodes.reserve(1000);
        directionalLights.reserve(30);
        pointLights.reserve(30);
        spotLights.reserve(30);
    }
    
    SceneManager* SceneManager::GetInstance() {
        if (!m_Instance){
            m_Instance = new SceneManager();
        }

        return m_Instance;
    }

    LVNode* SceneManager::getNodeById(int id){
        for(unsigned int i=0; i < nodes.size(); ++i) {
            if(id==nodes[i]->getNodeId()) {
                return nodes[i].get();
            } else {
                auto& childs { nodes[i]->getChilds() };
                for (unsigned int j=0; j<childs.size(); ++j) {   // TODO: pa optimizar, variable privada con numero de hijos
                    if(id == childs[j]->getNodeId())
                        return childs[j].get();
                }
            }
        }
        return NULL;
    }
    LVNode* SceneManager::addSceneNodeMesh(std::string name){
        auto* mesh = rMan->getMesh(name);
        std::shared_ptr<LVNode> node = std::make_shared<LVNode>();
        std::shared_ptr<LVMesh> entMesh = std::make_shared<LVMesh>();
        entMesh->setMesh(mesh);
        //node.get()->setEntity(mesh);
        node->setEntity(entMesh);
        //parent->addChild(node);
        nodes.push_back(node);
        return node.get();
    }

    LVNode* SceneManager::addSceneDirectionalNodeLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, bool on)
    {
        std::shared_ptr<LVNode> node = std::make_shared<LVNode>();
        std::shared_ptr<LVLight> lgt = std::make_shared<LVLight>(0, on, rMan->getShader());
        lgt->setDirectionParam(dir,amb,diff,spec,on,rMan->getShader());
        node.get()->setEntity(lgt);
        return node.get();
    }

    LVNode* SceneManager::addScenePointNodeLight(glm::vec3 pos, float constant, float linear, float quadratic, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, bool on)
    {
        std::shared_ptr<LVNode> node = std::make_shared<LVNode>();
        std::shared_ptr<LVLight> lgt = std::make_shared<LVLight>(1, on, rMan->getShader());
        lgt->setPunctualParam(pos, constant, linear, quadratic, amb, diff, spec, on, rMan->getShader());
        node->setEntity(lgt);
        return node.get();
    }

    LVNode* SceneManager::addSceneSpotNodeLight(glm::vec3 pos, glm::vec3 dir, float constant, float linear, float quadratic, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float innerCut, float outerCut, bool on)
    {
        std::shared_ptr<LVNode> node = std::make_shared<LVNode>();
        std::shared_ptr<LVLight> lgt = std::make_shared<LVLight>(2, on, rMan->getShader());
        lgt->setFocalParam(pos, dir, constant, linear, quadratic, amb, diff, spec, innerCut, outerCut, on, rMan->getShader());
        node.get()->setEntity(lgt);
        return node.get();
    }

    LVNode* SceneManager::addSceneNodeCameraFPS(float width, float height){
        camera = std::make_shared<LVCamera>(width,height);
        std::shared_ptr<LVNode> node = std::make_shared<LVNode>();
        camera->setPerspective(45.0f,width/height,0.1,150.0, rMan->getShader());
        node->setEntity(camera);
        //node.get()->setEntity(camera);
        //parent->addChild(node);
        nodes.push_back(node);
        rMan->getShader()->setVec3("camPos", camera->getCamPos());
        return node.get();
    }

    void SceneManager::addSkybox(std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back)
    {
        if(skybox.get() == nullptr)
            skybox = std::make_unique<LVSkybox>();
        skybox->loadSkybox(right, left, top, bottom, front, back);
    }

    void SceneManager::deleteLights()
    {
        LVLight::nDirectionals = 0;
        LVLight::nPoints = 0;
        LVLight::nSpots = 0;

        LVLight l; //Provisional light to change some shader parameters.

        l.setDirShaderArray(rMan->getShader());
        l.setPointShaderArray(rMan->getShader());
        l.setSpotShaderArray(rMan->getShader());

        directionalLights.clear();
        pointLights.clear();
        spotLights.clear();

    }

    void SceneManager::update(GLFWwindow* window, double x, double y){
        if(camera){
            camera.get()->Update(window, rMan->getShader(), x, y);
        }
        if(skybox){
            skybox->render(camera->getViewMatrix(), camera->getProjectionMatrix());
        }
        rMan->getShader()->use();
        for(unsigned int i=0; i < nodes.size(); i++){
            nodes[i].get()->runedDraw(rMan->getShader());
        }
    }

    void SceneManager::act(){
        for(unsigned int i=0; i < nodes.size(); i++){
            nodes[i].get()->run(nodes[i].get()->getTransformationMatrix());
        }
    }

    void SceneManager::clean(){
        camera.reset();
        skybox.reset();
        deleteLights();
        /* for(int i = 0; i < nodes.size(); i++)
        {
            nodes[i]->removeEntity();
        } */
        nodes.clear();
    }
}