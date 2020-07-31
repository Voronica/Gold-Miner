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


enum EntityType{PLAYER, PLATFORM, ENEMY};


class Entity {
public:
    EntityType entityType;
    
    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;

    
    float width = 1;
    float height = 1;
    
    float speed;
    
    GLuint textureID;
    glm::mat4 modelMatrix;
    
    bool isActive = true;
    bool collidedTop = false;
    bool collidedBottom = false;
    bool collidedLeft = false;
    bool collidedRight = false;
    
    Entity();
    
    bool CheckCollision(Entity *other);
    void CheckCollisionsY(Entity *objects, int objectCount);
    void CheckCollisionsX(Entity *objects, int objectCount);
    void CheckCollisionX(Map *map);
    void CheckCollisionY(Map *map);
    
    void AI(Entity *player);
    
    void Update(float deltaTime, Entity *player, Entity *object, int objectCount, Map *map);
    void Render(ShaderProgram *program);
    
    void DrawSpriteFromTextureAtlas(ShaderProgram *program, GLuint textureID, int index);
    
    
    
};
