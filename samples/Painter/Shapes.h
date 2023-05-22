#pragma once

#include "DirectMedia/Core.h"
#include "rad/Math/Random.h"

struct Circle
{
    ImVec2 center;
    float radius;
    ImColor color;
};

void Draw(ImDrawList* drawList, Circle& circle);

struct ShapeList : public rad::RefCounted<ShapeList>
{
    std::vector<Circle> circles;
};
