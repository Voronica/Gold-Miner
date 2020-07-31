//  Entity.cpp
//  GoldMiner
//
//  Created by Kewei Zhang on 7/30/20.
//  Copyright © 2020 ctg. All rights reserved.

#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    velocity = glm::vec3(0);
    acceleration = glm::vec3(0);
    
    speed = 0;
    
    modelMatrix = glm::mat4(1.0f);
}
bool Entity::CheckCollision(Entity *other){
    if (isActive == false || other->isActive == false) return false;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width)/2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height)/2.0f);
    
    if (xdist < 0  && ydist < 0){
        return true;
    }
    return false;
}

void Entity::CheckCollisionsY(Entity *objects, int objectCount){
    for (int i =0; i < objectCount; i++){
        Entity *object = &objects[i];
        
        if (CheckCollision(object)){
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - ((height + object->height)/2.0f));
            if (velocity.y > 0){
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
            }
            else if (velocity.y <0){
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity *objects, int objectCount){
    for (int i =0; i < objectCount; i++){
        Entity *object = &objects[i];
        
        if (CheckCollision(object)){
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - ((width + object->width)/2.0f));
            if (velocity.x > 0){
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
            }
            else if (velocity.x <0){
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
            }
        }
    }
    
}







void Entity::AI(Entity *player){
//    switch(aiType){
//        case WALKER:
//            AIWalker();
//            break;
//        case WAITANDGO:
//            AIWaitAndGO(player);
//            break;
//    }
}



void Entity::Update(float deltaTime, Entity *player, Entity *objects, int objectCount){
    if (isActive == false) return;
    
    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    if (entityType == PLAYER){
        
    }
    
    else if (entityType == ENEMY){
        AI(player);
    }
    
    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;
    
    position.y += velocity.y * deltaTime;
    
    CheckCollisionsY(objects, objectCount);//check velocity, set to zero
    
    position.x += velocity.x * deltaTime;

    CheckCollisionsX(objects, objectCount);
    
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}



void Entity::Render(ShaderProgram *program) {
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
