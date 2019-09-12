#ifndef UNIT_H
#define UNIT_H

#include <string>
#include <misc/resources.h>
#include <memory>

class WorldSettings;
class Unit
{
	public:
		std::string name;
		Resources cost;
		int pop;
		float speed;
		int buildTime;
		int attack;
		int defence;
		int defenceCavalry;
		int defenceArcher;
		bool buildingAttackMultiplier; //TODO: PERHAPS USE FLOAT, RESEARCH
		bool additionalMaxWallNegation; //TODO: PERHAPS USE FLOAT, RESEARCH
		int carry;
		int stealth;
		int perception;
		bool canAttack;
		bool canSupport;
		WorldSettings *worldSettings;
		Unit ( std::string xmlData, WorldSettings *worldSettings );
		int unitCount;
		std::unique_ptr<Unit> clone();
};

#endif // UNIT_H
