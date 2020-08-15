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

#include "Util.h"
#include "Entity.h"
#include "Scene.h"

#include "Menu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"


//include other header files here

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool gameOver = false;
int lives;

GLuint fontTextureID;

ShaderProgram program;
glm::mat4 modelMatrix, viewMatrix, projectionMatrix;

//declare other global var here

Scene *currentScene;
Scene *sceneList[4];

Mix_Music *music;
Mix_Chunk *select2;



void SwitchToScene(Scene *scene) {
    currentScene = scene;
    currentScene->Initialize();
}


void Initialize(){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("Gold Miner", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 960, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
        
#ifdef _WINDOWS
    glewInit();
#endif
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    music = Mix_LoadMUS("Summer Strolls.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME /4 );
    
    
    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    glUseProgram(program.programID);
    
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    //init game objects here
    select2 = Mix_LoadWAV("select2.wav");
    
    //Initialize the text
    fontTextureID = Util::LoadTexture("font2.png");
    
    sceneList[0] = new Menu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    
    SwitchToScene(sceneList[3]);
    
    lives = 3;
    
    
}

void ProcessInput(){
    
    
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        // Move the player left
                        break;
                    case SDLK_RIGHT:
                        // Move the player right
                        break;
                    case SDLK_SPACE:
                        if (currentScene->state.hook->keepMoving) {
                            currentScene->state.hook->velocity.y = -1;
                            currentScene->state.hook->keepMoving = false;
                            Mix_PlayChannel(-1, select2, 0);
                            break;
                        }
                    case SDLK_RETURN:
                        std::cout << "press enter" << std::endl;
                        if (currentScene == sceneList[0]){
                            currentScene->state.nextScene = 1;
                        }
                }
                break; // SDL_KEYDOWNULL
        }
    }
    
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    
    if (currentScene->state.hook->keepMoving) {
        if (keys[SDL_SCANCODE_LEFT]) {
            currentScene->state.hook->velocity.x = -1;
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            currentScene->state.hook->velocity.x = 1;
        }
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update(){
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);
        deltaTime -= FIXED_TIMESTEP;
        
    }
    
    //update life
    
    if (currentScene->loseLife && lives >= 2) {
        lives -= 1;
        currentScene->loseLife = false;
    }
    
    if (currentScene->loseLife && lives == 1) {
        lives = 0;
        gameOver = true;
        //TODO: Add lose screen, restart functionality
    }
    
    accumulator = deltaTime;
    //TODO: Update viewMatrix?
    
}

void Render(){
    glClear(GL_COLOR_BUFFER_BIT);
    
    currentScene->Render(&program);
    
    Util::DrawText(&program, fontTextureID, " x " + std::to_string(lives) , 0.25f, -0.1f, glm::vec3(3.9f, 3.28f, 0));
    
    if (currentScene == sceneList[3]) {
        if (currentScene->state.passLevel) {
            Util::DrawText(&program, fontTextureID, "You Win!" , 0.5f, -0.25f, glm::vec3(-1.0f, 1, 0));
        }
    }
    
    if (gameOver) {
        Util::DrawText(&program, fontTextureID, "You Lose!" , 0.5f, -0.25f, glm::vec3(-1.0f, 1, 0));
    }
    
    SDL_GL_SwapWindow(displayWindow);
    
}
int main(int argc, char* argv[]) {
    
    Initialize();
    
    while (gameIsRunning) {
        
        if (!gameOver) ProcessInput();
        Update();
        
        //Scene switch here
        if (currentScene->state.nextScene > 0){
            SwitchToScene(sceneList[currentScene->state.nextScene]);
        }
        Render();
        
    }
    
    SDL_Quit();
    return 0;
}
