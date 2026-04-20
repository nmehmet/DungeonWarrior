#include "Combat.h"
#include <iostream>
#include <cstdlib>

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