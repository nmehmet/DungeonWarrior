#include "raylib.h"
#include "Combat.h"
#include "GameConfig.h"
#include "UIHandler.h"
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
	player.y = 200;
	monster.x = 500;
	monster.y = 200;
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

	//Create Monster and add it to the game
	Monster* Target = new Monster(1);
	Target->x = 650;
	Target->y = 260;
	Target->lookingRight = false;

	std::string Message = " ";

	float combatStartTimer = 3.f;
	float messageDisplayTimer = 3.f;
	float defaultMessageDisplayTimer = 1.5f;

	//Main game loop
	while (!WindowShouldClose()) {
#pragma region Update Logic
		// --- A.Update Logic update in background ---
		messageDisplayTimer += GetFrameTime();
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
				if(player->y >= GameConfig::MapBoundaryMinY)player->y -= 5;//Move up
			}
			if (IsKeyDown(KEY_S)) {
				player->isWalking = true;
				if(player->y <= GameConfig::MapBoundaryMaxY)player->y += 5;//Move down
			}
			if (IsKeyDown(KEY_A)) {
				player->lookingRight = false;
				player->isWalking = true;
				if(player->x >= GameConfig::MapBoundaryMinX)player->x -= 5;//Move left
			}
			if (IsKeyDown(KEY_D)) {
				player->lookingRight = true;
				player->isWalking = true;
				if(player->x <= GameConfig::MapBoundaryMaxX)player->x += 5;//Move right
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
				if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
					if (CheckCollisionPointRec(mousePos, attackButton)) {
						Message = TryHit(*player, *Target);
						messageDisplayTimer = 0.f;
						StateMachine::ChangeCombatPhase(CombatPhase::ENEMY_TURN);//Switch to enemy turn after player action
					}
					else if (CheckCollisionPointRec(mousePos, potionButton)) {
						Message = TryUsePotion(*player);
						messageDisplayTimer = 0.f;
						StateMachine::ChangeCombatPhase(CombatPhase::ENEMY_TURN);
					}
				}
				if (IsKeyPressed(KEY_ONE)) {//Player Wants to attack
					
					Message = TryHit(*player, *Target);
					messageDisplayTimer = 0.f;
					StateMachine::ChangeCombatPhase(CombatPhase::ENEMY_TURN);//Switch to enemy turn after player action
				}
				else if (IsKeyPressed(KEY_TWO)) {
					Message = TryUsePotion(*player);
					messageDisplayTimer = 0.f;
					StateMachine::ChangeCombatPhase(CombatPhase::ENEMY_TURN);//Switch to enemy turn after player action
				}
			}
			else if (StateMachine::CurrentCombatPhase == ENEMY_TURN) {
				if(messageDisplayTimer > defaultMessageDisplayTimer){
					if (Target->Health <= 0) {
						Message = TextFormat("You Killed the enemy!!!");
						messageDisplayTimer = 0.f;
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
						messageDisplayTimer = 0.f;
						StateMachine::ChangeCombatPhase(CombatPhase::PLAYER_TURN);//Switch to enemy turn after player action
					}
					if (player->Health <= 0) {
						Message = TextFormat("You Died !!!");
						messageDisplayTimer = 0.f;
						StateMachine::ChangeState(GameState::GAME_OVER);//Switch to game over state after player dies
					}
				}
			}

			
		}
#pragma endregion
		
#pragma region Draw Logic
		// --- B.Draw Logic update in foreground ---
		BeginDrawing();
		ClearBackground(RAYWHITE);

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
		DrawTexturePro(Target->Sprite, Target->FrameRec, targetDestRec, targetOrigin, 0.0f, WHITE);
		DrawTexturePro(player->Sprite, player->FrameRec, destRec, origin, 0.0f, WHITE);

		UIHandler::DrawGameHUD(*player, *Target);

		//if the Combat statrt timer is smaller than 3 seconds we can draw a "Combat Starting in..." text to indicate the player that combat is about to start
		if (combatStartTimer < 3 && StateMachine::CurrentState == COMBAT_MODE) {
			DrawRectangle(250, 100, 300, 40, LIGHTGRAY);
			DrawText(TextFormat("Combat Begins..."), 260, 110, 20, BLACK);
		}

		//Draw Message if there is one and the message display timer is smaller than 3 seconds
		if ((int)messageDisplayTimer < (int)defaultMessageDisplayTimer && Message != " " && StateMachine::CurrentState == COMBAT_MODE) {
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