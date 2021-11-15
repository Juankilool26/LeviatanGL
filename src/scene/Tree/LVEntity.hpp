#pragma once

#include <glm/glm.hpp>
#include "../../manager/ResourceShader.hpp"

namespace LVGL{

    class LVEntity{ //Abstract class

        public:
            //Constructor and destructor
            LVEntity(){};
            ~LVEntity();

            //Getter
            unsigned int getId() { return idEntity; }

            //Functions
            virtual void Render(ResourceShader *,glm::mat4) = 0;

        private:
            unsigned int idEntity;
            
    };
    
}