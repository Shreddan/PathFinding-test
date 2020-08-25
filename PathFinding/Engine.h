#pragma once
#include <olcPixelGameEngine.h>

const int mWidth = 32;
const int mHeight = 32;

struct Tile
{
	int x;
	int y;
	bool isSolid;
	bool isStart;
	bool isEnd;
	bool isPath;
	bool Visited;
	Tile* parent = nullptr;
	float Global;
	float Local;
	std::vector<Tile*> neighbours;
};

class Engine : public olc::PixelGameEngine
{
public:
	Engine();
	~Engine();

	bool OnUserCreate();
	bool OnUserUpdate(float fElapsedTime);

	void DrawTileMap();

	void addNeighbours();

	bool aStar();

	Tile* map = nullptr;

	Tile* start = nullptr;
	Tile* end = nullptr;

	int TileSize = 30;
};

