#include "raylib.h"
#include "Combat.h"
#include "MapHandler.h"
#include "GameConfig.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

std::vector<std::vector<int>> MapHandler::LevelData;
void MapHandler::LoadMap(const char* filePath) {
	std::ifstream file(filePath);
	std::string line;
	while (std::getline(file, line)) {
		std::vector<int> row;
		std::istringstream ss(line);
		std::string tile;
		while (std::getline(ss, tile, '-')) {
		row.push_back(std::stoi(tile));
	}
	LevelData.push_back(row);
	}
}
bool MapHandler::IsSolidTile(int tileID) {
	switch (tileID)
	{
	case 6: return true;//Wall Middle
	case 8: return true;//Wall back
	default:
		return false;
	}
}
bool MapHandler::CheckCollision(Rectangle nextHitbox, int tileSize) {
	if (nextHitbox.x < 0 || nextHitbox.y < 0 || nextHitbox.x + nextHitbox.width > GameConfig::ScreenWidth || nextHitbox.y + nextHitbox.height > GameConfig::ScreenHeight) {
		return true;
	}

	int leftCol = (int)nextHitbox.x / tileSize;
	int rightCol = (int)(nextHitbox.x + nextHitbox.width) / tileSize;
	int topRow = (int)nextHitbox.y / tileSize;
	int bottomRow = (int)(nextHitbox.y + nextHitbox.height) / tileSize;

	if (leftCol < 0 || topRow < 0 || rightCol >= LevelData[0].size()  || bottomRow >= LevelData.size() ) {
		return true;
	}


	for (int y = topRow; y <= bottomRow; y++)
	{
		for (int x = leftCol; x <= rightCol; x++)
		{
			if (IsSolidTile(LevelData[y][x])) {
				return true;
			}
		}
	}

	return false;
}