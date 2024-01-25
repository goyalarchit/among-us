
#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "maze.h"
#include "player.h"
#include "text_renderer.h"
#include "game_object.h"


// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WON,
    GAME_LOST
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // game state
    GameState               State;	
    bool                    Keys[1024];
    unsigned int            Width, Height,Block_size;
    maze *m ;
    Player *p, *e;
    int health,task[2];
    bool light;
    float time;
    GameObject *v_btn,*p_btn;
    std::vector<GameObject *> SpawnedObjects; 
    // constructor/destructor
    Game(unsigned int width, unsigned int height,int block_size);
    ~Game();
    // initialize game state (load all shaders/textures/levels)
    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void RenderHUD(TextRenderer*);
    void AddReward(int);
    void RenderSpawnedObj(TextRenderer *);

};

#endif