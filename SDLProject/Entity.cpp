//  Entity.cpp
//  GoldMiner
//
//  Created by Kewei Zhang on 7/30/20.
//  Copyright © 2020 ctg. All rights reserved.

#include "Entity.h"

Entity::Entity(){
    position = glm::vec3(0);
    velocity = glm::vec3(0);
    weight = 0;
    value = 0;
    speed = 0;
    modelMatrix = glm::mat4(1.0f);
}

Entity* Entity::CheckCollision(Entity* other){
    if (this->entityType == ENEMY && loaded == true) return nullptr;//for enemy not picking up second mine
    if (isActive == false || other->isActive == false) return nullptr;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width)/2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height)/2.0f);
    
    if (xdist < 0  && ydist < 0){
        return other;
    }
    return nullptr;
}

bool Entity::subCheckCollision(Entity *other){
    if (isActive == false || other->isActive == false) return false;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width)/2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height)/2.0f);
    
    if (xdist < 0  && ydist < 0){
        return true;
    }
    return false;
}

Entity* Entity::CheckCollisionsY(Entity *objects, int objectCount){
    Entity* result = nullptr;
    for (int i =0; i < objectCount; i++){
        Entity *object = &objects[i];
        if (subCheckCollision(object)){
            //calculate penetration
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - ((height + object->height)/2.0f));
            //reset penetraion
            if (velocity.y > 0){
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                collideEnemy = true;
            }
            else if (velocity.y <0){
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                collideEnemy = true;
            }
            result = object;
        }
    }
    return result;
}

Entity* Entity::CheckCollisionsX(Entity *objects, int objectCount){
    Entity* result = nullptr;
    
    for (int i =0; i < objectCount; i++){
        Entity *object = &objects[i];
        if (subCheckCollision(object)){
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - ((width + object->width)/2.0f));
            if (velocity.x > 0){
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
                collideEnemy = true;
            }
            else if (velocity.x <0){
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                collideEnemy = true;
            }
            result = object;
        }
    }
    return result;
}


void Entity::AI(Entity *hook, Entity *mines, int minesCount){
    //TODO: touch player -1 lives/touch mine, move mine together
    velocity.x = 1;
    Entity *theStolen = nullptr;
    for (int i = 0; i < minesCount; i++) {
        theStolen = CheckCollision(&mines[i]);
        if (theStolen != nullptr && theStolen->entityName != "mineOnPig") {
            theStolen->isActive = false; //when enemy hit mine, diable mine
            Mix_PlayChannel(-1, Mix_LoadWAV("hit3.wav"), 0);
            loaded = true;
            break;
        }
    }
}

void Entity::MineBehavior(Entity *other){
    this->position = other->position;
}

void Entity::Update(float deltaTime, Entity *player, Entity *enemies, int enemiesCount, Entity *mines, int minesCount){
    if (isActive == false) return;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    if (entityType == HOOK){
        Entity *theCollided = nullptr;
        float loadWeight = 1;
        for (int i = 0; i < minesCount; i++) {
            theCollided= CheckCollision(&mines[i]);
            if (theCollided != nullptr){
                theCollided->MineBehavior(player);
                hookValue = theCollided->value;//pass the value to the hook, to be able to access from state.
                loadWeight = theCollided->weight;
                loaded = true;
                break;
            }
        }
        
        Entity *theCollided_Ememy = nullptr;
        for (int i = 0; i < enemiesCount; i++) {
            theCollided_Ememy = CheckCollision(&enemies[i]);
            if (theCollided_Ememy != nullptr) {
                this->loseLife = true;
                Mix_PlayChannel(-1, Mix_LoadWAV("hit2.wav"), 0);
            }
        }
        
        position.y += velocity.y * speed / loadWeight * deltaTime;
        position.x += velocity.x * speed * deltaTime;
        
        
        if(position.y < -3.25f || loaded){//hook return condition
            velocity.y = 1;
        }
        if (position.y >2){
            velocity.y = 0;
            position = glm::vec3(position.x, 2, position.z);
            keepMoving = true;
            if (loaded){
                theCollided->isActive = false;
                //play sound
                if (hookValue == 0){
                    Mix_PlayChannel(-1, Mix_LoadWAV("rock.wav"), 0);
                }
                else if (hookValue == 30 || hookValue == 200){
                    Mix_PlayChannel(-1, Mix_LoadWAV("gold.wav"), 0);
                }
                else if (hookValue == 500 || hookValue == 700){
                    Mix_PlayChannel(-1, Mix_LoadWAV("diamond.wav"), 0);
                }
            }
            loaded = false;
             //TODO: diable mine, track score.
        }
        if (position.x <-4.6){
            velocity.x = 0;
            position.x = -4.6;
        }
        else if (position.x > 4.4){
            velocity.x = 0;
            position.x = 4.4;
        }
    }
    
    else if (entityType == ENEMY){
        AI(player, mines, minesCount);
        
        if (this->entityName == "pig") {
            position.y += velocity.y * speed * deltaTime;
            position.x += velocity.x * speed * deltaTime;
        }
        
        if (this->entityName == "pig_reversed") {
           position.y += velocity.y * speed * deltaTime;
           position.x += velocity.x * speed * deltaTime * (-1);
        }
    
    }

    else if (entityType == MINE){
        if (this->entityName == "mineOnPig") {
            velocity.x = 1;
            position.y += velocity.y * speed * deltaTime;
            position.x += velocity.x * speed * deltaTime;
            if (this->position.x > 6) this->isActive = false;
        }
    }
    
    else if (entityType == MINE_CART){
        position.x = player->position.x + 0.1;
    }
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    velocity.x = 0;
}

void Entity::Render(ShaderProgram *program) {
    if (isActive == false) return;
    
    program->SetModelMatrix(modelMatrix);
    
    //float vertices[]  = { -3.2, -2.0, 3.2, -2.0, 3.2, 2.0, -3.2, -2.0, 3.2, 2.0, -3.2, 2.0 };
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render_Enemy(ShaderProgram *program) {
    if (isActive == false) return;
    
    program->SetModelMatrix(modelMatrix);
    
    
    float vertices[]  = { -0.5, -0.4, 0.5, -0.4, 0.5, 0.4, -0.5, -0.4, 0.5, 0.4, -0.5, 0.4 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render_Life(ShaderProgram *program) {
    if (isActive == false) return;
    
    program->SetModelMatrix(modelMatrix);
    
   
    float vertices[]  = { -0.3, -0.2, 0.3, -0.2, 0.3, 0.2, -0.3, -0.2, 0.3, 0.2, -0.3, 0.2 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render_Gold1(ShaderProgram *program) {
    if (isActive == false) return;
    
    program->SetModelMatrix(modelMatrix);
    

    float vertices[]  = { -0.2, -0.2, 0.2, -0.2, 0.2, 0.2, -0.2, -0.2, 0.2, 0.2, -0.2, 0.2};
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render_Gold2(ShaderProgram *program) {
    if (isActive == false) return;
    
    program->SetModelMatrix(modelMatrix);
    
    
    float vertices[]  = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render_Stone(ShaderProgram *program) {
    if (isActive == false) return;
    
    program->SetModelMatrix(modelMatrix);
    
    float vertices[]  = { -0.5, -0.4, 0.5, -0.4, 0.5, 0.4, -0.5, -0.4, 0.5, 0.4, -0.5, 0.4 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
