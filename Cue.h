#pragma once
#include <SDL.h>
#include "Vector2D.h"
#include "Ball.h"
class Cue {
public:
    Cue(SDL_Renderer* renderer, const char* texturePath);
    ~Cue();

    void handleInput(const SDL_Event& event, Vector2D& cueBallPosition, Vector2D& cueBallVelocity);
    void draw(SDL_Renderer* renderer, const Vector2D& cueBallPosition, bool ballsAreMoving, float mouseAngle);

    float getAngle() const;
    float getPower() const;
    void reset();

    float ballSeparation = 5.0f; // Сделаем член `public`

private:
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    float mouseAngle;
    bool dragging;
    Uint32 dragStartTime;
    float power;
    const float maxSpeed = 100.0f; // Максимальная скорость
};
