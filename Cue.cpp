#include "Cue.h"
#include <iostream>
#include <cmath>
#include <SDL_image.h>

Cue::Cue(SDL_Renderer* renderer, const char* texturePath)
    : renderer(renderer), texture(nullptr), mouseAngle(0.0f), dragging(false), power(0.0f) {
    SDL_Surface* surface = IMG_Load(texturePath);
    if (!surface) {
        std::cerr << "Unable to load texture: " << texturePath << " " << IMG_GetError() << std::endl;
    }
    else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) {
            std::cerr << "Unable to create texture from surface: " << SDL_GetError() << std::endl;
        }
    }
}

Cue::~Cue() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Cue::draw(SDL_Renderer* renderer, const Vector2D& cueBallPosition, bool ballsAreMoving, float mouseAngle) {
    if (ballsAreMoving) {
        return;
    }

    SDL_Rect destRect;
    destRect.w = 400; // Длина кия
    destRect.h = 100;  // Ширина кия
    destRect.x = static_cast<int>(cueBallPosition.getX() - std::cos(mouseAngle) * ballSeparation - destRect.w / 2);
    destRect.y = static_cast<int>(cueBallPosition.getY() - std::sin(mouseAngle) * ballSeparation - destRect.h / 2);

    SDL_Point center = { destRect.w / 2, destRect.h / 2 };

    SDL_RenderCopyEx(renderer, texture, nullptr, &destRect, mouseAngle * 180 / M_PI + 180, &center, SDL_FLIP_NONE); 
}

void Cue::handleInput(const SDL_Event& event, Vector2D& cueBallPosition, Vector2D& cueBallVelocity) {
    if (event.type == SDL_MOUSEMOTION) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        mouseAngle = std::atan2(mouseY - cueBallPosition.getY(), mouseX - cueBallPosition.getX());
    }
    if (event.type == SDL_MOUSEBUTTONDOWN && cueBallVelocity.getX() == 0 && cueBallVelocity.getY() == 0) {
        cueBallVelocity.setX(std::cos(mouseAngle) * 10);
        cueBallVelocity.setY(std::sin(mouseAngle) * 10);
    }
}


float Cue::getAngle() const {
    return mouseAngle;
}

float Cue::getPower() const {
    return power;
}

void Cue::reset() {
    power = 0;
    ballSeparation = 5.0f;
    dragging = false;
}
