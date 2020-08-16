//
//  Level2.cpp
//  GoldMiner
//
//  Created by Kewei Zhang on 8/3/20.
//  Copyright © 2020 ctg. All rights reserved.
//

#include "Level2.h"

#define LEVEL2_ENEMY_COUNT 2//TODO: Decide number
#define LEVEL2_MINES_COUNT 7
#define TARGET_SCORE 600

GLuint fontTextureID_2;
GLuint bgTextureID_2;

glm::mat4 backgroundMat_2;

void Level2::Initialize() {
    
    state.nextScene = -1;
    state.score = 0;
    bgTextureID_2 = Util::LoadTexture("background.png");
    backgroundMat_2 = glm::mat4(1.0f);
    backgroundMat_2 = glm::translate(backgroundMat_2, glm::vec3(0,-0.2,0));
    backgroundMat_2 = glm::scale(backgroundMat_2, glm::vec3(10,8,1));
    
    
    //Initiaize Hook/player - default position
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

    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].entityName = "pig";
    state.enemies[0].textureID = Util::LoadTexture("pig.png");
    state.enemies[0].position = glm::vec3(-10,-1,0);//TODO: design enemy position
    state.enemies[0].speed = 1;
    
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].entityName = "pig_reversed";
    state.enemies[1].textureID = Util::LoadTexture("pig_reverse.png");
    state.enemies[1].position = glm::vec3(10,1,0);//TODO: design enemy position
    state.enemies[1].speed = 1;
    
    
    
    
    //Mines
    state.mines = new Entity[LEVEL2_MINES_COUNT];
    // ------------------------------------
    //Initialize gold(type1) - value 30
    
    state.mines[0].entityType = MINE;
    state.mines[0].position = glm::vec3(2, 0, 0);
    state.mines[0].weight = 1;
    state.mines[0].value= 30;
    state.mines[0].textureID = Util::LoadTexture("gold1.png");

    state.mines[1].entityType = MINE;
    state.mines[1].position = glm::vec3(-2, 0, 0);
    state.mines[1].weight = 1;
    state.mines[1].value= 30;
    state.mines[1].textureID = Util::LoadTexture("gold2.png");
    
    state.mines[2].entityType = MINE;
    state.mines[2].position = glm::vec3(-1.5f, 1, 0);
    state.mines[2].weight = 1;
    state.mines[2].value= 30;
    state.mines[2].textureID = Util::LoadTexture("gold3.png");
    
    // ------------------------------------
    //Initialize gold(type2) - value 200
    
    state.mines[3].entityType = MINE;
    state.mines[3].position = glm::vec3(3.0f, -1, 0);
    state.mines[3].weight = 2;
    state.mines[3].value= 200;
    state.mines[3].textureID = Util::LoadTexture("gold4.png");
    
    state.mines[5].entityType = MINE;
    state.mines[5].position = glm::vec3(3.5f, -3, 0);
    state.mines[5].weight = 2;
    state.mines[5].value= 200;
    state.mines[5].textureID = Util::LoadTexture("gold1.png");
    
    // ------------------------------------
    //Initialize stone - value 0
    
    state.mines[4].entityType = MINE;
    state.mines[4].position = glm::vec3(0.5f, 0, 0);
    state.mines[4].weight = 3;
    state.mines[4].value= 0;
    state.mines[4].textureID = Util::LoadTexture("stone.png");
    
    // ------------------------------------
    //Initialize diamond - value 500
    state.mines[6].entityType = MINE;
    state.mines[6].position = glm::vec3(0.5f, -3.5, 0);
    state.mines[6].weight = 2;
    state.mines[6].value= 500;
    state.mines[6].textureID = Util::LoadTexture("diamond.png");
   
     //Initialize life
    state.lives = new Entity();
    state.lives->entityType = LIFE;
    state.lives->position = glm::vec3(3.7, 3.3, 0);
    state.lives->textureID = Util::LoadTexture("hook.png");
    state.lives->number = 3;
    
    //Initialize the text
    fontTextureID_2 = Util::LoadTexture("font2.png");
    
    scenceLevel = 2;
    
    
}

#define FIXED_TIMESTEP 0.0166666f
void Level2::Update(float deltaTime) {
    state.hook->Update(deltaTime, state.hook, state.enemies, LEVEL2_ENEMY_COUNT, state.mines, LEVEL2_MINES_COUNT);
    state.mineCart->Update(deltaTime, state.hook, state.enemies, LEVEL2_ENEMY_COUNT, state.mines, LEVEL2_MINES_COUNT);
    state.lives->Update(deltaTime, state.hook, state.enemies, LEVEL2_ENEMY_COUNT, state.mines, LEVEL2_MINES_COUNT);
    
    for (int i = 0; i < LEVEL2_MINES_COUNT; i++){
        state.mines[i].Update(deltaTime, state.hook, state.enemies, LEVEL2_ENEMY_COUNT, state.mines, LEVEL2_MINES_COUNT);
    }
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++){
        state.enemies[i].Update(deltaTime, state.hook, state.enemies, LEVEL2_ENEMY_COUNT, state.mines, LEVEL2_MINES_COUNT);
    }
    //add Score functionality;
    if (state.hook->keepMoving == true){
        state.score += state.hook->hookValue;
    }
    state.hook->hookValue = 0;
    
    //Lose Condition - all mines disppear but not reach the target score
    int inActive = 0;
    for (int i = 0; i < LEVEL2_MINES_COUNT; i++) {
        if (!state.mines[i].isActive) inActive += 1;
    }
    if (inActive == LEVEL2_MINES_COUNT && state.score < TARGET_SCORE) {
        std::cout << "Lose One Life" << std::endl;
        loseLife = true;
        state.nextScene = 2;
        state.score = 0;
    }
    //touch enemy
    if(state.hook->loseLife) {
        std::cout << "Lose One Life" << std::endl;
        loseLife = true;
        state.nextScene = 2;
        state.score = 0;
    }
    
    //switch scene condition(level win)
    if (state.score >= TARGET_SCORE){
        std::cout << "Pass Level2" << std::endl;
        //Mix_PlayChannel(-1, Mix_LoadWAV("pass.wav"), 0);
        state.passLevel = true;
        state.nextScene = 3;
    }
}



void Level2::Render(ShaderProgram *program) {
    glClear(GL_COLOR_BUFFER_BIT);
    
    float vertices[] = {-0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5};
    float texCoords[] = {0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0};
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->positionAttribute);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    //draw all my objects here
    program->SetModelMatrix(backgroundMat_2);
    glBindTexture(GL_TEXTURE_2D, bgTextureID_2);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
    
    state.mineCart->Render(program);
    state.hook->Render(program);
    state.lives->Render_Life(program);
    
    for (int i = 0; i < 3; i++){
        state.mines[i].Render_Gold1(program);
    }
    state.mines[6].Render_Gold1(program);
    
    state.mines[3].Render_Gold2(program);
    state.mines[4].Render_Stone(program);
    state.mines[5].Render_Gold2(program);
    
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++){
        state.enemies[i].Render_Enemy(program);
    }
    
    Util::DrawText(program, fontTextureID_2, "LEVEL 2", 0.5f, -0.2f, glm::vec3(-0.75,3.35,0));
    Util::DrawText(program, fontTextureID_2, "Score: " + std::to_string(state.score) , 0.25f, -0.1f, glm::vec3(-4.5, 3.4, 0));
    Util::DrawText(program, fontTextureID_2, "Target Score: " + std::to_string(TARGET_SCORE) , 0.25f, -0.1f, glm::vec3(-4.5, 3.1, 0));

    //std::cout << "Press ENTER or go through START" << std::endl;
}

