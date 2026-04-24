#pragma once
#include <string>

struct Character
{
	//------Base Character struct to hold common attributes and functions for both player and monster
	//Base attributes for both player and monster
	int Health = 0;
	int MaxHealth = 0;
	int DamageBonus = 0;
	int AC = 0;
	int x = 0;
	int y = 0;
	//Base attributes for animation and sprite handling
	Texture2D Sprite = {};
	Rectangle FrameRec = {};
	float Scale = 0.0f;
	float ScaledSize = 0.0f;
	float Rotation = 0.0f;
	//Animation timers
	float idleAnimationTimer = 0.f;
	float walkingAnimationTimer = 0.f;
	//Animation speeds 
	float idleAnimationSpeed = 0.0f;
	float walkingAnimationSpeed = 0.0f;
	//Animation frame counts
	int idleAnimationFrameCount = 0;
	int walkingAnimationFrameCount = 0;
	//Y position in the sprite sheet for the walking animation frames
	float idleAnimationY = 0.0f;
	float walkingAnimationY = 0.0f;

	bool isWalking = false;
	bool isAttacking = false;
	bool lookingRight = true;

	virtual ~Character() {
		UnloadTexture(Sprite);
	};
};
struct Player:Character
{
	int PotionCount = 0;
	bool isUsingPotion = false;
	
	Player(int _hp, int _dmgBonus, int _potionCount)
	{
		Health = _hp ? _hp : 0;
		MaxHealth = Health;
		DamageBonus = _dmgBonus ? _dmgBonus : 0;
		PotionCount = _potionCount ? _potionCount : 0;
		AC = 10;
		//Loading and editing player texture
		Sprite = LoadTexture("resources/Soldier.png");
		SetTextureFilter(Sprite, TEXTURE_FILTER_POINT);
		Scale = 2.5f;
		ScaledSize = 100.0f * Scale;
		FrameRec = { 0.0f, 0.0f, 100.0f, 100.0f };

		idleAnimationFrameCount = 6;
		idleAnimationSpeed = 0.25f;

		walkingAnimationFrameCount = 8;
		walkingAnimationSpeed = 0.10f;
		walkingAnimationY = 100.0f;
	}
	
	void UpdateAnimation(float deltaTime) {
		if (isWalking) {
			walkingAnimationTimer += deltaTime;
			int frame = (int)(walkingAnimationTimer / walkingAnimationSpeed) % walkingAnimationFrameCount;
			FrameRec = { (float)(frame * 100), walkingAnimationY, 100.0f, 100.0f };
		}
		else {
			idleAnimationTimer += deltaTime;
			int frame = (int)(idleAnimationTimer / idleAnimationSpeed) % idleAnimationFrameCount;
			FrameRec = { (float)(frame * 100), idleAnimationY, 100.0f, 100.0f };
		}

		if(!lookingRight) FrameRec.width = -100.0f; // Flip the sprite horizontally when looking left
		else FrameRec.width = 100.0f; // Reset to normal width when looking right
	}
};
struct Monster : Character
{
	
	Monster(int _wave)
	{
		Health = 5 + _wave;
		MaxHealth = Health;
		DamageBonus = _wave;
		AC = 5 + _wave;
		Sprite = LoadTexture("resources/Orc.png");
		SetTextureFilter(Sprite, TEXTURE_FILTER_POINT);
		Scale = 2.5f;
		ScaledSize = 100.0f * Scale;
		FrameRec = { 0.0f, 0.0f, 100.0f, 100.0f };

		idleAnimationFrameCount = 6;
		idleAnimationSpeed = 0.25f;

		walkingAnimationFrameCount = 8;
		walkingAnimationSpeed = 0.10f;
		walkingAnimationY = 100.0f;
	}

	void UpdateAnimation(float deltaTime) {
		if (isWalking) {
			walkingAnimationTimer += deltaTime;
			int frame = (int)(walkingAnimationTimer / walkingAnimationSpeed) % walkingAnimationFrameCount;
			FrameRec = { (float)(frame * 100), walkingAnimationY, 100.0f, 100.0f };
		}
		else {
			idleAnimationTimer += deltaTime;
			int frame = (int)(idleAnimationTimer / idleAnimationSpeed) % idleAnimationFrameCount;
			FrameRec = { (float)(frame * 100), idleAnimationY, 100.0f, 100.0f };
		}
		if (!lookingRight) FrameRec.width = -100.0f; // Flip the sprite horizontally when looking left
		else FrameRec.width = 100.0f; // Reset to normal width when looking right
	}
};

void PotionWrite(int _potionCount);
bool CanHit(int _diff);
int GetDamage(int bonus);
void Hit(Character& character, int damage);
std::string TryHit(Character& _hitter, Character& _target);
std::string TryUsePotion(Player& _player);
