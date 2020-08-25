#define OLC_PGE_APPLICATION
#include "Engine.h"
#include <iostream>

const int width = 901;
const int height = 901;

int main()
{
    Engine engine;
    engine.Construct(width, height, 1, 1);
    engine.Start();
}
