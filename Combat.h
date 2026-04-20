#pragma once

struct Character
{
	int Health;
	int DamageBonus;
	int x;
	int y;
};
struct Player:Character
{
	int PotionCount;
	
	Player(int _hp, int _dmgBonus, int _potionCount)
	{
		Health = _hp;
		DamageBonus = _dmgBonus;
		PotionCount = _potionCount;
	}
};
struct Monster : Character
{
	Monster(int _wave)
	{
		Health = 5 + _wave;
		DamageBonus = _wave;
	}
};

void PotionWrite(int _potionCount);
bool CanHit(int _diff);
int GetDamage(int bonus);
void Hit(Character& character, int damage);
