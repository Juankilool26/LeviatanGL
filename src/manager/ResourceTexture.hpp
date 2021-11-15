#pragma once 

#include <string>
#include <vector>
#include <glew/glew.h>
#include <glfw/glfw3.h>
#include "Resource.hpp"


namespace LVGL{

    struct ResourceTexture : public Resource{  
        ResourceTexture(std::string name){ loadFile(name); }
        ~ResourceTexture() { glDeleteTextures(1, &id); }
        void loadFile(std::string, std::string = "") override;
        
        const GLuint getTexture() const { return id; }
        
        GLuint id;
        std::string type; //Difusse, specular...
        int width, height, colorChannels;
    };

}