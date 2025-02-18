#include "Player.h"
#include <iostream>
#include <string>

Player::Player(SDL_Renderer* renderer, TTF_Font* font)
    : score(0), renderer(renderer), font(font) {
}

Player::~Player() {
}

int Player::getScore() const {
    return score;
}

void Player::addScore(int points) {
    score += points;
}

void Player::draw(SDL_Renderer* renderer) {
    // Отрисовка очков игрока
    SDL_Color textColor = { 255, 255, 255, 255 };
    std::string scoreText = "Score: " + std::to_string(score);

    // Используем SDL_ttf для рендеринга текста
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (textSurface != nullptr) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture != nullptr) {
            SDL_Rect textRect = { 10, 10, textSurface->w, textSurface->h };
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}
