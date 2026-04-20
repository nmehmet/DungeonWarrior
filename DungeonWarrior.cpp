#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Combat.h"


int main() {
	srand(time(0));

	Player mainChar = Player(20,0,3);
	Monster* Target = new Monster(1);
	int option;
	int wave = 1;
	int score = 0;

	while (mainChar.Health > 0) {	

		//Main UI 
		std::cout << "||Wave : "<< wave << "|| Players Health :" << mainChar.Health << "  Potions: ";
		PotionWrite(mainChar.PotionCount);
		std::cout << "Damage Bonus: "<<mainChar.DamageBonus << "     Targets Health: " << Target->Health << std::endl;

		// Your turn
		std::cout << " Please choose an action  Attack : 1    Use Potion : 2 ";
		std::cin >> option; 
		std::cout << std::endl;

		switch (option)
		{
		case 1 : 
			//If the player chooses to attack.
			if (CanHit(8)) {// If the player can hit the target.
				int damage = GetDamage(mainChar.DamageBonus);
				Hit(*Target, damage);
				std::cout << damage << " Damage given!" << std::endl;
			}
			// If the player can't hit the target.
			else std::cout << "Missed..." << std::endl; break;
		case 2 :
			// If the player chooses to use a potion.
			if (mainChar.PotionCount > 0) {// If the player has potions.
				mainChar.PotionCount--;
				if ((mainChar.Health + 6) > 20) mainChar.Health = 20;
				else mainChar.Health += 6;
				std::cout << "Healed !" << std::endl;
			}
			// If the player doesn't have any potions.
			else std::cout << "Not enough potions !!";
			break;
		default:
			std::cout << "Wrong Choice !!!"<< std::endl;
			break;
		}
		// If the target is killed.
		if (Target->Health <= 0) {
			std::cout << "You Killed the enemy!!!" << std::endl;
			score += 10;
			wave += 1;
			mainChar.DamageBonus += 1;
			delete Target;
			Target = new Monster(wave);
			std::cout << std::endl << "==================================================================================" << std::endl << std::endl;
			continue;
		}

		// Enemys turn
		std::cout << "Enemys Turn..." << std::endl << " Enemy is trying to hit..." << std::endl;
		if (CanHit(8)) {
			int npcDamage = GetDamage(wave);
			Hit(mainChar, npcDamage);
			std::cout << "Taken " << npcDamage << " damage!!!" << std::endl;
		}
		else std::cout << "Enemy Missed !!!!" << std::endl;

		std::cout << std::endl << "==================================================================================" << std::endl << std::endl;
	}
	std::cout << "You died... Your Score : " << score << std::endl;
	std::cin.get();

	return 0;
}

