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

	Population() {};
	Population(int);
	~Population();

	void calculateFitness();
	Player naturalSelection();
	Player mutate(Player , Player);
	void update(float, Area &area);
	void reset();
	void replace(Population &newGen);
	bool allDotsDead();

};