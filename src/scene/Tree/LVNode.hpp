#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "LVEntity.hpp"
#include <glm/gtx/string_cast.hpp>
#include "../../manager/ResourceShader.hpp"

#define KNodes 3

namespace LVGL {

class LVNode {
    public:

        LVNode();
        LVNode(std::shared_ptr<LVEntity> entity);
        ~LVNode(){};

        void addChild(std::shared_ptr<LVNode> child){childs.emplace_back(child);child.get()->setFather(this);};

    // Getters //

        // Devuelve la entidad asociada al nodo
        LVEntity* getEntity()                   const {return entity.get();}

        // Devuelve el nodo padre asociado al nodo
        LVNode*   getFather()                   const {return father;}

        // Devuelve el vector de hijos asociados al nodo
        const std::vector<std::shared_ptr<LVNode>>& getChilds() const {return childs;}

        // Devuelve el valor de translacion asociado al nodo
        glm::vec3 getTranslation()              const {return translation;};

        // Devuelve el valor de rotacion asociado al nodo
        glm::vec3 getRotation()                 const {return rotation;};

        // Devuelve el valor de escalado asociado al nodo
        glm::vec3 getScalation()                const {return scalation;};

        // Devuelve la matriz de transformacion asociada al nodo
        glm::mat4 getTransformationMatrix()     const {return transformationMatrix;};  


    // Setters //

        // Asigna un padre al nodo
        bool setFather(LVNode* newFather)       { father = newFather; return true; }

        // Asigna un padre al nodo
        void setChild(std::shared_ptr<LVNode> child);

        // Asigna una entidad al nodo
        void setEntity(std::shared_ptr<LVEntity> ent)           { entity = ent; }

        // Asigna un valor de translacion al nodo
        void setTranslation(glm::vec3 t){translation=t;modificatedFlag();} 

        // Asigna un valor de rotacion al nodo
        void setRotation(glm::vec3 r){rotation=r;modificatedFlag();}

        // Asigna un valor de escalado al nodo
        void setScalation(glm::vec3 s){scalation=s;modificatedFlag();} 

        // Situa el objeto en una posicion pasada por parametro
        void setPosition(const glm::vec3 &pos){ transformationMatrix = glm::mat4(1.0); setTranslation(pos);}

        
        
        // Elimina el hijo pasado por parametro del vector de hijos del nodo
        bool removeChild(LVNode* child);

        //Delete the entity contained by this node.
        void removeEntity(){ entity.reset(); }

        // Comprueba si tiene el hijo pasado por parametro en el vector de hijos asociados al nodo
        bool hasThisChild(std::shared_ptr<LVNode> child);


        glm::mat4 translateMatrix();
        glm::mat4 rotateMatrix();
        glm::mat4 scaleMatrix();
        glm::mat4 calculateTransformationMatrix(glm::mat4);

        // Identificacion de nodos
        void setNodeId(int i){ id=i; }
        int getNodeId(){ return id; }

        // Parametrizacion de Render by Node
        void setVisible(bool vis) { visible = vis;  }
        bool getVisible()         { return visible; }
        void modificatedFlag();
        
        // Desencadena un dibujado masivo de todos los nodos de la escena
        void run(glm::mat4); 
        void runedDraw(ResourceShader*);

    private:
        std::shared_ptr<LVEntity> entity {nullptr};
        std::vector<std::shared_ptr<LVNode>> childs;
        LVNode* father {nullptr};
        // Identificador
        int id;

        // Flags de Render y calculos de matrices
        bool visible     {true};
        bool modificated {true};


        glm::vec3 translation          { glm::vec3(0.0f) };
        glm::vec3 rotation             { glm::vec3(0.0f) };
        glm::vec3 scalation            { glm::vec3(1.0f) };
        glm::mat4 transformationMatrix { glm::mat4(1.0f) };
};


} // namespace LVGL