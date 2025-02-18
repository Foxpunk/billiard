#include "Game.h"
#include <iostream>
#include <cmath>
#include <SDL_image.h>
#include <string>
Vector2D Game::screenSize;
SDL_Event Game::event;

Game::Game() : isRunning(false), onStartScreen(true), ballCount(0), gameOver(false), window(nullptr), renderer(nullptr), table(nullptr), player(nullptr), cue(nullptr), cueBall(nullptr) {}

Game::~Game() {
    clean();
}

bool Game::init(const char* title, int width, int height, bool fullscreen) {
    int flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_DestroyWindow(window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "TTF_Init Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    font = TTF_OpenFont("assets/Tiny5-Regular.ttf", 24);
    if (!font) {
        std::cerr << "TTF_OpenFont Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    isRunning = true;

    screenSize.setX(width);
    screenSize.setY(height);

    initTable("assets/PoolTable.png");
    initCue("assets/cue.png");
    initPlayer();

    return true;
}

void Game::initTable(const char* texturePath) {
    table = new Table(screenSize.getX(), screenSize.getY(), texturePath, renderer);
    table->init();
}

void Game::initCue(const char* texturePath) {
    cue = new Cue(renderer, texturePath);
}

void Game::initPlayer() {
    player = new Player(renderer, font);
}

void Game::handleEvents() {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT) {
        isRunning = false;
    }
    if (onStartScreen) {
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
            case SDLK_1:
                ballCount = 3;
                break;
            case SDLK_2:
                ballCount = 6;
                break;
            case SDLK_3:
                ballCount = 10;
                break;
            case SDLK_4:
                ballCount = 15;
                break;
            case SDLK_RETURN:
                if (ballCount > 0) {
                    onStartScreen = false;
                    initBalls(ballCount);
                }
                break;
            }
        }
    }
    else if (!gameOver) {
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
            onStartScreen = false;
        }
        else if (!ballsAreMoving()) {
            cue->handleInput(event, cueBall->position, cueBall->velocity);
        }
    }
}

void Game::update() {
    if (onStartScreen || gameOver) {
        return;
    }

    updatePhysics();
    updateBallPositions();

    std::vector<int> pocketedBalls;

    for (int i = 0; i < balls.size(); ++i) {
        auto& ball = balls[i];
        if (ball->isPocketed) {
            if (ball == cueBall) {
                player->addScore(-1);
                respawnCue();
            }
            else {
                player->addScore(1);
                pocketedBalls.push_back(i);
            }
        }
    }

    for (int i = pocketedBalls.size() - 1; i >= 0; --i) {
        removeBall(pocketedBalls[i]);
    }

    if (balls.size() == 1) {
        gameOver = true;
    }

    Physics::HandleCollisions(*table, balls);
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (onStartScreen) {
        renderStartScreen();
    }
    else if (gameOver) {
        renderGameOverScreen();
    }
    else {
        table->draw(renderer);

        for (auto& b : balls) {
            b->draw();
        }

        for (auto& h : holes) {
            h.draw(renderer);
        }

        SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
        for (auto& e : edges) {
            SDL_RenderDrawLine(renderer, static_cast<int>(e.getStart().getX()), static_cast<int>(e.getStart().getY()), static_cast<int>(e.getEnd().getX()), static_cast<int>(e.getEnd().getY()));
        }

        cue->draw(renderer, cueBall->position, ballsAreMoving(), cue->getAngle());
        player->draw(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Game::renderStartScreen() {
    SDL_Color textColor = { 255, 255, 255, 255 };
    std::string authorText = "Author: Aleksandr Koryukov";
    std::string goalText = "Goal: Score points by pocketing balls.";
    std::string controlsText = "Controls: Use mouse to aim and shoot. Press Enter to start.";
    std::string optionsText = "Press 1 for 3 balls, 2 for 6 balls, 3 for 10 balls, 4 for 15 balls";

    SDL_Surface* authorSurface = TTF_RenderText_Solid(font, authorText.c_str(), textColor);
    SDL_Surface* goalSurface = TTF_RenderText_Solid(font, goalText.c_str(), textColor);
    SDL_Surface* controlsSurface = TTF_RenderText_Solid(font, controlsText.c_str(), textColor);
    SDL_Surface* optionsSurface = TTF_RenderText_Solid(font, optionsText.c_str(), textColor);

    SDL_Texture* authorTexture = SDL_CreateTextureFromSurface(renderer, authorSurface);
    SDL_Texture* goalTexture = SDL_CreateTextureFromSurface(renderer, goalSurface);
    SDL_Texture* controlsTexture = SDL_CreateTextureFromSurface(renderer, controlsSurface);
    SDL_Texture* optionsTexture = SDL_CreateTextureFromSurface(renderer, optionsSurface);

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    SDL_Rect authorRect = { windowWidth / 2 - authorSurface->w / 2, windowHeight / 4 - authorSurface->h / 2, authorSurface->w, authorSurface->h };
    SDL_Rect goalRect = { windowWidth / 2 - goalSurface->w / 2, windowHeight / 2 - goalSurface->h / 2, goalSurface->w, goalSurface->h };
    SDL_Rect controlsRect = { windowWidth / 2 - controlsSurface->w / 2, windowHeight * 3 / 4 - controlsSurface->h / 2, controlsSurface->w, controlsSurface->h };
    SDL_Rect optionsRect = { windowWidth / 2 - optionsSurface->w / 2, windowHeight * 3 / 4 + controlsSurface->h, optionsSurface->w, optionsSurface->h };

    SDL_RenderCopy(renderer, authorTexture, nullptr, &authorRect);
    SDL_RenderCopy(renderer, goalTexture, nullptr, &goalRect);
    SDL_RenderCopy(renderer, controlsTexture, nullptr, &controlsRect);
    SDL_RenderCopy(renderer, optionsTexture, nullptr, &optionsRect);

    SDL_FreeSurface(authorSurface);
    SDL_FreeSurface(goalSurface);
    SDL_FreeSurface(controlsSurface);
    SDL_FreeSurface(optionsSurface);
    SDL_DestroyTexture(authorTexture);
    SDL_DestroyTexture(goalTexture);
    SDL_DestroyTexture(controlsTexture);
    SDL_DestroyTexture(optionsTexture);
}

void Game::renderGameOverScreen() {
    SDL_Color textColor = { 255, 255, 255, 255 };
    std::string gameOverText = "Game Over!";
    std::string scoreText = "Your score: " + std::to_string(player->getScore());

    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, gameOverText.c_str(), textColor);
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);

    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    SDL_Rect gameOverRect = { windowWidth / 2 - gameOverSurface->w / 2, windowHeight / 3 - gameOverSurface->h / 2, gameOverSurface->w, gameOverSurface->h };
    SDL_Rect scoreRect = { windowWidth / 2 - scoreSurface->w / 2, windowHeight / 2 - scoreSurface->h / 2, scoreSurface->w, scoreSurface->h };

    SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
    SDL_RenderCopy(renderer, scoreTexture, nullptr, &scoreRect);

    SDL_FreeSurface(gameOverSurface);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(scoreTexture);
}

void Game::clean() {
    delete table;
    delete player;
    delete cue;
    for (auto& ball : balls) {
        delete ball;
    }
    balls.clear();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::drawBalls() {
    for (auto& b : balls) {
        b->draw();
    }
}

void Game::drawHoles() {
    for (auto& h : holes) {
        h.draw(renderer);
    }
}

void Game::drawEdges() {
    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255);
    for (auto& e : edges) {
        SDL_RenderDrawLine(renderer, static_cast<int>(e.getStart().getX()), static_cast<int>(e.getStart().getY()), static_cast<int>(e.getEnd().getX()), static_cast<int>(e.getEnd().getY()));
    }
}

void Game::initBalls(int count) {
    cueBall = new Ball(200, screenSize.getY() / 2, 15, 1, renderer, "assets/bb_white.png");
    balls.push_back(cueBall);

    float startX = screenSize.getX() - 250;
    float startY = screenSize.getY() / 2;
    float offsetX = 31;
    float offsetY = 26.5;
    int ballID = 2;

    auto addBall = [&](float x, float y, const char* texture) {
        balls.push_back(new Ball(x, y, 15, ballID++, renderer, texture));
        };

    if (count >= 3) {
        addBall(startX, startY, "assets/bb_red.png");
        addBall(startX + offsetX, startY - offsetY / 2, "assets/bb_purple.png");
        addBall(startX + offsetX, startY + offsetY / 2, "assets/bb_blue.png");
    }
    if (count >= 6) {
        addBall(startX + 2 * offsetX, startY - offsetY, "assets/bb_green.png");
        addBall(startX + 2 * offsetX, startY, "assets/bb_orange.png");
        addBall(startX + 2 * offsetX, startY + offsetY, "assets/bb_yellow.png");
    }
    if (count >= 10) {
        addBall(startX + 3 * offsetX, startY - 1.5 * offsetY, "assets/bb_black.png");
        addBall(startX + 3 * offsetX, startY - 0.5 * offsetY, "assets/bb_brown.png");
        addBall(startX + 3 * offsetX, startY + 0.5 * offsetY, "assets/bb_red.png");
        addBall(startX + 3 * offsetX, startY + 1.5 * offsetY, "assets/bb_purple.png");
    }
    if (count >= 15) {
        addBall(startX + 4 * offsetX, startY - 2 * offsetY, "assets/bb_blue.png");
        addBall(startX + 4 * offsetX, startY - offsetY, "assets/bb_green.png");
        addBall(startX + 4 * offsetX, startY, "assets/bb_orange.png");
        addBall(startX + 4 * offsetX, startY + offsetY, "assets/bb_yellow.png");
        addBall(startX + 4 * offsetX, startY + 2 * offsetY, "assets/bb_black.png");
    }
}

bool Game::ballsAreMoving() const {
    for (const auto& ball : balls) {
        if (ball->isMoving()) {
            return true;
        }
    }
    return false;
}

void Game::addBall(float px, float py, float r, int id, bool isCue, const char* texturePath) {
    if (isCue) {
        cueBall = new Ball(px, py, r, id, renderer, texturePath);
        balls.push_back(cueBall);
    }
    else {
        balls.push_back(new Ball(px, py, r, id, renderer, texturePath));
    }
}

void Game::addEdge(float px1, float py1, float px2, float py2, float r) {
    edges.push_back(Wall(Vector2D(px1, py1), Vector2D(px2, py2)));
}

void Game::addHole(float px, float py, float r) {
    holes.push_back(Pocket(px, py, r));
}

void Game::respawnCue() {
    cueBall->position.setX(200);
    cueBall->position.setY(screenSize.getY() / 2);
    cueBall->velocity.setX(0);
    cueBall->velocity.setY(0);
    cueBall->isPocketed = false;
}

void Game::updatePhysics() {
    for (auto& ball : balls) {
        ball->updatePhysics();
    }
}

void Game::updateBallPositions() {
    SDL_GetMouseState(&mousex, &mousey);
    mouseangle = std::atan2(mousex - cueBall->position.getX(), mousey - cueBall->position.getY());

    for (auto& b : balls) {
        b->update();
    }

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE) {
        for (auto& b : balls) {
            if (std::abs(mousex - b->position.getX()) < b->radius && std::abs(mousey - b->position.getY()) < b->radius) {
                b->velocity.setX(-std::sin(mouseangle) * hitPower / 50);
                b->velocity.setY(-std::cos(mouseangle) * hitPower / 50);
                hitPower = 0;
                break;
            }
        }
    }
}

void Game::removeBall(int index) {
    if (index >= 0 && index < balls.size()) {
        delete balls[index];
        balls.erase(balls.begin() + index);
    }
}
