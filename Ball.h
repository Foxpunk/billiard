#pragma once
#include <SDL.h>
#include "Vector2D.h"

class Ball {
public:
    Ball(float posx, float posy, float r, int ID, SDL_Renderer* renderer, const char* texturePath = nullptr);

    void printID();
    void printBall();
    void draw();
    void update();
    void updatePhysics();
    void resetCue();
    bool isMoving() const;

    Vector2D position;
    Vector2D texPos;
    Vector2D velocity;
    float radius;
    int id;
    float mass = 100;
    bool isPocketed;

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};
