#pragma once

#include "LVEntity.hpp"
#include "../../manager/ResourceShader.hpp"
#include <glfw/glfw3.h>
#include <glm/mat4x4.hpp>


namespace LVGL {

    class LVCamera : public LVEntity {

    public:
        LVCamera(float w, float h);
        ~LVCamera();
        void Render(ResourceShader *, glm::mat4 m) override;
        void Update(GLFWwindow*, ResourceShader *, double, double);


        //getters
        glm::mat4 getProjectionMatrix(){ return projection; }
        glm::mat4 getViewMatrix(){ return cameraView; }
        glm::vec3& getCamPos(){ return camPosition; }
        glm::vec3& getCamDirection(){ return camDirection; }
        glm::vec3& getCamRight(){ return camRight; }
        glm::vec3& getCamFront(){ return camFront; }
        glm::vec3& getCamUp(){ return camUp; }

        //setters
        void setTarget(glm::vec3);
        void setPerspective(GLfloat ,GLfloat ,GLfloat ,GLfloat ,ResourceShader*);
        void setParallel(GLfloat ,GLfloat ,GLfloat ,GLfloat ,GLfloat ,GLfloat ,ResourceShader*);
        void setCameraRotation(double time);
        void setPos(glm::vec3 newPos){camPosition = newPos;}

    private:

        glm::vec3 camPosition, camTarget, camDirection, camRight, camUp, camFront;
        glm::mat4 cameraView = glm::mat4(1.0);
        glm::mat4 projection = glm::mat4(1.0);
        GLfloat yaw   = -90.0f, pitch =  0.0f;
        bool firstPressed = true;
        float scrCenterX, scrCenterY;

        void setCameraConfig(GLfloat ,GLfloat ,GLfloat ,GLfloat ,GLfloat ,GLfloat, GLfloat);
        void checkInputMouse(double X, double Y);

    };

} // namespace LVGL