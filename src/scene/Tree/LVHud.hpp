#include "stb_image.h"
#include <string>
#include <glfw/glfw3.h>

namespace LVGL{

class LVHud
{
    public:
        LVHud() = default;
        bool loadTextureFromFile(std::string);
        GLuint  getImageId()        { return idImage;   };
        int     getImageWidth()     { return width;     };
        int     getImageHeight()    { return height;    };

    private:
        GLuint idImage;
        int width, height;
    };  
}