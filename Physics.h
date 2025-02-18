#pragma once
#include "Ball.h"
#include "Pocket.h"
#include "Wall.h"
#include "Vector2D.h"
#include "Table.h"
#include <vector>

class Physics {
public:
    static void HandleCollisions(Table& table, std::vector<Ball*>& balls);
    static bool DetectCollision(Ball* b1, Ball* b2);
    static bool DetectCollisionHole(Ball* b, Pocket* h);
    static Vector2D* DetectCollisionEdge(Ball* b, Wall* e);
    static void DynamicCollision(Ball* b1, Ball* b2);
};
