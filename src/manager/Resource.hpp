#pragma once

#include <glm/glm.hpp>
#include <glew/glew.h>
#include <string>


namespace LVGL{
    class Resource{
        public:
            Resource() = default;
            virtual ~Resource() = default; 
            virtual void loadFile(std::string,std::string) = 0;
            
            void setNameFile(std::string name){nameFile=name;}
            int resourceID { ++nextID };
            const std::string getNameFile() const { return nameFile; }

        private:
            inline static int nextID { 0 };
            std::string nameFile;
            
    };
}