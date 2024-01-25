#pragma once
#include<vector>
#include<iostream>
#include<string>
#include "shader.h"

class cell {
    private:
        int x,y;
        // top,right,bottom,left
        bool walls[4];


    public:
        bool visited; 
        cell(int x,int y);
        int get_x();
        int get_y();
        void print();
        void removeWall(char);
        std::string show(int); 
        std::vector<float> get_wall_coordinates(int);
        bool isWalled(char);

};

class maze {
    int rows, cols;
    std::vector<cell * > grid;
    float * coordinates;
    int length_coords;
    unsigned int VBO, VAO;

    private:
        cell * checkNeighbours(cell *);
        int index(int, int);
        void removeWalls(cell *,cell *);
        int distance_between(int,int,int,int);
    public:
        maze(int r,int c);
        void gen_maze(cell *);
        void show(int);
        void draw(Shader &shader);
        void get_coordinates(int);
        void init_draw(int);
        bool detect_collision(int,int,char);
        char shortest_path(int,int,int,int);

};
