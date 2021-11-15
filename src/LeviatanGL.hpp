#pragma once
#include "manager/ResourceManager.hpp"
#include "scene/Tree/SceneManager.hpp"
#include "scene/Tree/LVHud.hpp"
#include <ImGUI/src/imgui.h>
#include <ImGUI/src/imgui_impl_opengl3.h>
#include <ImGUI/src/imgui_impl_glfw.h>
#include <string_view>


namespace ARCS {
    struct Entity;
    struct CameraComponent;
    struct PositionComponent;
}

namespace LVGL
{
    struct LeviatanGL
    {
        explicit LeviatanGL(int, int, std::string);
        
        //RENDER
        bool update(GLFWwindow*,float);// done -> FIXED
        bool isDeviceRunning() ;
        void dropDevice() ;// done -> FIXED
        void cleanScene() ;  
        void createAnimatedNode(const int, const std::string) ;// done -> FIXED
        void createStaticNode(const int id, const std::string name) ;
        void createStaticNode(const glm::vec3, const glm::vec3, const std::string) ;// no es nuestra OUT
        void createStaticNode(const int id, const glm::vec3 trans, const glm::vec3 rot, const std::string name) ;
        void createFPSCameraNode(const int id, glm::vec3, glm::vec3) ;// done -> FIXED
        void setNodeTexture(int,std::string_view);
        void setCameraTarget(const int id, const glm::vec3 lookat) ;
        void setCameraPosition(const glm::vec3) ;// done
        void setMeshPosition(const int ,const glm::vec3 &) ;
        glm::vec2 getScreenCenterPosition() const ;// done
        void setNoVisible(const int) ;
        void setVisible(const int) ;// creo que no la utilizamos porque no dependemos de irr para las colisiones
        void setVisible(const int ,bool ) ;
       
        void hudInit() ;
        void menuInit() ;
        void pauseInit() ;
        void interInit() ;
        void drawHud(int) ;
        void menuOptions(int) ;
        void secondaryInit(int);
        void pauseClean() ;
        void keyHUD() ;
        void clearKeyHUD(int) ;
        void orbHUD() ;
        void clearOrbHUD() ;
        void interClean() ;
        void initStam() ;
        void drawStam(float) ;
        void drawHudDeath() ;
        void clearHudDeath() ;
        void clearHudLife() ;
        void drawStealBar(int);
        void drawSeen(bool);
        void drawInter(bool);
        void drawConver(int);
        void stopTalking();
        void drawPause(bool);
        void drawDeath(bool);
        void drawOrb(bool);
        void drawKill(bool);
        void setSkybox(std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back);
        glm::vec2 getCursorPosition();
        void setWindowTitle(std::string_view &&);
        void imguiPrerender();
        void loadImagesHud();
        void closeWindow();
        void changeCharging();
        void chargingFunc();

        //INPUT
        bool processInput(GLFWwindow* w, int keyCode);
        bool mouseIsDown(GLFWwindow* w);
        //bool processInput(GLFWwindow* w, glm::vec3& camPos, glm::vec3& camFront, glm::vec3& camUp, float deltaTime, int keyCode);



        //UTIL
        GLFWwindow* getWindow(){return window;}
        SceneManager* getSmgr(){return smgr;}
        float getWidth() { return width; }
        float getHeight(){ return height;}

        void setCursorPosition(double x,double y);
        //void setLocalCursorPosition(double x,double y);

        private:
            void imguiPreRender();
            void imguiPostRender();
            LVHud* test;

            inline static SceneManager* smgr { 0 };
            inline static GLFWwindow* window { 0 };
            double posX, posY = 0;
            int width, height;

            std::vector<std::shared_ptr<LVHud>> lifeContainer;
            int postActLife = 6;

            std::vector<std::shared_ptr<LVHud>> stam;    
            int postActStam = 3; 

            std::vector<std::shared_ptr<LVHud>> barStealth;
            int postActStealth = 0;

            std::vector<std::shared_ptr<LVHud>> eye;  
            bool actEye = false;  

            std::shared_ptr<LVHud> inter;
            bool actInter = false;  

            std::vector<std::shared_ptr<LVHud>> conver;
            int postActConver = 1;
            bool actTalking = false;

            std::shared_ptr<LVHud> pause;  
            bool actPause = false; 

            std::shared_ptr<LVHud> death;  
            bool actDeath = false; 

            std::shared_ptr<LVHud> orb;
            bool actOrb = false;

            std::shared_ptr<LVHud> kill;
            bool actKill = false;

            std::vector<std::shared_ptr<LVHud>> menu1;
            int postActMenu1 = 0;

            std::vector<std::shared_ptr<LVHud>> menu2;
            int postActMenu2 = 0;

            bool playing {false};

            bool paintinMenu2 {false};

            std::shared_ptr<LVHud> charge;
            bool charging {false};

            std::shared_ptr<LVHud> yesKey;
            std::shared_ptr<LVHud> noKey;

            bool gotKey { false };

    };
}