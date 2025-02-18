#include "Ball.h"
#include "Game.h"
#include <iostream>
#include <cmath>
#include <SDL_image.h>

Ball::Ball(float posx, float posy, float r, int ID, SDL_Renderer* renderer, const char* texturePath)
    : renderer(renderer), texture(nullptr), isPocketed(false) {
    id = ID;
    position.setX(posx);
    position.setY(posy);
    radius = r;
    texPos.setX(position.getX() - radius);
    texPos.setY(position.getY() - radius);

    std::cout << "Ball initialized at position (" << posx << ", " << posy << ") with velocity (" << velocity.getX() << ", " << velocity.getY() << ")" << std::endl;

    if (texturePath) {
        SDL_Surface* surface = IMG_Load(texturePath);
        if (surface) {
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }
        else {
            std::cerr << "Unable to load texture: " << IMG_GetError() << std::endl;
        }
    }
}

void Ball::printBall() {
    std::cout << "Ball" << std::endl;
    std::cout << id << std::endl;
    std::cout << position.getX() << std::endl;
    std::cout << position.getY() << std::endl;
    std::cout << velocity.getX() << std::endl;
    std::cout << velocity.getY() << std::endl;
    std::cout << mass << std::endl;
}

void Ball::printID() {
    std::cout << id << std::endl;
}

void Ball::resetCue() {
    position.setX(350.f);
    position.setY(335.f);
    velocity.setX(0.f);
    velocity.setY(0.f);
}

void Ball::draw() {
    if (isPocketed) return;

    if (texture) {
        SDL_Rect destRect;
        destRect.x = static_cast<int>(texPos.getX());
        destRect.y = static_cast<int>(texPos.getY());
        destRect.w = static_cast<int>(radius * 2);
        destRect.h = static_cast<int>(radius * 2);
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Красный цвет   
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w; // Горизонтальное отклонение
                int dy = radius - h; // Вертикальное отклонение
                if ((dx * dx + dy * dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(renderer, static_cast<int>(position.getX()) + dx, static_cast<int>(position.getY()) + dy);
                }
            }
        }
    }
}

void Ball::updatePhysics() {
    //std::cout << "Before update - Position: (" << position.getX() << ", " << position.getY() << ") Velocity: (" << velocity.getX() << ", " << velocity.getY() << ")" << std::endl;
    velocity.setX(velocity.getX() * 0.997);
    velocity.setY(velocity.getY() * 0.997);

    if (abs(velocity.getX() * velocity.getX() + velocity.getY() * velocity.getY()) < 0.0001) {
        velocity.setX(0);
        velocity.setY(0);
    }

    position.setX(position.getX() + velocity.getX());
    position.setY(position.getY() + velocity.getY());
   // std::cout << "After update - Position: (" << position.getX() << ", " << position.getY() << ")" << std::endl;
}

void Ball::update() {
    if (position.getX() < 0)
        position.setX(Game::screenSize.getX());
    if (position.getX() > Game::screenSize.getX())
        position.setX(0);
    if (position.getY() < 0)
        position.setY(Game::screenSize.getY());
    if (position.getY() > Game::screenSize.getY())
        position.setY(0);

    texPos.setX(position.getX() - radius);
    texPos.setY(position.getY() - radius);
}

bool Ball::isMoving() const {
    return velocity.getX() != 0 || velocity.getY() != 0;
}
