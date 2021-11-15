#include "LVCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>


namespace LVGL {

    LVCamera::LVCamera(float w, float h)
    { 
        camPosition = glm::vec3(0.0f,0.0f,0.0f);
        camTarget = glm::vec3(0.0f,0.0f,0.0f);
        camFront = glm::vec3(0.0f, 0.0f, -1.0f); // where the camera goes to
        setTarget(glm::vec3(0.0f,0.0f,0.0f));
        scrCenterX =  w / 2.0;
        scrCenterY =  h / 2.0;
    }

    LVCamera::~LVCamera(){}

    void LVCamera::setTarget(glm::vec3 tar)
    {
        glm::vec3 up(0.0f,1.0f,0.0f);
        //camTarget = glm::vec3(tar.x, camPosition.y, tar.z);
        //Cross product return a perpendicular vector from other two
        //camDirection = glm::normalize(camPosition - camTarget); //z
        //camRight = glm::normalize(glm::cross(up,camDirection)); //x
        //camUp = glm::cross(camDirection,camRight); //y

        //Initializate camera view
        cameraView = glm::lookAt(camPosition, tar, up);
    }

    void
    LVCamera::setPerspective(GLfloat fov, GLfloat resolution, GLfloat near, GLfloat far, ResourceShader *s)
    {
        projection = glm::perspective(glm::radians(fov), resolution, near, 50.0f);
        s->setMat4("projection", projection); 
    }

    void
    LVCamera::setParallel(GLfloat left, GLfloat right, GLfloat top, GLfloat bottom, GLfloat near, GLfloat far, ResourceShader *s)
    {
        projection = glm::ortho(left, right, bottom, top, near, far);
        s->setMat4("projection", projection); 
    }

    void LVCamera::Render(ResourceShader*, glm::mat4 m){}

    void
    LVCamera::Update(GLFWwindow* window, ResourceShader *shader, double posX, double posY) 
    {
        //checkInputMouse(posX,posY);

        //glm::mat4 view = glm::lookAt(camPosition, camPosition + camFront, camUp);
        shader->setMat4("view", cameraView);

        shader->setVec3("camPos", camPosition); //Used for the specular light calculus
        //lastX = scrCenterX;
        //lastY = scrCenterY;
    }

    void
    LVCamera::checkInputMouse(double x, double y)
    {
        /*
        if (firstPressed)
        {
            //lastX = x;
            //lastY = y;
            firstPressed = false;
        }

        GLfloat xoffset = x - lastX;
        GLfloat yoffset = lastY - y; // reversed since y-coordinates go from bottom to top
        lastX = x;
        lastY = y;

        GLfloat sensitivity = 0.01f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;
        

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camFront = glm::normalize(front);
        camPosition.y = 2.4f;
        */
    }

    void
    LVCamera::setCameraRotation(double time)
    {
        const GLfloat radius = 10.0f;
        GLfloat camX = sin(time) * radius;
        GLfloat camZ = cos(time) * radius;
        cameraView = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    }

}