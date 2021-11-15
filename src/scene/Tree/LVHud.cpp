#include "LVHud.hpp"

namespace LVGL{

    bool
    LVHud::loadTextureFromFile(std::string filename){
    
        unsigned char* image_data = stbi_load(filename.c_str(), &width, &height, NULL, 4);
        if (image_data == NULL)
            return false;

        // Create a OpenGL texture identifier
        glGenTextures(1, &idImage);
        glBindTexture(GL_TEXTURE_2D, idImage);

        // Setup filtering parameters for display
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F); // This is required on WebGL for non power-of-two textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F); // Same

        // GL_TEXTURE_WRAP_S - 0x2803
        // GL_TEXTURE_WRAP_T - 0x2803

        // Upload pixels into texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
        stbi_image_free(image_data);

        return true;
    }






}