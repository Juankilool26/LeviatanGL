#include "LVSkybox.hpp"
#include <iostream>
#include "stb_image.h"


namespace LVGL
{
    LVSkybox::LVSkybox()
    {
        //Initialize vertex buffers and array
        glGenBuffers(1, &skyVBO);
        glGenVertexArrays(1, &skyVAO);

        glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
        glBindVertexArray(skyVAO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        skyShader = std::make_unique<ResourceShader>("lib/LeviatanGL/src/scene/Shader/skyboxshader.vert","lib/LeviatanGL/src/scene/Shader/skyboxshader.frag");
    }

    void LVSkybox::loadSkybox(std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back)
    {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
        unsigned char *data;

        //Create a vector to go through all cube faces.
        std::vector<std::string> faces;
        faces.reserve(6);
        faces.push_back(right);
        faces.push_back(left);
        faces.push_back(top);
        faces.push_back(bottom);
        faces.push_back(front);
        faces.push_back(back);


        for(unsigned int i=0; i < faces.size(); i++)
        {
            data = stbi_load(faces[i].c_str(), &width, &height, &colorChannels, 0);
            if(data)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else
            {
                std::cout << "Failed loading skybox with path: " << faces[i] << std::endl;
                stbi_image_free(data);
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void LVSkybox::render(glm::mat4 view, glm::mat4 projection)
    {
        glDepthFunc(GL_LEQUAL);
        skyShader->use();
        glm::mat4 viewWithoutW = glm::mat4(glm::mat3(view));
        skyShader->setMat4("view", viewWithoutW);
        skyShader->setMat4("projection", projection);
        glBindVertexArray(skyVAO);
        glActiveTexture(0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);
        glActiveTexture(GL_TEXTURE0);
    }

}