#pragma once
#include <vector>
#include "Pocket.h"
#include "Wall.h"
#include <SDL.h>

class Table {
public:
    Table(float width, float height, const char* texturePath, SDL_Renderer* renderer);
    ~Table();

    void init();
    void draw(SDL_Renderer* renderer);
    void addPocket(const Pocket& pocket);
    void addWall(const Wall& wall);
    std::vector<Pocket>& getPockets();
    std::vector<Wall>& getWalls();
    float getWidth();
    float getHeight();

private:
    float width;
    float height;
    SDL_Texture* texture;
    std::vector<Pocket> pockets;
    std::vector<Wall> walls;
    SDL_Renderer* renderer;
};
