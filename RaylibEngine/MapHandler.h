#pragma once

#include "raylib.h"
#include "Combat.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

struct MapHandler {
	static std::vector<std::vector<int>> LevelData;
	static void LoadMap(const char* filePath);
	static bool IsSolidTile(int tileID);
	static bool CheckCollision(Rectangle nextHitbox, int tileSize);
};