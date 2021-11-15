#include "LVNode.hpp"
#include "LVMesh.hpp"
#include <algorithm>



namespace LVGL {


LVNode::LVNode(){
    childs.reserve(KNodes);
}


bool
LVNode::hasThisChild(std::shared_ptr<LVNode> child){
    std::vector<std::shared_ptr<LVNode>>::iterator it;
    it = find (childs.begin(), childs.end(), child);
    if(it != childs.end())
        return true;
    return false;
}


glm::mat4
LVNode::rotateMatrix(){
    glm::mat4 identity = glm::mat4(1.0f);
    identity = glm::rotate(identity, glm::radians(rotation.y) , glm::vec3(0,1,0));
    identity = glm::rotate(identity, glm::radians(rotation.z) , glm::vec3(0,0,1));
    identity = glm::rotate(identity, glm::radians(rotation.x) , glm::vec3(1,0,0));
    return identity;
}


void
LVNode::setChild(std::shared_ptr<LVNode> child){
    if(hasThisChild(child))
        return;
    childs.push_back(child);
}


glm::mat4
LVNode::calculateTransformationMatrix(glm::mat4 acumMatrix){
    glm::mat4 identity = glm::mat4 {1.0f};

    glm::mat4 tm =    acumMatrix *
                        glm::translate(identity, translation) *
                        rotateMatrix() *
                        glm::scale(identity, scalation);
    transformationMatrix=tm;
    return tm;
}


void
LVNode::run(glm::mat4 acumMatrix){

    if(modificated){
        calculateTransformationMatrix(acumMatrix);
        modificated = false;
    }
    for(auto& child : childs){
        child->run(transformationMatrix);
    }
}


void
LVNode::runedDraw(ResourceShader* shader){
    
    if(entity && visible)
    {
        entity->Render(shader,transformationMatrix);
    }
    
    for(auto& child : childs)
    {
        child->runedDraw(shader);
    }
}


void
LVNode::modificatedFlag(){
    modificated = true;
    for(auto node:childs){
        node->modificatedFlag();
    }
    return;
}

bool
LVNode::removeChild(LVNode* child){
    for(unsigned int i = 0; i<childs.size(); ++i){
        if(child == childs[i].get()){
            childs.erase(childs.begin()+i);
            return true;
        }
    }
    return false;
}

} // namespace LVGL
