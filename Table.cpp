#include "Table.h"
#include <iostream>
#include <SDL_image.h> 

Table::Table(float width, float height, const char* texturePath, SDL_Renderer* renderer)
    : width(width), height(height), texture(nullptr), renderer(renderer) { // Инициализация renderer
    // Загрузка текстуры стола
    SDL_Surface* surface = IMG_Load(texturePath);
    if (!surface) {
        std::cerr << "Unable to load texture: " << IMG_GetError() << std::endl;
    }
    else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!texture) {
            std::cerr << "Unable to create texture: " << SDL_GetError() << std::endl;
        }
    }
}

Table::~Table() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr; 
    }
}

void Table::init() {
    // Инициализация луз
    addPocket(Pocket(80, 80, 30)); 
    addPocket(Pocket(width - 80, 80, 30));
    addPocket(Pocket(80, height - 80, 30));
    addPocket(Pocket(width - 80, height - 80, 30));
    addPocket(Pocket(width / 2, 80, 30));
    addPocket(Pocket(width / 2, height - 80, 30));

    // Инициализация стенок
    addWall(Wall(Vector2D(50, 50), Vector2D(width - 50, 50)));  // Верхняя стенка
    addWall(Wall(Vector2D(50, height - 50), Vector2D(width - 50, height - 50)));  // Нижняя стенка
    addWall(Wall(Vector2D(50, 50), Vector2D(50, height - 50)));  // Левая стенка
    addWall(Wall(Vector2D(width - 50, 50), Vector2D(width - 50, height - 50)));  // Правая стенка
}

void Table::draw(SDL_Renderer* renderer) {
    // Отрисовка стола
    if (texture) {
        SDL_Rect destRect;
        destRect.x = 0;
        destRect.y = 0;
        destRect.w = static_cast<int>(width);
        destRect.h = static_cast<int>(height);
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
        SDL_Rect tableRect = { 0, 0, static_cast<int>(width), static_cast<int>(height) };
        SDL_RenderFillRect(renderer, &tableRect);
    }

    // Отрисовка луз
    for (auto& pocket : pockets) {
        pocket.draw(renderer);
    }

    // Отрисовка стенок
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); 
    for (auto& wall : walls) {
        SDL_RenderDrawLine(renderer,
            static_cast<int>(wall.getStart().getX()), static_cast<int>(wall.getStart().getY()),
            static_cast<int>(wall.getEnd().getX()), static_cast<int>(wall.getEnd().getY()));
    }
}

void Table::addPocket(const Pocket& pocket) {
    pockets.push_back(pocket);
}

void Table::addWall(const Wall& wall) {
    walls.push_back(wall);
}

std::vector<Pocket>& Table::getPockets() {
    return pockets;
}

std::vector<Wall>& Table::getWalls() {
    return walls;
}

float Table::getWidth() {
    return width;
}

float Table::getHeight() {
    return height;
}
