#include "Physics.h"
#include <cmath>
#include <algorithm>
// ����� ��������� ���� ������������ � ����
void Physics::HandleCollisions(Table& table, std::vector<Ball*>& balls) {
    // ��������� ������������ ����� ������
    for (size_t i = 0; i < balls.size(); ++i) {
        for (size_t j = i + 1; j < balls.size(); ++j) {
            if (DetectCollision(balls[i], balls[j])) {
                DynamicCollision(balls[i], balls[j]);
            }
        }
    }

    // ��������� ������������ ����� ������ � ������� �����
    for (auto& ball : balls) {
        for (auto& wall : table.getWalls()) {
            Vector2D* collisionPoint = DetectCollisionEdge(ball, &wall);
            if (collisionPoint) {
                delete collisionPoint;
            }
        }
    }

    // ��������� ������������ ����� ������ � ������
    for (auto& ball : balls) {
        for (auto& pocket : table.getPockets()) {
            if (DetectCollisionHole(ball, &pocket)) {
                ball->isPocketed = true; // ��� ����� � ����
                ball->velocity.setX(0); // ��������� ����
                ball->velocity.setY(0); // ��������� ����
            }
        }
    }
}

// ����� ����������� ������������ ���� �����
bool Physics::DetectCollision(Ball* ball1, Ball* ball2) {
    // ���������� ���������� ����� �������� �����
    float distanceX = ball1->position.getX() - ball2->position.getX();
    float distanceY = ball1->position.getY() - ball2->position.getY();
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);
    float overlap = 0.5f * (distance - ball1->radius - ball2->radius);

    // ���� ���������� ������ ����� ��������, ������ ���� ������������
    if (distance < ball1->radius + ball2->radius) {
        // ������������� ������� ����� ��� ���������� ���������
        ball1->position.setX(ball1->position.getX() - overlap * (distanceX / distance));
        ball1->position.setY(ball1->position.getY() - overlap * (distanceY / distance));
        ball2->position.setX(ball2->position.getX() + overlap * (distanceX / distance));
        ball2->position.setY(ball2->position.getY() + overlap * (distanceY / distance));
        return true;
    }
    return false;
}

// ����� ��������� ������������� ������������ ���� �����
void Physics::DynamicCollision(Ball* ball1, Ball* ball2) {
    // ���������� ���������� ����� �������� �����
    float distanceX = ball1->position.getX() - ball2->position.getX();
    float distanceY = ball1->position.getY() - ball2->position.getY();
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

    // ������������ ����������� ������������
    float normalX = (ball2->position.getX() - ball1->position.getX()) / distance;
    float normalY = (ball2->position.getY() - ball1->position.getY()) / distance;
    float relativeVelocityX = ball1->velocity.getX() - ball2->velocity.getX();
    float relativeVelocityY = ball1->velocity.getY() - ball2->velocity.getY();
    float impactSpeed = 2.0 * (normalX * relativeVelocityX + normalY * relativeVelocityY) / (ball1->mass + ball2->mass);

    // ���������� ��������� ����� ����� ������������
    ball1->velocity.setX(ball1->velocity.getX() - impactSpeed * ball2->mass * normalX);
    ball1->velocity.setY(ball1->velocity.getY() - impactSpeed * ball2->mass * normalY);
    ball2->velocity.setX(ball2->velocity.getX() + impactSpeed * ball1->mass * normalX);
    ball2->velocity.setY(ball2->velocity.getY() + impactSpeed * ball1->mass * normalY);
}

// ����� ����������� ������������ ���� � �����
bool Physics::DetectCollisionHole(Ball* ball, Pocket* pocket) {
    float distanceX = ball->position.getX() - pocket->getPosition().getX();
    float distanceY = ball->position.getY() - pocket->getPosition().getY();
    float distance = std::sqrt(distanceX * distanceX + distanceY * distanceY);

    // ��������, ��������� �� ��� � �������� ������� ����
    return distance < ball->radius + pocket->getRadius();
}

// ����� ����������� ������������ ���� � ����� �����
Vector2D* Physics::DetectCollisionEdge(Ball* ball, Wall* wall) {
    Vector2D wallStart = wall->getStart();
    Vector2D wallEnd = wall->getEnd();
    Vector2D ballPosition = ball->position;

    Vector2D wallVector = wallEnd - wallStart;
    Vector2D ballToWallStart = ballPosition - wallStart;

    float wallLengthSquared = wallVector.dot(wallVector);
    float projectionFactor = ballToWallStart.dot(wallVector) / wallLengthSquared;

    // ����������� ������� ��������, ����� ����� ���������� �� �������� �����
    projectionFactor = std::max(0.0f, std::min(1.0f, projectionFactor));

    // ���������� ��������� ����� �� �����
    Vector2D closestPoint = wallStart + wallVector * projectionFactor;
    Vector2D collisionNormal = ballPosition - closestPoint;
    float collisionDistance = collisionNormal.length();

    // ��������, ��������� �� ��� � �������� ������� �����
    if (collisionDistance < ball->radius) {
        float overlap = ball->radius - collisionDistance;
        collisionNormal.normalize();
        ball->position = ball->position + collisionNormal * overlap;

        // ���������� �������� ���� ����� ������������ �� ������
        float velocityDotNormal = ball->velocity.dot(collisionNormal);
        ball->velocity.setX(ball->velocity.getX() - collisionNormal.getX() * velocityDotNormal * 2.0f);
        ball->velocity.setY(ball->velocity.getY() - collisionNormal.getY() * velocityDotNormal * 2.0f);

        return new Vector2D(closestPoint);
    }

    return nullptr;
}
