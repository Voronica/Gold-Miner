#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

//include other header file here
//TODO: need Map.h?


enum EntityType{PLAYER, ENEMY, MINE};


class Entity {
public:
    EntityType entityType;
    
    glm::vec3 position;
    glm::vec3 velocity;
    
    
    float speed;
    
    float width = 1;
    float height = 1;
    
    float weight;
    float value;
    
    
    
    
    GLuint textureID;
    glm::mat4 modelMatrix;
    
    
    bool isActive = true;
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    
    Entity();
    
    bool subCheckCollision(Entity *other);
    Entity* CheckCollision(Entity* other);
        
    Entity* CheckCollisionsY(Entity *objects, int objectCount);
    Entity* CheckCollisionsX(Entity *objects, int objectCount);
    
    void AI(Entity *player, Entity *mines, int minesCount);
    void MineBehavior(Entity *player, Entity* enemies, int enemiesCount);
    
    void Update(float deltaTime, Entity *player, Entity *enemies, int enemiesCount, Entity *mines, int minesCount);
    void Render(ShaderProgram *program);

};
