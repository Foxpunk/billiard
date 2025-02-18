#pragma once
#include <SDL.h>
#include <vector>
#include "Player.h"
#include "Table.h"
#include "Ball.h"
#include "Physics.h"
#include "Vector2D.h"
#include "Cue.h"

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height, bool fullscreen);
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() const { return isRunning; }

    static Vector2D screenSize;
    static SDL_Event event;

private:
    bool isRunning;
    bool onStartScreen; 
    int ballCount;      
    bool gameOver;      
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    Table* table;
    Player* player;
    Cue* cue;

    std::vector<Ball*> balls;
    std::vector<Pocket> holes;
    std::vector<Wall> edges;
    Ball* cueBall;
    bool queueRespawn = false;
    int mousex, mousey;
    float mouseangle;
    int hitPower = 0;

    void initTable(const char* texturePath);
    void initCue(const char* texturePath);
    void initPlayer();
    void renderStartScreen();
    void renderGameOverScreen(); 

    void drawBalls();
    void drawHoles();
    void drawEdges();
    void initBalls(int count); 
    bool ballsAreMoving() const;
    void addBall(float px, float py, float r, int id, bool isCue, const char* texturePath);
    void addEdge(float px1, float py1, float px2, float py2, float r);
    void addHole(float px, float py, float r);
    void respawnCue();
    void updatePhysics();
    void updateBallPositions();
    void removeBall(int index);
};
