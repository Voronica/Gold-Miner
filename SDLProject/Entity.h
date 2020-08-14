#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

//include other header file here
//TODO: need Map.h?


enum EntityType{HOOK, ENEMY, MINE, MINE_CART, LIFE};


class Entity {
public:
    EntityType entityType;
    
    glm::vec3 position;
    glm::vec3 velocity;
    
    
    float speed;
    float width = 1;
    float height = 1;
    float number;
    std::string entityName = "";
    
    float weight = 1;//1 is default weight, >1 pulls slower, <1 pulls faster. cannot be zero
    float value;
    float hookValue;
    
    GLuint textureID;
    glm::mat4 modelMatrix;
    
    bool isActive = true;
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    bool collideEnemy = false;
    bool keepMoving = true;
    bool loaded = false;
    bool loseLife = false;
    bool passLevel = false;
    
    Entity();
    
    bool subCheckCollision(Entity *other);
    Entity* CheckCollision(Entity* other);
        
    Entity* CheckCollisionsY(Entity *objects, int objectCount);
    Entity* CheckCollisionsX(Entity *objects, int objectCount);

    
    void AI(Entity *hook, Entity *mines, int minesCount);
    void MineBehavior(Entity *other);

    void Update(float deltaTime, Entity *player, Entity *enemies, int enemiesCount, Entity *mines, int minesCount);
    
    
    void Render(ShaderProgram *program);
    void Render_Enemy(ShaderProgram *program);
    void Render_Life(ShaderProgram *program);
    void Render_Gold1(ShaderProgram *program);
    void Render_Gold2(ShaderProgram *program);
    void Render_Stone(ShaderProgram *program);

};
