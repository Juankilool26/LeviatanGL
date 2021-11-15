#include "LeviatanGL.hpp"

namespace LVGL
{

    LeviatanGL::LeviatanGL(int w, int h, std::string tittle)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(w, h, tittle.c_str(), NULL, NULL);
        if (window == NULL)
        {
            glfwTerminate();
        }
        this->width=w;
        this->height=h;
        glfwMakeContextCurrent(window);

        GLenum err=glewInit();
        if(err!=GLEW_OK) {
            exit(1);
        }
        smgr = SceneManager::GetInstance();

        glViewport(0, 0, w, h);
        glEnable(GL_DEPTH_TEST); //enable depth test
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        
        //Set a keyboard and mouse functions to handle events.
        glfwSetWindowUserPointer(window,this);

        
        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mode){
            //LeviatanGL* lv = (LeviatanGL*)glfwGetWindowUserPointer(window);
            // if(key == 256)
            // {
            //     glfwSetWindowShouldClose(window,true);
            // }
        });

        const char* glsl_version= "#version 130";

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        /*
        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos){
            LeviatanGL* lv = (LeviatanGL*)glfwGetWindowUserPointer(window);
            lv->setLocalCursorPosition(xpos, ypos);
        });
        */

        
        lifeContainer.reserve(7);
        stam.reserve(4);
        barStealth.reserve(6);
        eye.reserve(2);
        conver.reserve(7);
        menu1.reserve(4);
        menu2.reserve(7);

        loadImagesHud();
        
    }
 

    //RENDER
    bool 
    LeviatanGL::update(GLFWwindow* window, float delta)
    {
        imguiPreRender();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        smgr->act();
        smgr->update(window, posX, posY);
        posX = width  / 2.0;
        posY = height / 2.0;
        setCursorPosition(posX, posY);
        hudInit();
        chargingFunc();
        imguiPostRender();

        glfwSwapBuffers(window);
        glfwPollEvents();

        return true;
    }

    bool LeviatanGL::isDeviceRunning()
    {
        return !glfwWindowShouldClose(window);
    }

    void
    LeviatanGL::closeWindow(){
        return glfwSetWindowShouldClose(window,true);
    }

    void LeviatanGL::dropDevice()
    {
        cleanScene();
        delete smgr;
        glfwTerminate();
    }
    
    void LeviatanGL::createAnimatedNode(const int id, const std::string name)
    {
        LVNode* node {smgr->addSceneNodeMesh(name)}; //FIXED ESTO SOLO TIENE QUE RECIBIR EL NOMBRE DEL OBJ
        node->setNodeId(id);
    }

    void LeviatanGL::createFPSCameraNode(const int id, glm::vec3 posCam, glm::vec3 lookAt) 
    {
        LVNode* camNode = smgr->addSceneNodeCameraFPS(width, height);
        camNode->setNodeId(id);
        smgr->getCamera()->setPos(posCam);
        //lookAt = glm::vec3(lookAt.x, lookAt.y - 90.0f, lookAt.z);
        setCameraTarget(id, lookAt);
    }

    void LeviatanGL::setNodeTexture(int id,std::string_view name)
    {
        auto* node = smgr->getNodeById(id);
        auto* ent = node->getEntity();
        if(LVMesh* mesh = dynamic_cast<LVMesh*>(ent))
            mesh->setTexture(name);
    }

    void LeviatanGL::setCameraPosition(const glm::vec3 pos)
    {   
        smgr->getCamera()->setPos({pos.x, pos.y - 5, pos.z});
    }

    glm::vec2 LeviatanGL::getScreenCenterPosition() const{
        return glm::vec2{width/2, height/2};
    }

    void LeviatanGL::cleanScene()
    {
        smgr->clean();
        setCursorPosition(0,0);
    }

    void LeviatanGL::createStaticNode(const int id, const std::string name)
    {
        LVNode* n = smgr->addSceneNodeMesh(name.data());
        n->setNodeId(id);
    }

    void LeviatanGL::createStaticNode(const int id, const glm::vec3 trans, const glm::vec3 rot, const std::string name)
    {
        LVNode* n = smgr->addSceneNodeMesh(name.data());
        n->setTranslation(trans);
        n->setRotation(rot);
        n->setNodeId(id);
    }

    void LeviatanGL::setCameraTarget(const int id, const glm::vec3 lookat)
    {
        smgr->getCamera()->setTarget(lookat);
    }

    void LeviatanGL::setCursorPosition(double x,double y) {
        glfwSetCursorPos(window, x, y);
        posX = x;  posY = y;
    }
/*
    void LeviatanGL::setLocalCursorPosition(double x,double y) {
        posX = x;  posY = y;
        //smgr->getCamera()->setLast(x, y);
    }
*/

    glm::vec2
    LeviatanGL::getCursorPosition() {
        glfwGetCursorPos(window, &posX, &posY);
        return glm::vec2(posX, posY);
    }

    void LeviatanGL::setMeshPosition(const int id,const glm::vec3 &pos){}

    bool LeviatanGL::processInput(GLFWwindow* w, int keyCode)
    {        
        return (glfwGetKey(w, keyCode) == GLFW_PRESS);
    }

    bool LeviatanGL::mouseIsDown(GLFWwindow* w)
    {
        return (glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    }

    void LeviatanGL::setWindowTitle(std::string_view&& title) {
        glfwSetWindowTitle(window, &title.front());
    }

    void
    LeviatanGL::imguiPreRender(){
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void
    LeviatanGL::imguiPostRender(){
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
    }

    void
    LeviatanGL::loadImagesHud(){

        // Load menu1 hud images
        for(unsigned int i = 0; i < 4; i++){
            std::shared_ptr<LVHud> hud = std::make_shared<LVHud>();
            hud->loadTextureFromFile("media/textures/menu1_"+std::to_string(i)+".png");
            menu1.push_back(hud);
        }

        // Load menu2 hud images
        for(unsigned int i = 0; i < 7; i++){
            std::shared_ptr<LVHud> hud = std::make_shared<LVHud>();
            hud->loadTextureFromFile("media/textures/menu2_"+std::to_string(i)+".png");
            menu2.push_back(hud);
        }
        
        
        // Load life hud images
        for(unsigned int i = 0; i < 7; i++){
            std::shared_ptr<LVHud> hud = std::make_shared<LVHud>();
            hud->loadTextureFromFile("media/textures/vida"+std::to_string(i)+".png");
            lifeContainer.push_back(hud);
        }
        // Load stam hud images
        for(unsigned int i = 0; i < 4; i++){
            std::shared_ptr<LVHud> hud = std::make_shared<LVHud>();
            hud->loadTextureFromFile("media/textures/stam"+std::to_string(i)+".png");
            stam.push_back(hud);
        }

        for(unsigned int i = 0; i < 6; i++){
            std::shared_ptr<LVHud> hud = std::make_shared<LVHud>();
            hud->loadTextureFromFile("media/textures/barraSigilo"+std::to_string(i)+".png");
            barStealth.push_back(hud);
        }

        for(unsigned int i = 0; i < 2; i++){
            std::shared_ptr<LVHud> hud = std::make_shared<LVHud>();
            if(i == 0)
                hud->loadTextureFromFile("media/textures/unseen.png");
            else
                hud->loadTextureFromFile("media/textures/seen.png");
            eye.push_back(hud);
        }

        inter = std::make_shared<LVHud>();
        inter->loadTextureFromFile("media/textures/tecla_inter.png");

        for(unsigned int i = 0; i < 7; i++){
            std::shared_ptr<LVHud> hud = std::make_shared<LVHud>();       
            hud->loadTextureFromFile("media/textures/npc"+ std::to_string(i + 1) +".png");
            conver.push_back(hud);
        }

        pause = std::make_shared<LVHud>();
        pause->loadTextureFromFile("media/textures/pause.png");

        death = std::make_shared<LVHud>();
        death->loadTextureFromFile("media/textures/gameOver.png");

        orb = std::make_shared<LVHud>();
        orb->loadTextureFromFile("media/textures/yesOrb.png");

        kill = std::make_shared<LVHud>();
        kill->loadTextureFromFile("media/textures/kill.png");

        charge = std::make_shared<LVHud>();
        charge->loadTextureFromFile("media/textures/pantallaCargaBasic1.png");

        yesKey = std::make_shared<LVHud>();
        yesKey->loadTextureFromFile("media/textures/yesKeyGris.png");

        noKey = std::make_shared<LVHud>();
        noKey->loadTextureFromFile("media/textures/noKey0.png");

    }

    void
    LeviatanGL::hudInit(){

        if(playing){

            //Barra de sigilo
            if(barStealth[postActStealth]){
                ImGui::SetNextWindowPos(ImVec2{40, 0});
                ImGui::Begin("Sigilo",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((void*)(intptr_t) barStealth[postActStealth]->getImageId(), ImVec2(barStealth[postActStealth]->getImageWidth(), barStealth[postActStealth]->getImageHeight()));
                ImGui::End();
            }

            // Barra de vida
            if(lifeContainer[postActLife]){
                ImGui::SetNextWindowPos(ImVec2{0, static_cast<float>(height-lifeContainer[postActLife]->getImageHeight())});
                ImGui::SetNextWindowSize(ImVec2{301,251});
                ImGui::Begin("Vida",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((void*)(intptr_t)lifeContainer[postActLife]->getImageId(), ImVec2(lifeContainer[postActLife]->getImageWidth(), lifeContainer[postActLife]->getImageHeight()));
                ImGui::End();
            }

            // Ojo
            if(eye[actEye]){
                ImGui::SetNextWindowPos(ImVec2{40, 0});
                ImGui::SetNextWindowSize(ImVec2{static_cast<float>(eye[actEye]->getImageWidth()+1), static_cast<float>(eye[actEye]->getImageHeight()+1)});
                ImGui::Begin("Ojo",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((void*)(intptr_t)eye[actEye]->getImageId(), ImVec2(eye[actEye]->getImageWidth(), eye[actEye]->getImageHeight()));
                ImGui::End();
            }

            //Estamina
            if(stam[postActStam]){
                ImGui::SetNextWindowPos(ImVec2{0, static_cast<float>(height-(stam[postActStam]->getImageHeight()+50))});
                ImGui::SetNextWindowSize(ImVec2{301,251});
                ImGui::Begin("Stam",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((void*)(intptr_t)stam[postActStam]->getImageId(), ImVec2(stam[postActStam]->getImageWidth(), stam[postActStam]->getImageHeight()));
                ImGui::End();
            }   
            //Interactuar
            if(actInter){
                ImGui::SetNextWindowPos(ImVec2{625, static_cast<float>(height-(inter->getImageHeight())-30)});
                ImGui::SetNextWindowSize(ImVec2{81,81});
                ImGui::Begin("Inter",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((void*)(intptr_t)inter->getImageId(), ImVec2(inter->getImageWidth(), inter->getImageHeight()));
                ImGui::End();
            }  
            //Conversaciones
            if(actTalking){
                ImGui::SetNextWindowPos(ImVec2{40, 0});
                ImGui::SetNextWindowSize(ImVec2{1241,721});
                ImGui::Begin("Conver",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((void*)(intptr_t)conver[postActConver]->getImageId(), ImVec2(conver[postActConver]->getImageWidth(), conver[postActConver]->getImageHeight()));
                ImGui::End();
            }  

            if(actPause){
                ImGui::SetNextWindowPos(ImVec2{40, 0});
                ImGui::SetNextWindowSize(ImVec2{1241,721});
                ImGui::Begin("Pause",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((void*)(intptr_t)pause->getImageId(), ImVec2(pause->getImageWidth(), pause->getImageHeight()));
                ImGui::End();
            } 

            if(actDeath){
                ImGui::SetNextWindowPos(ImVec2{40, 0});
                ImGui::SetNextWindowSize(ImVec2{1241,721});
                ImGui::Begin("Death",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((void*)(intptr_t)death->getImageId(), ImVec2(death->getImageWidth(), death->getImageHeight()));
                ImGui::End();
            } 

            if(actOrb){
                ImGui::SetNextWindowPos(ImVec2{0, static_cast<float>(height-orb->getImageHeight())});
                ImGui::SetNextWindowSize(ImVec2{301,251});
                ImGui::Begin("Orb",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((void*)(intptr_t)orb->getImageId(), ImVec2(orb->getImageWidth(), orb->getImageHeight()));
                ImGui::End();
            } 

            if(actKill){
                ImGui::SetNextWindowPos(ImVec2{600, static_cast<float>(height-kill->getImageHeight())});
                ImGui::SetNextWindowSize(ImVec2{81,61});
                ImGui::Begin("Kill",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((void*)(intptr_t)kill->getImageId(), ImVec2(kill->getImageWidth(), kill->getImageHeight()));
                ImGui::End();
            }
            if(!gotKey){
                ImGui::SetNextWindowPos(ImVec2{ 0, static_cast<float>(720-250)});
                ImGui::SetNextWindowSize(ImVec2{static_cast<float>(noKey->getImageWidth()),static_cast<float>(noKey->getImageHeight())});
                ImGui::Begin("noKey",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((void*)(intptr_t)noKey->getImageId(), ImVec2(noKey->getImageWidth(), noKey->getImageHeight()));
                ImGui::End();
            } 
            if(gotKey){
                ImGui::SetNextWindowPos(ImVec2{0, static_cast<float>(720-250)});
                ImGui::SetNextWindowSize(ImVec2{static_cast<float>(yesKey->getImageWidth()),static_cast<float>(yesKey->getImageHeight())});
                ImGui::Begin("noKey",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                ImGui::Image((void*)(intptr_t)yesKey->getImageId(), ImVec2(yesKey->getImageWidth(), yesKey->getImageHeight()));
                ImGui::End();
            } 
        }
        else{
            
            if(!paintinMenu2){
                if(menu1[postActMenu1]){
                    ImGui::SetNextWindowPos(ImVec2{-6, -6});
                    ImGui::SetNextWindowSize(ImVec2{static_cast<float>(width+6), static_cast<float>(height+6)});
                    ImGui::Begin("Menu1",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                    ImGui::Image((void*)(intptr_t) menu1[postActMenu1]->getImageId(), ImVec2(menu1[postActMenu1]->getImageWidth(), menu1[postActMenu1]->getImageHeight()));
                    ImGui::End();
                }
            }
            else{
                if(menu2[postActMenu2]){
                    ImGui::SetNextWindowPos(ImVec2{-6, -6});
                    ImGui::SetNextWindowSize(ImVec2{static_cast<float>(width+6),static_cast<float>(height+6)});
                    ImGui::Begin("Menu2",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
                    ImGui::Image((void*)(intptr_t) menu2[postActMenu2]->getImageId(), ImVec2(menu2[postActMenu2]->getImageWidth(), menu2[postActMenu2]->getImageHeight()));
                    ImGui::End();
                }
            }
        }
    }

    // Estandarizado para pintar imagenes del menu inicial

    // ImGui::SetNextWindowPos(ImVec2{-6,-6});
    // ImGui::SetNextWindowSize(ImVec2{width+6,height+6});
    // ImGui::Begin("Prueba hud en bucle de juego",NULL,ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
    // ImGui::Image((void*)(intptr_t)test->getImageId(), ImVec2(test->getImageWidth(), test->getImageHeight()));
    // ImGui::End();

    void
    LeviatanGL::drawHud(int vida){
        if (vida == 6)
            actDeath = false;
        else if (vida < 0)
            vida = 0;
        postActLife = vida;
        playing = true;
    }   

    void 
    LeviatanGL::drawStam(float stamPlayer){

        if(stamPlayer >= 1.2){ //66%
            postActStam = 3;
        }

        if(stamPlayer < 1.2 && stamPlayer >= 0.6){ //33%
            postActStam = 2;
        }

        if(stamPlayer < 0.6 && stamPlayer > 0){
            postActStam = 1;
        }

        if(stamPlayer <= 0){
            postActStam = 0;
        }
    }

    void LeviatanGL::drawStealBar(int stealth){
        postActStealth = stealth;
    }

    void LeviatanGL::drawSeen(bool seen){
        actEye = seen;
    }

    void LeviatanGL::drawInter(bool inte){
        actInter = inte;
    }

    void LeviatanGL::drawConver(int con){
        postActConver = con;
        actTalking = true;
    }

    void LeviatanGL::stopTalking(){
        actTalking = false;
    }

    void LeviatanGL::drawPause(bool paus){
        actPause = paus;
    }

    void LeviatanGL::drawDeath(bool die){
        actDeath = die;
        if(die)
            actOrb = false;
    }

    void LeviatanGL::drawOrb(bool orbe){
        actOrb = orbe;
    }

    void LeviatanGL::drawKill(bool kil){
        actKill = kil;
    }

    void
    LeviatanGL::menuOptions(int option){
        postActMenu1 = option-1;
    }

    void
    LeviatanGL::secondaryInit(int i){
        if(!paintinMenu2) paintinMenu2 = true;
        postActMenu2 = i;
    }

    void
    LeviatanGL::menuInit(){
        playing = false;
        paintinMenu2 = false;
    }

    void LeviatanGL::setSkybox(std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back)
    {
        smgr->addSkybox(right, left, top, bottom, front, back);
    }
    
    void
    LeviatanGL::changeCharging(){
        if(charging){
            charging = false;
        }
        else{
            charging = true;
        }
    }

    void
    LeviatanGL::chargingFunc(){
        if(charging){
            ImGui::SetNextWindowPos(ImVec2{-6, -6});
            ImGui::SetNextWindowSize(ImVec2{static_cast<float>(width+6), static_cast<float>(height+6)});
            ImGui::Begin("Charge",NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
            ImGui::Image((void*)(intptr_t) charge->getImageId(), ImVec2(charge->getImageWidth(), charge->getImageHeight()));
            ImGui::End();
        }
    }

    void
    LeviatanGL::clearKeyHUD(int id){
        gotKey = false;
    }
    
    void
    LeviatanGL::keyHUD(){
        gotKey = true;
    }
}