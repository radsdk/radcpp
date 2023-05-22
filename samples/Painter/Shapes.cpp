#include "Shapes.h"

void Draw(ImDrawList* drawList, Circle& circle)
{
    drawList->AddCircleFilled(circle.center, circle.radius, circle.color);
}
