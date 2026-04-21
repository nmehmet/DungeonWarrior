#include <iostream>
#include "raylib.h"
#include "Combat.h"
#include <cstdlib>
#include <ctime>
#include <string>
#include <typeinfo>

void PotionWrite(int _potionCount) {
	for (int i = 1; i <= 3; i++)
	{
		if (i <= _potionCount)
		{
			std::cout << "[] ";
		}
	}
}
bool CanHit(int diff) {
	if ((rand() % 20) > diff) return true;
	else return false;
}
int GetDamage(int bonus) {

	return (rand() % 6) + 1 + bonus;
}
void Hit(Character& character, int damage) {
	character.Health -= damage;
}
std::string TryHit(Character& _hitter, Character& _target) {
	if (CanHit(_target.AC)) {
		int damage = GetDamage(_hitter.DamageBonus);
		Hit(_target, damage);
		if (typeid(_hitter) == typeid(Player)) {
			return TextFormat("%d Damage given!", damage);
		}
		else if (typeid(_hitter) == typeid(Monster)) {
			return TextFormat("%d Damage taken!", damage);
		}
		else return " ";
	}
	else {
		if (typeid(_hitter) == typeid(Player)) {
			return TextFormat("You missed the hit !!");
		}
		else if (typeid(_hitter) == typeid(Monster)) {
			return TextFormat("Enemy Missed the hit !!");
		}
		else return " ";
	}
}

std::string TryUsePotion(Player& _player) {
	if (_player.PotionCount > 0) {
		_player.PotionCount--;
		if (_player.Health + 6 > 20) _player.Health = 20;
		else _player.Health += 6;

		return TextFormat("Healed !");
	}
	else {
		return TextFormat("Not enough potions !!");
	}
}