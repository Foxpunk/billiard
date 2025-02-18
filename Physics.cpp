#include "Physics.h"
#include <cmath>
#include <algorithm>
// Метод обработки всех столкновений в игре
void Physics::HandleCollisions(Table& table, std::vector<Ball*>& balls) {
    // Обработка столкновений между шарами
    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            if (DetectCollision(balls[i], balls[j])) {
                DynamicCollision(balls[i], balls[j]);
            }
        }
    }

    // Обработка столкновений между шарами и стенами стола
    for (auto& ball : balls) {
        for (auto& wall : table.getWalls()) {
            Vector2D* collisionPoint = DetectCollisionEdge(ball, &wall);
            if (collisionPoint) {
                delete collisionPoint;
            }
        }
    }

    // Обработка столкновений между шарами и лузами
    for (auto& ball : balls) {
        for (auto& pocket : table.getPockets()) {
            if (DetectCollisionHole(ball, &pocket)) {
                ball->isPocketed = true; // Шар забит в лузу
                ball->velocity.setX(0); // Остановка шара
                ball->velocity.setY(0); // Остановка шара
            }
        }
    }
}

// Метод обнаружения столкновения двух шаров
bool Physics::DetectCollision(Ball* ball1, Ball* ball2) {
    // Вычисление расстояния между центрами шаров
    float distanceX = ball1->position.getX() - ball2->position.getX();
    float distanceY = ball1->position.getY() - ball2->position.getY();
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);
    float overlap = 0.5f * (distance - ball1->radius - ball2->radius);

    // Если расстояние меньше суммы радиусов, значит шары сталкиваются
    if (distance < ball1->radius + ball2->radius) {
        // Корректировка позиций шаров для устранения наложения
        ball1->position.setX(ball1->position.getX() - overlap * (distanceX / distance));
        ball1->position.setY(ball1->position.getY() - overlap * (distanceY / distance));
        ball2->position.setX(ball2->position.getX() + overlap * (distanceX / distance));
        ball2->position.setY(ball2->position.getY() + overlap * (distanceY / distance));
        return true;
    }
    return false;
}

// Метод обработки динамического столкновения двух шаров
void Physics::DynamicCollision(Ball* ball1, Ball* ball2) {
    // Вычисление расстояния между центрами шаров
    float distanceX = ball1->position.getX() - ball2->position.getX();
    float distanceY = ball1->position.getY() - ball2->position.getY();
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

    // Нормализация направления столкновения
    float normalX = (ball2->position.getX() - ball1->position.getX()) / distance;
    float normalY = (ball2->position.getY() - ball1->position.getY()) / distance;
    float relativeVelocityX = ball1->velocity.getX() - ball2->velocity.getX();
    float relativeVelocityY = ball1->velocity.getY() - ball2->velocity.getY();
    float impactSpeed = 2.0 * (normalX * relativeVelocityX + normalY * relativeVelocityY) / (ball1->mass + ball2->mass);

    // Обновление скоростей шаров после столкновения
    ball1->velocity.setX(ball1->velocity.getX() - impactSpeed * ball2->mass * normalX);
    ball1->velocity.setY(ball1->velocity.getY() - impactSpeed * ball2->mass * normalY);
    ball2->velocity.setX(ball2->velocity.getX() + impactSpeed * ball1->mass * normalX);
    ball2->velocity.setY(ball2->velocity.getY() + impactSpeed * ball1->mass * normalY);
}

// Метод обнаружения столкновения шара с лузой
bool Physics::DetectCollisionHole(Ball* ball, Pocket* pocket) {
    float distanceX = ball->position.getX() - pocket->getPosition().getX();
    float distanceY = ball->position.getY() - pocket->getPosition().getY();
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

    // Проверка, находится ли шар в пределах радиуса лузы
    return distance < ball->radius + pocket->getRadius();
}

// Метод обнаружения столкновения шара с краем стола
Vector2D* Physics::DetectCollisionEdge(Ball* ball, Wall* wall) {
    Vector2D wallStart = wall->getStart();
    Vector2D wallEnd = wall->getEnd();
    Vector2D ballPosition = ball->position;

    Vector2D wallVector = wallEnd - wallStart;
    Vector2D ballToWallStart = ballPosition - wallStart;

    float wallLengthSquared = wallVector.dot(wallVector);
    float projectionFactor = ballToWallStart.dot(wallVector) / wallLengthSquared;

    // Ограничение фактора проекции, чтобы точка находилась на сегменте стены
    projectionFactor = std::max(0.0f, std::min(1.0f, projectionFactor));

    // Вычисление ближайшей точки на стене
    Vector2D closestPoint = wallStart + wallVector * projectionFactor;
    Vector2D collisionNormal = ballPosition - closestPoint;
    float collisionDistance = collisionNormal.length();

    // Проверка, находится ли шар в пределах радиуса стены
    if (collisionDistance < ball->radius) {
        float overlap = ball->radius - collisionDistance;
        collisionNormal.normalize();
        ball->position = ball->position + collisionNormal * overlap;

        // Обновление скорости шара после столкновения со стеной
        float velocityDotNormal = ball->velocity.dot(collisionNormal);
        ball->velocity.setX(ball->velocity.getX() - collisionNormal.getX() * velocityDotNormal * 2.0f);
        ball->velocity.setY(ball->velocity.getY() - collisionNormal.getY() * velocityDotNormal * 2.0f);

        return new Vector2D(closestPoint);
    }

    return nullptr;
}
