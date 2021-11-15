#pragma once

#include "../../manager/ResourceMesh.hpp"

namespace LVGL{

    class LVAnimation {
    public:
    LVAnimation(int id, std::vector<ResourceMesh *> kf, bool l) : ID{id}, keyframes{kf}, loop{l} {};
    
    int ID;
    std::vector<ResourceMesh *> keyframes;
    bool loop;
    std::vector<uint8_t> framesBetweenKeyframes;
    
    };

}