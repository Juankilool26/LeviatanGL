#pragma once
#include <glm/glm.hpp>

namespace LVGL
{


struct Vertex
{
    Vertex(glm::vec3 ,glm::vec3 ,glm::vec2 );
    
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoordinates;
};


} // namespace LVGL
