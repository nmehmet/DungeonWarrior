#include "UIHandler.h"
#include "GameConfig.h"
#include "StateMachine.h"

namespace UIHandler {
	void DrawGameHUD(Player& player, Monster& target) {
		//Get mouse position for button hover effects
		Vector2 mousePos = GetMousePosition();

		//Draw Player Health and Potion Count
		DrawText(TextFormat("Player Health: %d", player.Health), 10, 10, 20, player.BarColor);
		DrawRectangle(10, 35, 200, 20, GRAY);
		DrawRectangle(12, 37, (int)(200 * ((float)player.Health / player.MaxHealth) - 4), 16, player.BarColor);
		for (int i = 1; i <= 3; i++)
		{
			int startX = 15 * i;
			DrawRectangle(startX, 60, 10, 10, player.BarColor);
			if (i > player.PotionCount) {
				DrawRectangle(startX + 2, 62, 6, 6, RAYWHITE);
			}
		}
		//Draw Target Health

		if (StateMachine::CurrentState == GameState::COMBAT_MODE) {
			DrawText(TextFormat("Monster Health: %d", target.Health), 600, 10, 20, target.BarColor);
			DrawRectangle(590, 35, 200, 20, GRAY);
			float healthBarWidth = 200 * ((float)target.Health / target.MaxHealth) - 4;
			float barStartX = 788 - healthBarWidth;
			DrawRectangle((int)barStartX, 37, (int)healthBarWidth, 16, target.BarColor);

			//Set rectangles for the attack and potion buttons
			Rectangle attackButton = { 100, 500, 150, 40 };
			Rectangle potionButton = { 100, 550, 150, 40 };

			//In combat mode we can draw the combat UI and options for the player to choose from
			DrawRectangle(100, 500, 150, 40, LIGHTGRAY);
			if (CheckCollisionPointRec(mousePos, attackButton)) {
				DrawRectangle(105, 505, 140, 30, GRAY);
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) DrawRectangle(105, 505, 140, 30, DARKGRAY);
			}
			DrawText(" 1.Attack", 110, 510, 20, BLACK);

			DrawRectangle(100, 550, 150, 40, LIGHTGRAY);
			if (CheckCollisionPointRec(mousePos, potionButton)) {
				DrawRectangle(105, 555, 140, 30, GRAY);
				if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) DrawRectangle(105, 555, 140, 30, DARKGRAY);
			}
			DrawText(" 2.Use Potion", 110, 560, 20, BLACK);
		}
		if (StateMachine::CurrentState == GameState::GAME_OVER) {
			DrawRectangle(250, 100, 300, 40, LIGHTGRAY);
			DrawText(TextFormat("Game Over! Score: %d", StateMachine::Score), 260, 110, 20, BLACK);
		}
	}
}