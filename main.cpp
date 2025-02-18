#include "Game.h"
#include <iostream>

int main(int argc, char* args[]) {
    Game game;

    // ������������� ���� � ������ �����������
    if (!game.init("Billiards Game", 1656, 928, false)) {
        std::cerr << "Failed to initialize game." << std::endl;
        return -1;
    }

    // �������� ���� ����
    while (game.running()) {
        game.handleEvents();
        game.update();
        game.render();
    }

    // ������� �������� ����� ���������� ����
    game.clean();

    return 0;
}
