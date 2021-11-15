#include "ResourceMesh.hpp"
#include "OBJ_Loader.h"

namespace LVGL
{
    void ResourceMesh::loadFile(std::string filepath,std::string name){
        objl::Loader loader;
        loader.LoadFile(filepath);
        
        unsigned int verSize =    loader.LoadedMeshes[0].Vertices.size();
        unsigned int indSize =    loader.LoadedMeshes[0].Indices.size();

        vertices.reserve(verSize);
        indices.reserve(indSize);

        //Load vertex
        for(unsigned int i = 0;i < verSize;i++)
        {
            objl::Vertex v = loader.LoadedMeshes[0].Vertices[i];
            vertices.emplace_back(glm::vec3(v.Position.X ,v.Position.Y,v.Position.Z),
                                  glm::vec3(v.Normal.X, v.Normal.Y,v.Normal.Z),
                                  glm::vec2(v.TextureCoordinate.X,v.TextureCoordinate.Y));
        }

        //Load indices
        for(unsigned int i = 0;i < indSize;i++)
        {
            indices.emplace_back(loader.LoadedMeshes[0].Indices[i]);
        }

        //Load textures
        //GETTER
        texture = std::make_unique<ResourceTexture>(name);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        //bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoordinates));
        glEnableVertexAttribArray(2);
        // Unbind VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        // Unbind VAO 
        glBindVertexArray(0);

        setNameFile(name);
    }

    void ResourceMesh::setTexture(std::string name)
    {
        //GETTER
        texture.reset();
        texture = std::make_unique<ResourceTexture>(name);
    }

    void ResourceMesh::setMaterial(std::string path, ResourceShader* s)
    {
        //GETTER
        material.reset();
        material = std::make_unique<ResourceMaterial>(path, s);
    }
}