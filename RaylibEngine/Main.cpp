#include "raylib.h"
#include "Combat.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <typeinfo>

// Game State Enum to follow the current state of the game, whether it's in map mode, combat mode, or game over.
enum GameState {
	MAP_MODE,//Player can move around the map and encounter monsters
	COMBAT_MODE,//Player is in combat with a monster, can attack or use potions
	GAME_OVER,//Player has died, show game over screen
};
enum CombatPhase {
	PLAYER_TURN,//Player can choose to attack or use potion
	ENEMY_TURN//Enemy will attack the player
};


void ResetPosition(Player& player, Monster& monster) {
	player.x = 100;
	player.y = 260;
	monster.x = 650;
	monster.y = 260;	
}



int main() {
	srand(time(0));

	//Start window
	InitWindow(800, 600, "DungeonWarrior - Raylib Edition");
	SetTargetFPS(60);

	//Crate Player and add it to the game
	Player player = Player(20, 0, 3);
	player.x = 100;
	player.y = 260;
	int score = 0;

	//Create Monster and add it to the game
	Monster* Target = new Monster(1);
	Target->x = 650;
	Target->y = 260;
	int wave = 1;

	Color TargetColor = ORANGE;
	Color PlayerColor = BLUE;

	//Setting the initial game state to map mode
	GameState currentState = MAP_MODE;
	CombatPhase currentPhase = PLAYER_TURN;

	std::string Message = " ";

	float combatStartTimer = 3.f;
	float messageDisplayTimer = 3.f;

	float defaultMessageDisplayTimer = 1.5f;

	//Main game loop
	while (!WindowShouldClose()) {
		// --- A.Update Logic update in background ---
		messageDisplayTimer += GetFrameTime();
		combatStartTimer += GetFrameTime();

		//set a vector for mouse position
		Vector2 mousePos = GetMousePosition();

		//Set rectangles for the attack and potion buttons
		Rectangle attackButton = { 100, 500, 150, 40 };
		Rectangle potionButton = { 100, 550, 150, 40 };


		if (currentState == MAP_MODE) {
			// WASD buttons to move the player
			if (IsKeyDown(KEY_W)) {
				if(player.y >= 5)player.y -= 5;//Move up
			}
			if (IsKeyDown(KEY_S)) {
				if(player.y <= 545)player.y += 5;//Move down
			}
			if (IsKeyDown(KEY_A)) {
				if(player.x >= 5)player.x -= 5;//Move left
			}
			if (IsKeyDown(KEY_D)) {
				if(player.x <= 745)player.x += 5;//Move right
			}	

			//Draw Hitbox
			Rectangle playerHitbox = { (float)player.x, (float)player.y,50,50 };
			Rectangle targetHitbox = { (float)Target->x, (float)Target->y,50,50 };

			if (CheckCollisionRecs(playerHitbox, targetHitbox)) {
				currentState = COMBAT_MODE;//Switch to combat mode when player collides with the monster
				
				TargetColor = RED;//Change monster color to red to indicate combat mode
				combatStartTimer = 0.f;
			}
		}
		else if (currentState == COMBAT_MODE) {
			//Combat logic goes here , for now we will just switch back to map mode after 3 seconds
			if (currentPhase == PLAYER_TURN) {

				//check if the mouse button is pressed
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					if (CheckCollisionPointRec(mousePos, attackButton)) {
						Message = TryHit(player, *Target);
						messageDisplayTimer = 0.f;
						currentPhase = ENEMY_TURN;//Switch to enemy turn after player action
					}
					else if (CheckCollisionPointRec(mousePos, potionButton)) {
						Message = TryUsePotion(player);
						messageDisplayTimer = 0.f;
						currentPhase = ENEMY_TURN;
					}

				}


				if (IsKeyPressed(KEY_ONE)) {//Player Wants to attack
					
					Message = TryHit(player, *Target);
					messageDisplayTimer = 0.f;
					currentPhase = ENEMY_TURN;//Switch to enemy turn after player action
				}
				else if (IsKeyPressed(KEY_TWO)) {
					Message = TryUsePotion(player);
					messageDisplayTimer = 0.f;
					currentPhase = ENEMY_TURN;//Switch to enemy turn after player action
				}
			}
			else if (currentPhase == ENEMY_TURN) {
				if(messageDisplayTimer > defaultMessageDisplayTimer){
					if (Target->Health <= 0) {
						Message = TextFormat("You Killed the enemy!!!");
						messageDisplayTimer = 0.f;
						score += 10;
						wave += 1;
						player.DamageBonus += 1;
						delete Target;
						Target = new Monster(wave);
						ResetPosition(player, *Target);
						currentState = MAP_MODE;//Switch back to map mode after killing the monster
						currentPhase = PLAYER_TURN;//Reset combat phase to player turn for the next combat
					}
					else {
						Message = TryHit(*Target, player);
						messageDisplayTimer = 0.f;
						currentPhase = PLAYER_TURN;//Switch to enemy turn after player action
					}
					if (player.Health <= 0) {
						Message = TextFormat("You Died !!!");
						messageDisplayTimer = 0.f;
						currentState = GAME_OVER;//Switch to game over state after player dies
					}
				}
			}

			
		}
		
		
		// --- B.Draw Logic update in foreground ---
		BeginDrawing();
		ClearBackground(RAYWHITE);

		if(currentState == GAME_OVER){
			DrawRectangle(250, 100, 300, 40, LIGHTGRAY);
			DrawText(TextFormat("Game Over! Score: %d", score), 260, 110, 20, BLACK);
			EndDrawing();
			continue;
		}

		//if the Combat statrt timer is smaller than 3 seconds we can draw a "Combat Starting in..." text to indicate the player that combat is about to start
		if (combatStartTimer < 3) {
			DrawRectangle(250, 100, 300, 40, LIGHTGRAY);
			DrawText(TextFormat("Combat Begins..."), 260, 110, 20, BLACK);
		}


		//Draw status Texts
		DrawText(TextFormat("Player Health: %d", player.Health), 10, 10, 20, PlayerColor);
		for (int i = 1; i <= 3; i++)
		{
			int startX = 15 * i;
			DrawRectangle(startX, 40, 10, 10, PlayerColor);
			if (i > player.PotionCount) {
				DrawRectangle(startX + 2, 42, 6, 6, RAYWHITE);
			}
		}
		DrawText(TextFormat("Monster Health: %d", Target->Health), 600, 10, 20, TargetColor);

		//Draw Message if there is one and the message display timer is smaller than 3 seconds
		if (messageDisplayTimer < defaultMessageDisplayTimer && Message != " ") {
			DrawRectangle(250, 100, 300, 40, LIGHTGRAY);
			DrawText(Message.c_str(), 260, 110, 20, BLACK);
		}
		
		//Draw the map and the characters based on the current game state
		if (currentState == MAP_MODE) {
			//Draw Player and Monster
			DrawRectangle(Target->x, Target->y, 50, 50, TargetColor);
			DrawRectangle(player.x, player.y, 50, 50, PlayerColor);
		}

		//In combat mode we can draw the combat UI and options for the player to choose from
		else if (currentState == COMBAT_MODE) {

			DrawRectangle(200, 200, 50, 50, PlayerColor);
			DrawRectangle(500, 200, 50, 50, TargetColor);

			DrawRectangle(100, 500, 150, 40, LIGHTGRAY);
			if (CheckCollisionPointRec(mousePos, attackButton)) {
				DrawRectangle(105, 505, 140, 30, GRAY);
				if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) DrawRectangle(105, 505, 140, 30, DARKGRAY);
		}
			DrawText(" 1.Attack",110,510,20,BLACK);

			DrawRectangle(100, 550, 150, 40, LIGHTGRAY);
			if (CheckCollisionPointRec(mousePos, potionButton)) {
				DrawRectangle(105, 555, 140, 30, GRAY);
				if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) DrawRectangle(105, 555, 140, 30, DARKGRAY);
			}
			DrawText(" 2.Use Potion", 110, 560, 20, BLACK);
		}

		//Draw Monster and Player

		EndDrawing();
	}

	//Clear Memory and Exit
	delete Target;
	CloseWindow();

	return 0;

}