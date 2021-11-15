#include "ResourceMaterial.hpp"
#include <fstream>
#include <vector>
#include <iostream>

namespace LVGL
{

   ResourceMaterial::ResourceMaterial(std::string path, ResourceShader* s)
   {
      //Open txt where the material is stored, read it and fill private variables.
      loadFile(path);

      setAmbient(ambient, s);
      setDiffuse(s); 
      setSpecular(specular, s);
      setShininess(shininess, s);
   }

   void ResourceMaterial::loadFile(std::string filepath, std::string name)
   {
      std::ifstream materialFile;
      materialFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
      try 
      {
          // open files
         materialFile.open(filepath);
         float amb1, amb2, amb3, spec1, spec2, spec3;

         materialFile >> amb1 >> amb2 >> amb3 >> spec1 >> spec2 >> spec3 >> shininess >> materialName;

         ambient = glm::vec3(amb1, amb2, amb3);
         specular = glm::vec3(spec1, spec2, spec3);

         materialFile.close();
      }
      catch(std::ifstream::failure const&)
      {
         std::cout << "ERROR::MATERIAL::FILE_NOT_SUCCESFULLY_READ" << std::endl;
      }
   }


   void ResourceMaterial::setAmbient(glm::vec3 amb, ResourceShader* s)
   {
      ambient = amb;
      s->setVec3("material.ambient", amb);
   }

   void ResourceMaterial::setDiffuse(ResourceShader* s)
   {
      s->setInt("material.diffuse", 0);
   }

   void ResourceMaterial::setSpecular(glm::vec3 spec, ResourceShader* s)
   {
      specular = spec;
      s->setVec3("material.specular", spec);
   }

   void ResourceMaterial::setShininess(float sh, ResourceShader* s) 
   {
      shininess = sh;
      s->setFloat("material.shininess", sh);
   }

}