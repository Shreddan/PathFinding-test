#define OLC_PGE_APPLICATION
#include "Engine.h"
#include <iostream>

const int width = 785;
const int height = 785;

int main()
{
    Engine engine;
    engine.Construct(width, height, 1, 1);
    engine.Start();
}
