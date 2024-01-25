#pragma once
#include "shader.h"

class Player{
    private:
        int x, y;
        unsigned int VAO,VBO;
        float *coordinates;
        unsigned int *indices; 
        void update_coordinates(int);
    public:
        Player(int x,int y);
        int get_x();
        int get_y();
        void draw(Shader &shader,int);
        void init_draw(int);
        void move(char);
};