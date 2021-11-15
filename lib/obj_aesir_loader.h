#include <vector>
#include "glm/glm.hpp"
#include <string.h>

class obj_aesir_loader
{

public:

    obj_aesir_loader();
    ~obj_aesir_loader();
    bool loadOBJ(std::string);
    std::vector< glm::vec3 > getVert(){return temp_vertices_done;};
    std::vector< glm::vec3 > getNorm(){return temp_normals_done;};
    std::vector< glm::vec2 > getUV(){return temp_uvs_done;};

private:

    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;
    std::vector< glm::vec3 > temp_vertices_done;
    std::vector< glm::vec2 > temp_uvs_done;
    std::vector< glm::vec3 > temp_normals_done;

};