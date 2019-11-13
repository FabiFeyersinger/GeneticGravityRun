#pragma once
#include "Player.h";
#include "Area.h";
#include <vector>;
#include <iostream>;
#include <chrono>;

class Population
{
public:
	//common methods
	std::vector<Player> players;
	float fitnessSum = 0;
	int size = 200;
	Player champ;
	Player mutate(Player& parent);
	void update(float, Area& area);
	void reset();
	void replace(Population& newGen);
	bool allDotsDead();
	Population() {};
	Population(int);

	
	~Population();

	void calculateFitness();

	//Genetic Algorithm methods
	Player naturalSelection(Population &pop);
	Player mutate(Player &parentOne, Player &parentTwo);

	//ABC algorithm methods
	void generateFirstGeneration(Population &pop);
	Player compareFitness(Player& employed, Player& candidate);
	Player generateCanditate(Population& pop, Player& employedBee);
	Player waggleDance(Population& pop);
};