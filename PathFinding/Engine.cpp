#include "Engine.h"

Engine::Engine()
{
	sAppName = "Pathfinding Practice!";
}
Engine::~Engine()
{
	OnUserDestroy();
}

bool Engine::OnUserCreate()
{
	map = new Tile[mWidth * mHeight];
	for (int x = 0; x < mWidth; x++)
	{
		for (int y = 0; y < mHeight; y++)
		{
			map[y * mWidth + x].x = x;
			map[y * mWidth + x].y = y;
			map[y * mWidth + x].isSolid = false;
			map[y * mWidth + x].Visited = false;
			map[y * mWidth + x].parent = nullptr;
			map[y * mWidth + x].isPath = false;
		}
	}
	addNeighbours();
	start = &map[(mHeight / 2) * mWidth + 1];
	return true;
}

bool Engine::OnUserUpdate(float fElapsedTime)
{
	int SelectedTileX = GetMouseX() / TileSize;
	int SelectedTileY = GetMouseY() / TileSize;


	if (GetMouse(0).bReleased)
	{
		if (SelectedTileX >= 0 && SelectedTileX < mWidth)
		{
			if (SelectedTileY >= 0 && SelectedTileY < mHeight)
			{
				if (GetKey(olc::SHIFT).bHeld)
				{
					start = &map[SelectedTileY * mWidth + SelectedTileX];
				}
				else if (GetKey(olc::CTRL).bHeld)
				{
					end = &map[SelectedTileY * mWidth + SelectedTileX];
				}
				else
				{
					map[SelectedTileY * mWidth + SelectedTileX].isSolid = !map[SelectedTileY * mWidth + SelectedTileX].isSolid;
				}
				aStar();
			}
		}
	}

	Clear(olc::BLACK);
	if (end != nullptr)
	{
		Tile* t = end;
		while (t->parent != nullptr && t != start)
		{
			t->isPath = true;
			t = t->parent;
		}
	}
	DrawTileMap();
	return true;
}

void Engine::DrawTileMap()
{
	for (int i = 0; i < (mWidth - 1) * (mHeight - 1); i++)
	{
		if (map[i].isSolid == 0)
		{
			if (&map[i] != start && &map[i] != end)
			{
				if (!map[i].isPath)
				{
					FillRect(olc::vf2d(map[i].x * 28, map[i].y * 28), olc::vf2d(28, 28), olc::BLUE);
				}
				else if (map[i].isPath)
				{
					FillRect(olc::vf2d(map[i].x * 28, map[i].y * 28), olc::vf2d(28, 28), olc::YELLOW);
				}
			}
			else if (&map[i] == start)
			{
				//std::cout << "Start : " << map[i].x << " " << map[i].y << std::endl;
				FillRect(olc::vf2d(map[i].x * 28, map[i].y * 28), olc::vf2d(28, 28), olc::GREEN);
			}
			else if (&map[i] == end)
			{
				//std::cout << "End : " << map[i].x << " " << map[i].y << std::endl;
				FillRect(olc::vf2d(map[i].x * 28, map[i].y * 28), olc::vf2d(28, 28), olc::RED);
			}
			
		}
		else
		{
			FillRect(olc::vf2d(map[i].x * 28, map[i].y * 28), olc::vf2d(28, 28), olc::VERY_DARK_BLUE);
		}

		DrawRect(olc::vf2d(map[i].x * 28, map[i].y * 28), olc::vf2d(28, 28), olc::WHITE);
		
	}
}

void Engine::addNeighbours()
{
	for (int x = 0; x < mWidth; x++)
	{
		for (int y = 0; y < mHeight; y++)
		{
			if (y > 0)
			{
				map[y * mWidth + x].neighbours.push_back(&map[(y - 1) * mWidth + (x + 0)]);
			}
			if (y < mHeight - 1)
			{
				map[y * mWidth + x].neighbours.push_back(&map[(y + 1) * mWidth + (x + 0)]);
			}
			if (x > 0)
			{
				map[y * mWidth + x].neighbours.push_back(&map[(y + 0) * mWidth + (x - 1)]);
			}
			if (x < mWidth - 1)
			{
				map[y * mWidth + x].neighbours.push_back(&map[(y + 0) * mWidth + (x + 1)]);
			}
		}
	}
}

bool Engine::aStar()
{
	std::cout << "Calculating A*" << std::endl;
	for (int x = 0; x < mWidth; x++)
	{
		for (int y = 0; y < mHeight; y++)
		{
			map[y * mWidth + x].Local = INFINITY;
			map[y * mWidth + x].Global = INFINITY;
			map[y * mWidth + x].Visited = false;
			map[y * mWidth + x].parent = nullptr;
		}
	}
	
	std::cout << "Reset variables" << std::endl;

	auto distance = [](Tile* a, Tile* b)
	{
		return sqrtf(a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y);
	};

	auto heuristic = [distance](Tile* a, Tile* b)
	{
		return distance(a, b);
	};

	currTile = start;
	currTile->Local = 0.0f;
	currTile->Global = heuristic(start, end);

	std::cout << "Setting Current Tile + Global Goal" << std::endl;

	uTiles.push_back(start);

	std::cout << "Created Node List" << std::endl;

	while (!uTiles.empty() && currTile != end)
	{
		uTiles.sort([](const Tile* lhs, const Tile* rhs) {return lhs->Global < rhs->Global; });

		while (!uTiles.empty() && uTiles.front()->Visited)
		{
			uTiles.pop_front();
		}

		
		currTile = uTiles.front();
		currTile->Visited = true;
		

		if (uTiles.empty())
		{
			std::cout << "Job done!" << std::endl;
			break;
		}

		for (auto tNeighbour : currTile->neighbours)
		{
			if (!tNeighbour->Visited && tNeighbour->isSolid == 0)
			{
				uTiles.push_back(tNeighbour);
			}
			float pLowerGoal = currTile->Local + distance(currTile, tNeighbour);
			if (pLowerGoal < tNeighbour->Local)
			{
				tNeighbour->parent = currTile;
				tNeighbour->Local = pLowerGoal;
				tNeighbour->Global = tNeighbour->Local + heuristic(tNeighbour, end);
			}
		}
	}
	return true;
}
