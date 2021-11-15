#include "LVMesh.hpp"

namespace LVGL
{

    LVMesh::LVMesh()
    { 
        mesh = nullptr;       
    }

    void LVMesh::Render(ResourceShader *shader, glm::mat4 matrix)
    {
        if(mesh && mesh->texture)
        {
            shader->use();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,mesh->texture->id);

            // draw mesh
            glBindVertexArray(mesh->getVAO());
            shader->setMat4("model",matrix);
            glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            //Set it back to the first texture to begin with next iteration if required
            glActiveTexture(GL_TEXTURE0);
        }
    }

    void LVMesh::setTexture(std::string_view name)
    {
        if(mesh)
        {
            mesh->setTexture(name.data());
        }
    }

    void LVMesh::doAnimation(int id){
        if (currentAnim == nullptr || currentAnim->ID != id) {
            currentAnim = animations[id];
            mesh = currentAnim->keyframes[0];
            //nextMesh = currentAnimation->keyframes[1];
            //distanceBetweenKeyFrames = currentAnimation->framesBetweenKeyframes;
        }
        end=false;
        currentFrame = 0;
    }

    void LVMesh::createAnimation(int id, std::vector<ResourceMesh*> a, bool loop){
        std::shared_ptr<LVAnimation> anim = std::make_shared<LVAnimation>(id, a, loop);
        addAnimation(id,anim);
    }

    void LVMesh::addAnimation(int id, std::shared_ptr<LVAnimation> animation) {
            animations[id] = animation;
    }

    void LVMesh::animate() {
        if(!end){
                currentFrame++;

            if (currentFrame > static_cast<int>(currentAnim->keyframes.size()-1)){
                currentFrame = 0;
                if(!currentAnim->loop) end=true;
            }

            mesh = currentAnim->keyframes[currentFrame];
        }
    }
    //}

    void LVMesh::animateInterpolated() {
        //if(ItsTimeToAnimate()) {
                // si tenemos que cambiar ya de keyFrame...
            if (currentDistance == 1){//distanceBetweenKeyFrames[currentFrame]) {
                animate();
                currentDistance = 0;
            }

            //float percentTick = std::min(1.0f, (static_cast<float>(currentDistance) / distanceBetweenKeyFrames[currentKeyFrameIndex]));

            // de cada keyframe, recorremos sus mallas
            // for (size_t idxMesh = 0; idxMesh < mesh->GetvectorMesh().size(); idxMesh++) {
            //     auto& prevSubMesh = mesh->GetvectorMeshPtr()->at(idxMesh);
            //     auto& nextSubMesh = nextMesh->GetvectorMeshPtr()->at(idxMesh);
            //     for (size_t idxVertex = 0; idxVertex < prevSubMesh.vertices.size(); idxVertex++) {
            //         auto& prevVertex = prevSubMesh.vertices.at(idxVertex);
            //         auto& nextVertex = nextSubMesh.vertices.at(idxVertex);

            //         glm::vec3 position = mix(prevVertex.position, nextVertex.position, percentTick);
            //         glm::vec3 normal = mix(prevVertex.normal, nextVertex.normal, percentTick);
            //         prevVertex.animationOffsetPos = position - prevVertex.position;
            //         prevVertex.animationOffsetNormal = normal - prevVertex.normal;
            //     }
            // }
            currentDistance++;

        //}
    }
}