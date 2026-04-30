#pragma once

enum GameState {
	MAP_MODE,//Player can move around the map and encounter monsters
	COMBAT_MODE,//Player is in combat with a monster, can attack or use potions
	GAME_OVER,//Player has died, show game over screen
};
enum CombatPhase {
	PLAYER_TURN,//Player can choose to attack or use potion
	ENEMY_TURN,//Enemy will attack the player
	WAITING,//Waiting phase between player and enemy turns to show messages and animations 
};

struct StateMachine {
	static GameState CurrentState;
	static CombatPhase CurrentCombatPhase;
	static int Score;
	static int Wave;
	static float MessageDisplayTimer;
	static bool InfluenceDamage;
	static void ChangeState(GameState newState);
	static void ChangeCombatPhase(CombatPhase newPhase);
};
