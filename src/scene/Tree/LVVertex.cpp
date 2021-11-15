#include "LVVertex.hpp"


namespace LVGL {


Vertex::Vertex(glm::vec3 pos, glm::vec3 nor,  glm::vec2 txCord)
    :   position       { pos    }
    ,   normal         { nor    }
    ,   texCoordinates { txCord }
{ }


} // namespace LVGL