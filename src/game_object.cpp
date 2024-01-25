#include "game_object.h"
#include <iostream>


GameObject::GameObject(ObjectType t,ObjectState s,int r,int c)
{
    this->state = s;
    this->type=t;
    switch(type)
    {
        case BUTTON :   this->reward = 0;
                        break;
        case BOMB :     this->reward = -((rand()%5)+1);
                        break;
        case COIN :     this->reward = ((rand()%5)+1);
                        break;
    }
    this->x = rand()%r;
    this->y = rand()%c;
    if(this->x == 0)
        this->x=1;
    if(this->y == 0)
        this->y=1;
    std::cout<<"Reward: "<<reward;
}

void GameObject::draw(TextRenderer *t,int w)
{
    if (state == ACTIVE)
    {
        switch(this->type)
        {
            case BUTTON :   t->RenderText("b",(float) x*w, y*w, 1.0f, glm::vec3(0.0f,0.0f,1.0f));
                            break;
            case BOMB :   t->RenderText("O",(float) x*w, y*w, 1.0f, glm::vec3(0.0f,0.0f,1.0f));
                            break;
            case COIN :   t->RenderText("C",(float) x*w, y*w, 1.0f, glm::vec3(0.0f,0.0f,1.0f));
                            break;
        }
    }
    else 
    {
        if(this->type==BUTTON)
            t->RenderText("p",(float) x*w, y*w, 1.0f, glm::vec3(0.0f,0.0f,1.0f));
    }
}

int GameObject::get_reward(int i,int j)
{
    if(this->state == ACTIVE )
    {
        if(this->x == i && this->y == j)
        {
            this->state=DEACTIVE;
            return this->reward;
        }
    }
    return 0;
}
bool GameObject::button_found(int i,int j)
{
    if(this->state == ACTIVE && this->type == BUTTON)
    {
        if(this->x == i && this->y == j)
        {
            this->state=DEACTIVE;
            return true;
        }
    }
    return false;
}