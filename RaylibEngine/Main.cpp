#include "raylib.h"
#include "Combat.h"
#include "GameConfig.h"
#include "UIHandler.h"
#include "MapHandler.h"
#include "StateMachine.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <typeinfo>

void ResetPosition(Player& player, Monster& monster) {
	player.x = 100;
	player.y = 260;
	monster.x = 650;
	monster.y = 260;	
}
void CombatPosition(Player& player, Monster& monster) {
	player.x = 200;
	player.y = 300;
	monster.x = 500;
	monster.y = 300;
}
int main() {
	srand((unsigned int)time(0));

	//Start window
	InitWindow(GameConfig::ScreenWidth, GameConfig::ScreenHeight, GameConfig::WindowTitle);
	SetTargetFPS(GameConfig::TargetFPS);

	//Crate Player and add it to the game
	Player* player = new Player(20, 0, 3);
	player->x = 100;
	player->y = 260;
	player->lookingRight = true;
	int walkSpeed = 5;

	//Create Monster and add it to the game
	Monster* Target = new Monster(1);
	Target->x = 650;
	Target->y = 260;
	Target->lookingRight = false;

	std::string Message = " ";

	float combatStartTimer = 3.f;
	float defaultMessageDisplayTimer = 1.5f;

	//map woring
	Texture2D MapTileMapTexture = LoadTexture("resources/walls_floor.png");
	SetTextureFilter(MapTileMapTexture, TEXTURE_FILTER_POINT);
	MapHandler::LoadMap("resources/Level1.txt");

	int tileSize = 32;

	Rectangle MapTileRecs[20] = {
		{ 192.0f, 336.0f, 16.0f, 16.0f },//Tiled Floor 3 {0}
		{ 7.0f, 85.0f, 16.0f, 16.0f },//Floor {1}
		{ 176.0f, 336.0f, 16.0f, 16.0f },//Tiled Floor 1 {2}
		{ 176.0f, 352.0f, 16.0f, 16.0f },//Tiled Floor 2 {3}
		{ 192.0f, 352.0f, 16.0f, 16.0f },//Tiled Floor 4 {4}
		{ 16.0f, 64.0f, 16.0f, 16.0f },//Wall Bottom {5}
		{ 16.0f, 48.0f, 16.0f, 16.0f },//Wall Middle {6}
		{ 16.0f, 32.0f, 16.0f, 16.0f },//Wall Top {7}
		{ 16.0f, 0.0f, 16.0f, 16.0f },//Wall back {8}
		{ 16.0f, 16.0f, 16.0f, 16.0f },//Wall black {9}
		{ 64.0f, 112.0f, 16.0f, 16.0f },//floor wall corner {10}
	};
	//Main game loop
	while (!WindowShouldClose()) {
#pragma region Update Logic
		// --- A.Update Logic update in background ---
		StateMachine::MessageDisplayTimer += GetFrameTime();
		combatStartTimer += GetFrameTime();
		Vector2 mousePos = GetMousePosition();

		//Set rectangles for the attack and potion buttons
		Rectangle attackButton = { 100, 500, 150, 40 };
		Rectangle potionButton = { 100, 550, 150, 40 };

		if (StateMachine::CurrentState == MAP_MODE) {
			// WASD buttons to move the player
			bool wasWalking = player->isWalking;
			player->isWalking = false;
			if (IsKeyDown(KEY_W)) {
				player->isWalking = true;
				Rectangle nextHitbox = {(float)player->x, (float)player->y - walkSpeed, 50, 50};
				if (!MapHandler::CheckCollision(nextHitbox, tileSize)) player->y -= walkSpeed;//Move up
			}
			if (IsKeyDown(KEY_S)) {
				player->isWalking = true;
				Rectangle nextHitbox = { (float)player->x, (float)player->y + walkSpeed, 50, 50 };
				if (!MapHandler::CheckCollision(nextHitbox, tileSize)) player->y += walkSpeed;//Move down
			}
			if (IsKeyDown(KEY_A)) {
				player->lookingRight = false;
				player->isWalking = true;
				Rectangle nextHitbox = { (float)player->x - walkSpeed, (float)player->y, 50, 50 };
				if (!MapHandler::CheckCollision(nextHitbox, tileSize)) player->x -= walkSpeed;//Move left
			}
			if (IsKeyDown(KEY_D)) {
				player->lookingRight = true;
				player->isWalking = true;
				Rectangle nextHitbox = { (float)player->x + walkSpeed, (float)player->y, 50, 50 };
				if (!MapHandler::CheckCollision(nextHitbox, tileSize)) player->x += walkSpeed;//Move right
			}

			//Draw Hitbox
			Rectangle playerHitbox = { (float)player->x, (float)player->y,50,50 };
			Rectangle targetHitbox = { (float)Target->x, (float)Target->y,50,50 };

			if (wasWalking && !player->isWalking) {
				player->walkingAnimationTimer = 0.f;//Reset walking animation timer when player stops walking to reset the animation to the first frame when the player starts walking again
			}

			if (CheckCollisionRecs(playerHitbox, targetHitbox)) {
				StateMachine::CurrentState = COMBAT_MODE;//Switch to combat mode when player collides with the monster
				player->isWalking = false;
				player->lookingRight = true;

				CombatPosition(*player, *Target);
			
				Target->BarColor = RED;//Change monster color to red to indicate combat mode
				combatStartTimer = 0.f;
			}
		}
		else if (StateMachine::CurrentState == COMBAT_MODE) {
			//Combat logic goes here , for now we will just switch back to map mode after 3 seconds
			if (StateMachine::CurrentCombatPhase == PLAYER_TURN && combatStartTimer > 3.0f) {
				//check if the mouse button is pressed
				if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, attackButton)) || IsKeyPressed(KEY_ONE)) {//Players wants to attack
					Message = TryHit(*player, *Target);
				}
				else if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePos, potionButton)) || IsKeyPressed(KEY_TWO)) {
					Message = TryUsePotion(*player);
				}
			}
			else if (StateMachine::CurrentCombatPhase == ENEMY_TURN) {
				if(StateMachine::MessageDisplayTimer > defaultMessageDisplayTimer){
					if (Target->Health <= 0) {
						Message = TextFormat("You Killed the enemy!!!");
						StateMachine::MessageDisplayTimer = 0.f;
						StateMachine::Score += 10;
						StateMachine::Wave += 1;
						player->DamageBonus += 1;
						delete Target;
						Target = new Monster(StateMachine::Wave);
						Target->lookingRight = false;
						ResetPosition(*player, *Target);
						StateMachine::ChangeState(GameState::MAP_MODE);//Switch back to map mode after killing the monster
						StateMachine::ChangeCombatPhase(CombatPhase::PLAYER_TURN);//Reset combat phase to player turn for the next combat
						Target->BarColor = ORANGE;//Change monster color back to orange to indicate map mode
					}
					else {
						Message = TryHit(*Target, *player);
					}
					if (player->Health <= 0) {
						Message = TextFormat("You Died !!!");
						StateMachine::ChangeState(GameState::GAME_OVER);//Switch to game over state after player dies
					}
				}
			}
			if (StateMachine::InfluenceDamage) {
				player->InfluenceDamage();
				Target->InfluenceDamage();
				StateMachine::InfluenceDamage = false;
			}

			
		}
#pragma endregion
		
#pragma region Draw Logic
		// --- B.Draw Logic update in foreground ---
		BeginDrawing();
		ClearBackground(RAYWHITE);
		//Draw Map
		int currentX = 0;
		int currentY = 0;

		for (int y = 0; y < MapHandler::LevelData.size(); y++)
		{
			currentX = 0;
			for (int x = 0; x < MapHandler::LevelData[y].size(); x++) {
				int tileID = MapHandler::LevelData[y][x];
				if (tileID > 20 || tileID < 0) tileID = 0;//If the tile ID is out of bounds we can set it to 0 to avoid errors
				Rectangle sourceRec = { 0.0f, 0.0f, 16.0f, 16.0f };

				Rectangle destRec = { (float)currentX, (float)currentY, 32.0f, 32.0f };
				DrawTexturePro(MapTileMapTexture, MapTileRecs[3], destRec, { 0, 0 }, 0.0f, WHITE);
				DrawTexturePro(MapTileMapTexture, MapTileRecs[tileID], destRec, { 0, 0 }, 0.0f, WHITE);
				currentX += tileSize;
			}
			currentY += tileSize;
		}

		//set a vector for player position to use in drawing the player sprite
		Vector2 playerPos = { (float)player->x, (float)player->y };
		Rectangle destRec = { (float)playerPos.x + 25.0f, (float)playerPos.y + 25.0f, player->ScaledSize, player->ScaledSize };
		Vector2 origin = { player->ScaledSize / 2, player->ScaledSize / 2 };
		player->UpdateAnimation(GetFrameTime());
		//set a vector for monster position to use in drawing the monster sprite
		Vector2 targetPos = { (float)Target->x, (float)Target->y };
		Rectangle targetDestRec = { (float)targetPos.x + 25.0f, (float)targetPos.y + 25.0f, Target->ScaledSize, Target->ScaledSize };
		Vector2 targetOrigin = { Target->ScaledSize / 2, Target->ScaledSize / 2 };
		
		Target->UpdateAnimation(GetFrameTime());

		//Draw Player and Monster
		DrawTexturePro(Target->Sprite, Target->FrameRec, targetDestRec, targetOrigin, 0.0f, Target->SpriteTint);
		DrawTexturePro(player->Sprite, player->FrameRec, destRec, origin, 0.0f, player->SpriteTint);

		UIHandler::DrawGameHUD(*player, *Target);

		//if the Combat statrt timer is smaller than 3 seconds we can draw a "Combat Starting in..." text to indicate the player that combat is about to start
		if (combatStartTimer < 3 && StateMachine::CurrentState == COMBAT_MODE) {
			DrawRectangle(250, 100, 300, 40, LIGHTGRAY);
			DrawText(TextFormat("Combat Begins..."), 260, 110, 20, BLACK);
		}

		//Draw Message if there is one and the message display timer is smaller than 3 seconds
		if ((int)StateMachine::MessageDisplayTimer < (int)defaultMessageDisplayTimer && Message != " " && StateMachine::CurrentState == COMBAT_MODE) {
			DrawRectangle(250, 100, 300, 40, LIGHTGRAY);
			DrawText(Message.c_str(), (int)260, 110, 20, BLACK);
		}

#pragma endregion

		EndDrawing();
	}

	//Clear Memory and Exit
	delete Target;
	delete player;
	CloseWindow();

	return 0;

}