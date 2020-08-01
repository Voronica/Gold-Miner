//
//  Level1.cpp
//  GoldMiner
//
//  Created by Shucong Zhou on 2020/7/31.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Level1.h"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 1
#define LEVEL1_MINES_COUNT 3
#define TARGET_SCORE 200

GLuint fontTextureID_1;

void Level1::Initialize() {
    
    state.nextScene = -1;
    
    //Initiaize Hook - the player - control position
    state.hook = new Entity();
    state.hook->textureID = Util::LoadTexture("hook.png");
    state.hook->entityType = HOOK;
    state.hook->position = glm::vec3(0, 2, 0);
    state.hook->speed = 2;
    //Initialize MineCart
    state.mineCart = new Entity();
    state.mineCart->textureID = Util::LoadTexture("mineCart.png");
    state.mineCart->entityType = MINE_CART;
    state.mineCart->position = glm::vec3(state.hook->position.x+0.1, 3, 0);
    
    //Mines
    GLuint gold1TextureID = Util::LoadTexture("gold1.png");
    state.mines = new Entity[LEVEL1_MINES_COUNT];
    for (int i = 0; i < LEVEL1_MINES_COUNT; i++){
        state.mines[i].entityType = MINE;
        state.mines[i].textureID = gold1TextureID;
        state.mines[i].position = glm::vec3(2*i-1, 0.5*(2-i)-1, 0);
        state.mines[i].weight = 3;
        state.mines[i].value = 100;
        
    }
    GLuint pigTextureID = Util::LoadTexture("pig.png");
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = pigTextureID;
        state.enemies[i].position = glm::vec3(-6,-1,0);
        
    }
    
     //Initialize life
    state.lives = new Entity();
    state.lives->entityType = LIFE;
    state.lives->position = glm::vec3(3.7, 3.3, 0);
    state.lives->textureID = Util::LoadTexture("hook.png");
    state.lives->number = 3;
    
    //Initialize the text
    fontTextureID_1 = Util::LoadTexture("font2.png");
    
    
}

#define FIXED_TIMESTEP 0.0166666f
void Level1::Update(float deltaTime) {
    state.hook->Update(deltaTime, state.hook, state.enemies, LEVEL1_ENEMY_COUNT, state.mines, LEVEL1_MINES_COUNT);
    state.mineCart->Update(deltaTime, state.hook, state.enemies, LEVEL1_ENEMY_COUNT, state.mines, LEVEL1_MINES_COUNT);
    state.lives->Update(deltaTime, state.hook, state.enemies, LEVEL1_ENEMY_COUNT, state.mines, LEVEL1_MINES_COUNT);
    
    for (int i = 0; i < LEVEL1_MINES_COUNT; i++){
        state.mines[i].Update(deltaTime, state.hook, state.enemies, LEVEL1_ENEMY_COUNT, state.mines, LEVEL1_MINES_COUNT);
    }
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].Update(deltaTime, state.hook, state.enemies, LEVEL1_ENEMY_COUNT, state.mines, LEVEL1_MINES_COUNT);
    }
    //add Score functionality;
    if (state.hook->keepMoving == true){
        state.score += state.hook->hookValue;
    }
    state.hook->hookValue = 0;
    //switch scene condition
    if (state.score > TARGET_SCORE){
        state.nextScene = 2;
    }
   
    
    std::cout << state.lives->position.x << std::endl;
    std::cout << state.lives->position.y << std::endl;
    
}



void Level1::Render(ShaderProgram *program) {
    state.mineCart->Render(program);
    state.hook->Render(program);
    state.lives->Render_Life(program);
    
    for (int i = 0; i < LEVEL1_MINES_COUNT; i++){
        state.mines[i].Render(program);
    }
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].Render(program);
    }
    
    
    Util::DrawText(program, fontTextureID_1, "Score: " + std::to_string(state.score) , 0.25f, -0.1f, glm::vec3(-4.5, 3.4, 0));
    Util::DrawText(program, fontTextureID_1, "Target Score: " + std::to_string(TARGET_SCORE) , 0.25f, -0.1f, glm::vec3(-4.5, 3.1, 0));

    //std::cout << "Press ENTER or go through START" << std::endl;
}


