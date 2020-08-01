
//  Menu.cpp
//  SDLProject
//
//  Created by Shucong Zhou on 2020/7/16.
//  Copyright © 2020 ctg. All rights reserved.
//


#include "Menu.h"
#include "Entity.h"

#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 3

GLuint fontTextureID_Menu;


void Menu::Initialize() {
    
    state.nextScene = -1;
    fontTextureID_Menu = Util::LoadTexture("font2.png");
    
    state.hook = new Entity();
    
    
    
     
     
     
    
}

#define FIXED_TIMESTEP 0.0166666f
void Menu::Update(float deltaTime) {
    
    
    
    
    
    
    
}



void Menu::Render(ShaderProgram *program) {
    
    Util::DrawText(program, fontTextureID_Menu, "GOLD MINER!", 0.5, -0.1, glm::vec3(-2,1,0));
    Util::DrawText(program, fontTextureID_Menu, "press ENTER to play", 0.25, -0.1, glm::vec3(-1.5,0,0));
    
    

    //std::cout << "Press ENTER or go through START" << std::endl;
}

