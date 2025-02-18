#include "Game.h"
#include <iostream>

int main(int argc, char* args[]) {
    Game game;

    // Инициализация игры с шестью аргументами
    if (!game.init("Billiards Game", 1656, 928, false)) {
        std::cerr << "Failed to initialize game." << std::endl;
        return -1;
    }

    // Основной цикл игры
    while (game.running()) {
        game.handleEvents();
        game.update();
        game.render();
    }

    // Очистка ресурсов после завершения игры
    game.clean();

    return 0;
}
