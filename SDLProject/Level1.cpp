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
    
    
    //Initialize MineCart
    state.mineCart = new Entity();
    state.mineCart->position = glm::vec3(0, 2, 0);
    state.mineCart->textureID = Util::LoadTexture("mineCart.jpg");
    
     
    
}

#define FIXED_TIMESTEP 0.0166666f
void Level1::Update(float deltaTime) {
    
    
    state.mineCart->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.mines, LEVEL1_MINES_COUNT);
    
    
    
    
}



void Level1::Render(ShaderProgram *program) {
    
    

    //std::cout << "Press ENTER or go through START" << std::endl;
}


