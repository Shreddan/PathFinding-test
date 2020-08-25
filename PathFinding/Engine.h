#pragma once
#include <olcPixelGameEngine.h>

const int mWidth = 30;
const int mHeight = 30;

struct Tile
{
	int x;
	int y;
	bool isSolid;
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

	std::list<Tile*> uTiles;

	Tile* currTile = nullptr;

	int TileSize = 28;
};

