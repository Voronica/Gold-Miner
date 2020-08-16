//
//  Level1.cpp
//  GoldMiner
//
//  Created by Shucong Zhou on 2020/7/31.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Level1.h"
#define LEVEL1_ENEMY_COUNT 1
#define LEVEL1_MINES_COUNT 8
#define TARGET_SCORE 300

GLuint fontTextureID_1;
GLuint bgTextureID_1;

glm::mat4 backgroundMat_1;

void Level1::Initialize() {
    
    state.nextScene = -1;
    state.score = 0;
    bgTextureID_1 = Util::LoadTexture("background.png");
    backgroundMat_1 = glm::mat4(1.0f);
    backgroundMat_1 = glm::translate(backgroundMat_1, glm::vec3(0,-0.2,0));
    backgroundMat_1 = glm::scale(backgroundMat_1, glm::vec3(10,8,1));
    
    
    //Initiaize Hook - the player - control position
    state.hook = new Entity();
    state.hook->textureID = Util::LoadTexture("hook.png");
    state.hook->entityType = HOOK;
    state.hook->position = glm::vec3(0, 2, 0);
    state.hook->speed = 2;
    state.hook->hookValue = 0;
    //Initialize MineCart
    state.mineCart = new Entity();
    state.mineCart->textureID = Util::LoadTexture("mineCart.png");
    state.mineCart->entityType = MINE_CART;
    state.mineCart->position = glm::vec3(state.hook->position.x+0.1, 3, 0);
    
    //Enemies
    GLuint pigTextureID = Util::LoadTexture("pig.png");
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].entityType = ENEMY;
        state.enemies[i].textureID = pigTextureID;
        state.enemies[i].position = glm::vec3(-6,-1,0);
        state.enemies[i].speed = 0.8;
        state.enemies[i].entityName = "pig";
    }
    
    //Mines
    state.mines = new Entity[LEVEL1_MINES_COUNT];
    GLuint goldID1 = Util::LoadTexture("gold1.png");
    GLuint goldID2 = Util::LoadTexture("gold2.png");
    GLuint goldID3 = Util::LoadTexture("gold3.png");
    GLuint goldID4 = Util::LoadTexture("gold4.png");
    
    // ------------------------------------
    //Initialize gold(type1) - value 30
    
    state.mines[0].entityType = MINE;
    state.mines[0].position = glm::vec3(2, 0, 0);
    state.mines[0].weight = 1;
    state.mines[0].value= 30;
    state.mines[0].textureID = goldID1;

    state.mines[1].entityType = MINE;
    state.mines[1].position = glm::vec3(-2, 0, 0);
    state.mines[1].weight = 1;
    state.mines[1].value= 30;
    state.mines[1].textureID = goldID2;
    
    state.mines[2].entityType = MINE;
    state.mines[2].position = glm::vec3(-1.5f, 1, 0);
    state.mines[2].weight = 1;
    state.mines[2].value= 30;
    state.mines[2].textureID = goldID1;
    
    state.mines[5].entityType = MINE;
    state.mines[5].position = glm::vec3(-2.2f, -2, 0);
    state.mines[5].weight = 1;
    state.mines[5].value= 30;
    state.mines[5].textureID = goldID2;
    
    state.mines[6].entityType = MINE;
    state.mines[6].position = glm::vec3(-4.3f, -3.2, 0);
    state.mines[6].weight = 1;
    state.mines[6].value= 30;
    state.mines[6].textureID = goldID3;
    
    state.mines[7].entityType = MINE;
    state.mines[7].position = glm::vec3(-0.5f, -2.8, 0);
    state.mines[7].weight = 1;
    state.mines[7].value= 30;
    state.mines[7].textureID = goldID4;
    
    
    // ------------------------------------
    //Initialize gold(type2) - value 200
    
    state.mines[3].entityType = MINE;
    state.mines[3].position = glm::vec3(3.0f, -1, 0);
    state.mines[3].weight = 2;
    state.mines[3].value= 200;
    state.mines[3].textureID = goldID2;
    
    // ------------------------------------
    //Initialize stone - value 0
    
    state.mines[4].entityType = MINE;
    state.mines[4].position = glm::vec3(-0.3f, -0.0f, 0);
    state.mines[4].weight = 3;
    state.mines[4].value= 0;
    state.mines[4].textureID = Util::LoadTexture("stone.png");
    
   
     //Initialize life
    state.lives = new Entity();
    state.lives->entityType = LIFE;
    state.lives->position = glm::vec3(3.7, 3.3, 0);
    state.lives->textureID = Util::LoadTexture("hook.png");
    state.lives->number = 3;
    
    //Initialize the text
    fontTextureID_1 = Util::LoadTexture("font2.png");
    
    scenceLevel = 1;
    
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
        std::cout << state.enemies[i].position.x << std::endl;
        std::cout << state.enemies[i].position.y << std::endl;
    }
    //add Score functionality;
    if (state.hook->keepMoving == true){
        state.score += state.hook->hookValue;
    }
    state.hook->hookValue = 0;
    
    //Lose Condition - all mines disppear but not reach the target score
    int inActive = 0;
    for (int i = 0; i < LEVEL1_MINES_COUNT; i++) {
        if (!state.mines[i].isActive) inActive += 1;
    }
    if (inActive == LEVEL1_MINES_COUNT && state.score < TARGET_SCORE) {
        Mix_PlayChannel(-1, Mix_LoadWAV("fail.wav"), 0);
        loseLife = true;
        state.nextScene = 1;
        state.score = 0;
    }
    //switch scene condition
    if (state.score >= TARGET_SCORE){
        std::cout << "Pass Level1" << std::endl;
        Mix_PlayChannel(-1, Mix_LoadWAV("pass.wav"), 0);
        state.nextScene = 2;
        state.passLevel = true;
        
    }
    if(state.hook->loseLife) {
        std::cout << "Lose One Life" << std::endl;
        loseLife = true;
        state.nextScene = 1;
        state.score = 0;
    }
}



void Level1::Render(ShaderProgram *program) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->positionAttribute);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    //draw all my objects here
    program->SetModelMatrix(backgroundMat_1);
    glBindTexture(GL_TEXTURE_2D, bgTextureID_1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);

    
    state.mineCart->Render(program);
    state.hook->Render(program);
    state.lives->Render_Life(program);
    
    for (int i = 0; i < 3; i++){
        state.mines[i].Render_Gold1(program);
    }
    for (int i = 5; i < 8; i++){
        state.mines[i].Render_Gold1(program);
    }
    
    state.mines[3].Render_Gold2(program);
    state.mines[4].Render_Stone(program);
    
    
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++){
        state.enemies[i].Render_Enemy(program);
    }
    
    Util::DrawText(program, fontTextureID_1, "LEVEL 1", 0.5f, -0.2f, glm::vec3(-0.75,3.35,0));
    Util::DrawText(program, fontTextureID_1, "Score: " + std::to_string(state.score) , 0.25f, -0.1f, glm::vec3(-4.5, 3.4, 0));
    Util::DrawText(program, fontTextureID_1, "Target Score: " + std::to_string(TARGET_SCORE) , 0.25f, -0.1f, glm::vec3(-4.5, 3.1, 0));

    //std::cout << "Press ENTER or go through START" << std::endl;
}


