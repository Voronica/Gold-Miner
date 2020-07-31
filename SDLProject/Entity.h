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


enum EntityType{PLAYER, ENEMY, MINE, MINE_CART};


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
    
<<<<<<< HEAD
    bool CheckCollision(Entity *other);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void CheckCollisionsX(Entity *objects, int objectCount);
    
=======
    bool subCheckCollision(Entity *other);
    Entity* CheckCollision(Entity* other);
        
    Entity* CheckCollisionsY(Entity *objects, int objectCount);
    Entity* CheckCollisionsX(Entity *objects, int objectCount);
>>>>>>> bb50928d09c738060bd1d601d060b3e058327b77
    
    void AI(Entity *player, Entity *mines, int minesCount);
    void MineBehavior(Entity *player, Entity* enemies, int enemiesCount);
    
<<<<<<< HEAD
    void Update(float deltaTime, Entity *player, Entity *object, int objectCount);
=======
    void Update(float deltaTime, Entity *player, Entity *enemies, int enemiesCount, Entity *mines, int minesCount);
>>>>>>> bb50928d09c738060bd1d601d060b3e058327b77
    void Render(ShaderProgram *program);

};
