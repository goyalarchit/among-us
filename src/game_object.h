#pragma once

#include "text_renderer.h"

enum ObjectType {
    BUTTON,
    BOMB,
    COIN
};

enum ObjectState {
    ACTIVE,
    DEACTIVE
};

class GameObject{
    ObjectType type;
    int x,y,reward;
    ObjectState state;
    public:
        GameObject(ObjectType,ObjectState,int,int);
        void draw(TextRenderer *,int);
        int get_reward(int,int);
        bool button_found(int,int);
};