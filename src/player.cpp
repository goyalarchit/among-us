#include "player.h"
#include<iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int Player::get_x()
{
    return x;
}

int Player::get_y()
{
    return y;
}






Player::Player(int x,int y)
{
    this->x=x;
    this->y=y;
}

void Player::update_coordinates(int w)
{
    float x_ = 0;
    float y_ = 0;
    coordinates[0] = (float)w/4;
    coordinates[1] = (float)0;
    coordinates[2] = 0.0f;
    coordinates[3] = (float)3*w/4;
    coordinates[4] = (float)0;
    coordinates[5] = 0.0f;
    coordinates[6] = (float)3*w/4;
    coordinates[7] = (float)3*w/4;
    coordinates[8] = 0.0f;
    coordinates[9] = (float)w/4;
    coordinates[10] = (float)3*w/4;
    coordinates[11] = 0.0f;
    // for(int i=0;i<12;i++)
    // {
    //     std::cout<<"\t"<<coordinates[i];
    // }
}

void Player::init_draw(int w)
{
    coordinates= new float[12] ;
    update_coordinates(w);
    indices = new unsigned int[6] {
        0,1,2
        ,2,3,0
    };
    unsigned int buffer;
    glGenBuffers(1,&buffer);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    std::cout<<"\n VAO: "<<VAO <<"\t VBO: "<<VBO<<"\t Size of :"<<sizeof(coordinates);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 4*3*sizeof(float), coordinates, GL_STREAM_DRAW);
    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
    
}

void Player::draw(Shader &shader,int w)
{
    // init_draw(10);
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), glm::vec3((float)x*w, (float)y*w, 0.0f));
    shader.Use().SetMatrix4("model",trans);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);

}


void Player::move(char direction)
{
    switch(direction)
    {
        case 't':   y=y-1;
                    break;
        case 'b':   y=y+1;
                    break;
        case 'l':   x=x-1;
                    break;
        case 'r':   x=x+1;
                    break;
    }
    std::cout<<"\n x: "<<x<<"\t y: "<<y;
    update_coordinates(10);
}