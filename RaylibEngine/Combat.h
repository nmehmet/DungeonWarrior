#pragma once
#include <string>

struct Character
{
	int Health = 0;
	int DamageBonus = 0;
	int AC = 0;
	int x = 0;
	int y = 0;
	virtual ~Character() {};
};
struct Player:Character
{
	int PotionCount = 0;
	
	Player(int _hp, int _dmgBonus, int _potionCount)
	{
		Health = _hp ? _hp : 0;
		DamageBonus = _dmgBonus ? _dmgBonus : 0;
		PotionCount = _potionCount ? _potionCount : 0;
		AC = 10;
	}
};
struct Monster : Character
{
	Monster(int _wave)
	{
		Health = 5 + _wave;
		DamageBonus = _wave;
		AC = 5 + _wave;
	}
};

void PotionWrite(int _potionCount);
bool CanHit(int _diff);
int GetDamage(int bonus);
void Hit(Character& character, int damage);
std::string TryHit(Character& _hitter, Character& _target);
std::string TryUsePotion(Player& _player);
