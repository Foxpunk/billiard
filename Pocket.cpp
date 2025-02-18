#include "Pocket.h"

Pocket::Pocket(float x, float y, float r) : position(x, y), radius(r) {}

void Pocket::draw(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Черный цвет для луз
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
