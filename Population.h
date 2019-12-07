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
	void update(float, Area& area);
	void reset();
	void replace(Population& newGen);
	bool allDotsDead();
	Population() {};
	float getFitnessSum() { return fitnessSum; };
	float getFitnessAverage() { return fitnessSum / size; };
	Population(int);
	Player getChamp() { return champ; };
	
	~Population();

	void calculateFitness();

	//Genetic Algorithm methods
	Player naturalSelection(Population &pop);
	Player mutate(Player &parentOne, Player &parentTwo);

	//ABC algorithm methods
	Player compareFitness(Player& employed, Player& candidate);
	Player generateCanditate(Population& pop, Player& employedBee);
	Player waggleDance(Population& pop);
	void abandonFoodResource(Population& pop, Player& champ);
};