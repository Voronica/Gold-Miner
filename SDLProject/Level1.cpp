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
#define LEVEL1_ENEMY_COUNT 3
#define LEVEL1_MINES_COUNT 3



void Level1::Initialize() {
    
    state.nexScene = -1;
    
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
        state.mines[i].position = glm::vec3(2*i-1, -1, 0);
        state.mines[i].weight = 3;
        state.mines[i].value = 100;
        
    }
    
    
    
    
}

#define FIXED_TIMESTEP 0.0166666f
void Level1::Update(float deltaTime) {
    
    state.hook->Update(deltaTime, state.hook, state.enemies, LEVEL1_ENEMY_COUNT, state.mines, LEVEL1_MINES_COUNT);
    state.mineCart->Update(deltaTime, state.hook, state.enemies, LEVEL1_ENEMY_COUNT, state.mines, LEVEL1_MINES_COUNT);
    for (int i = 0; i < LEVEL1_MINES_COUNT; i++){
        state.mines[i].Update(deltaTime, state.hook, state.enemies, LEVEL1_ENEMY_COUNT, state.mines, LEVEL1_MINES_COUNT);
    }
    
    
    
    std::cout << state.hook->position.x << std::endl;
    std::cout << state.hook->position.y << std::endl;
    std::cout << state.hook->keepMoving << std::endl;
    
}



void Level1::Render(ShaderProgram *program) {
    state.mineCart->Render(program);
    state.hook->Render(program);
    for (int i = 0; i < LEVEL1_MINES_COUNT; i++){
        state.mines[i].Render(program);
    }
    
    
    

    //std::cout << "Press ENTER or go through START" << std::endl;
}


