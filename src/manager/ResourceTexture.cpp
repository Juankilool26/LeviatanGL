#include "ResourceTexture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

namespace LVGL
{
    void ResourceTexture::loadFile(std::string name, std::string filename)
    {
        glGenTextures(1,&id);
        glBindTexture(GL_TEXTURE_2D, id); 

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        std::string path = "media/textures/" + name + ".bmp";
            
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &colorChannels, 0);
            
        if(data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << stbi_failure_reason() << std::endl;
        }

        stbi_image_free(data);
        setNameFile(filename);
    }
}