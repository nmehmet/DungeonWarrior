#pragma once


namespace GameConfig {
	constexpr int ScreenWidth = 800;
	constexpr int ScreenHeight = 600;
	constexpr int TargetFPS = 60;
	constexpr const char* WindowTitle = "DungeonWarrior - Raylib Edition";

	constexpr int MapBoundaryMinX = 5;
	constexpr int MapBoundaryMaxX = ScreenWidth - 55;
	constexpr int MapBoundaryMinY = 5;
	constexpr int MapBoundaryMaxY = ScreenHeight - 55;
}