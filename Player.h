#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

class Player {
public:
    Player(SDL_Renderer* renderer, TTF_Font* font);
    ~Player();

    int getScore() const;
    void addScore(int points);
    void draw(SDL_Renderer* renderer);

private:
    int score;
    SDL_Renderer* renderer;
    TTF_Font* font;
};
