#pragma once
#include "Player.h";
#include "Area.h";
#include <vector>;
#include <iostream>;
#include <chrono>;

class Population
{
public:
	std::vector<Player> players;
	float fitnessSum = 0;
	Player champ;

	Population() {};
	Population(int);
	~Population();

	void calculateFitness();
	Player naturalSelection(Population &pop);
	Player mutate(Player &parentOne, Player &parentTwo);
	Player mutate(Player& parent);
	void update(float, Area &area);
	void reset();
	void replace(Population &newGen);
	bool allDotsDead();

};