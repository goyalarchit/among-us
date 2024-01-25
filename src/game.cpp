
#include "game.h"
#include "resource_manager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


TextRenderer  *Text;
int r,c;

Game::Game(unsigned int width, unsigned int height,int block_size) 
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height), Block_size(block_size)
{ 

}

Game::~Game()
{
    
}

void Game::Init()
{
    this->time = 300;
    this->health = 100;
    this->light = true;
    this-> task[0]=0;
    this-> task[1]=2;
    ResourceManager::LoadShader("../assets/shaders/maze.vert", "../assets/shaders/maze.frag", nullptr, "maze");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height),0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("maze").Use().SetMatrix4("projection", projection);
    ResourceManager::LoadShader("../assets/shaders/player.vert", "../assets/shaders/player.frag", nullptr, "player");
    ResourceManager::GetShader("player").Use().SetMatrix4("projection", projection);
    r = (this->Height-2*Block_size)/this->Block_size;
    c = this->Width/this->Block_size;
    m = new maze(r,c);
    m->gen_maze(nullptr);
    m->show(20);
    m->init_draw(this->Block_size);
    p = new Player(0,0);
    p->init_draw(this->Block_size);
    e = new Player(rand()%r,rand()%c);
    e->init_draw(this->Block_size);
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../assets/fonts/NewTegomin-Regular.ttf", 20);
    v_btn = new GameObject(BUTTON,ACTIVE,r,c);
    p_btn = new GameObject(BUTTON,ACTIVE,r,c);

}

void Game::Update(float dt)
{
    if(this->State == GAME_ACTIVE)
    {
        this->time-=dt;
        if(this->time<=0)
            this->State = GAME_LOST;
        // if(e!=nullptr && p!=nullptr)
        //     e->move(m->shortest_path(e->get_x(),e->get_y(),p->get_x(),p->get_y()));
        if((e!= nullptr && p->get_x()==e->get_x()) && (p->get_y()==e->get_y()))
        {
            this->State = GAME_LOST;

        }
    }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        // float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_LEFT])
        {
            if(m->detect_collision(p->get_x(),p->get_y(),'l')==false)
                p->move('l');
        }
        if (this->Keys[GLFW_KEY_RIGHT])
        {
            if(m->detect_collision(p->get_x(),p->get_y(),'r')==false)
                p->move('r');
        }
        if (this->Keys[GLFW_KEY_UP])
        {
            if(m->detect_collision(p->get_x(),p->get_y(),'t')==false)
                p->move('t');
        }
        if (this->Keys[GLFW_KEY_DOWN])
        {
            if(m->detect_collision(p->get_x(),p->get_y(),'b')==false)
                    p->move('b');        
        }
        if(v_btn->button_found(p->get_x(),p->get_y()))
        {
            delete e;
            e = nullptr;
            this->task[0]++;
        }
        if(p_btn->button_found(p->get_x(),p->get_y()))
        {
            this->task[0]++;
            int n = (rand()%r+c)+1;
            GameObject *spawnedObj;
            for (int i=0;i<n;i++)
            {
                if((rand()%2)==0)
                {   spawnedObj = new GameObject(COIN,ACTIVE,r,c);}
                else
                {   spawnedObj = new GameObject(BOMB,ACTIVE,r,c);}
                this->SpawnedObjects.push_back(spawnedObj);
            }
        }
        for(auto obj:this->SpawnedObjects)
            this->AddReward(obj->get_reward(p->get_x(),p->get_y()));
        if(p->get_x()==r-1 && p->get_y()==c-1 && task[0]==2)
            this->State=GAME_WON;
    }
   
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE)
    {
        Shader s = ResourceManager::GetShader("maze");
        m->draw(s);
        Shader s2 = ResourceManager::GetShader("player");
        p->draw(s2,this->Block_size);
        if(e!=nullptr)
            e->draw(s2,this->Block_size);
        this->RenderHUD(Text);
        v_btn->draw(Text,this->Block_size);
        p_btn->draw(Text,this->Block_size);
        this->RenderSpawnedObj(Text);
    }
    else if (this->State == GAME_WON)
    {
        Text->RenderText("YOU WON",(float) r*Block_size/2, c*Block_size/2, 2.0f, glm::vec3(0.0f,1.0f,0.0f));
    }
    else if (this->State == GAME_LOST)
    {
        Text->RenderText("YOU LOST",(float) r*Block_size/2, c*Block_size/2, 2.0f, glm::vec3(1.0f,0.0f,0.0f));
    }
}

void Game::RenderHUD(TextRenderer *t)
{
    std::vector<std::string> labels{"HEALTH","LIGHT","TASKS","TIME"};
    std::vector<std::string> values;
    values.push_back(std::to_string(this->health));
    values.push_back((this->light?"ON":"OFF"));
    values.push_back(std::to_string(this->task[0])+"/"+std::to_string(this->task[1]));
    values.push_back(std::to_string( (int)this->time ));
    int offset = (this->Width-Block_size)/labels.size();
    for(int i=0;i<labels.size();i++ )
        t->RenderText(labels[i],(float) Block_size+i*offset, this->Height-2*Block_size, 1.0f, glm::vec3(1.0f,0.0f,0.0f));
    for(int i=0;i<values.size();i++ )
        t->RenderText(values[i],(float) Block_size+i*offset, this->Height-Block_size, 1.0f, glm::vec3(1.0f,1.0f,1.0f));
    
}

void Game::RenderSpawnedObj(TextRenderer *t)
{
    for(auto obj: this->SpawnedObjects)
        obj->draw(t,this->Block_size);
}

void Game::AddReward(int r)
{
    this->health+=r;
    if(this->health>100)
        this->health=100;
    else if (this->health<=0)
    {
        this->health =0;
        this->State=GAME_LOST;
    }
}
