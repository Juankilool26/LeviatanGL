#pragma once 

#include "LVVertex.hpp"
#include "LVEntity.hpp"
#include "../../manager/ResourceMesh.hpp"
#include "../../manager/ResourceTexture.hpp"
#include <glew/glew.h>
#include <glfw/glfw3.h>
#include <string>
#include <unordered_map>
#include "../../manager/ResourceMesh.hpp"
#include "LVAnimation.hpp"

namespace LVGL{

    class LVMesh : public LVEntity
    {
        public:

            LVMesh();
            void Render(ResourceShader *shader,glm::mat4 matrix) override;
            void setMesh(ResourceMesh* m){mesh=m;}
            void setTexture(std::string_view);

            void doAnimation(int id);
            void createAnimation(int id, std::vector<ResourceMesh*>, bool);
            void addAnimation(int id,std::shared_ptr<LVAnimation>);
            void animate();
            void animateInterpolated();

        private:
            ResourceMesh* mesh;
            std::shared_ptr<LVAnimation> currentAnim {nullptr}; 
            std::unordered_map<int, std::shared_ptr<LVAnimation>> animations;
            int currentFrame {0};
            int currentDistance{0};
            const int FPS_ANIM {25};
            const float TIME_ANIM {1.0f/FPS_ANIM*1000.f};
            bool end {true};
    };

}