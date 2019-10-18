#include "Population.h"


Population::Population(int size)
{
	players.resize(size);
	for (int i = 0; i < players.size(); i++) {
		players[i] = Player();
	}
}

Population::~Population()
{
}

void Population::calculateFitness()
{
	for (int i = 0; i < players.size(); i++) {
		fitnessSum += players[i].calculateFitness();
	}
	for (int i = 0; i < players.size(); i++) {
		players[i].normalizeFitness(fitnessSum);
	}
	std::cout << fitnessSum << std::endl;
}

Player Population::naturalSelection()
{
	std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	srand(ms.count());
	float pool = (float)(rand() % 10000) / 10000.f;
	for (int i = 0; i < players.size(); i++) {
		if (players[i].getFitness() > pool) {
			return players[i];
		}
		else {
			pool -= players[i].getFitness();
		}
	}
	return players[players.size() - 1];
}

Player Population::mutate(Player parentOne, Player parentTwo)
{
	Player child = Player();
	child.getShape().setFillColor(parentOne.getShape().getFillColor() + parentTwo.getShape().getFillColor());
	child.mutate(parentOne.getDNA(), parentTwo.getDNA());
	return child;
}

void Population::update(float dt, Area &area)
{
	for (int i = 0; i < players.size(); i++) {
		players[i].updatePosition(dt);
		area.check4Collision(players[i]);
	}

}

void Population::reset()
{
	fitnessSum = 0;
	for (int i = 0; i < players.size(); i++) {
		players[i].reset();
	}
}

void Population::replace(Population &newGen)
{
	for (int i = 0; i < players.size(); i++) {
		players[i] = newGen.players[i];
	}
}

bool Population::allDotsDead()
{
	for (int i = 0; i < players.size(); i++) {
		if (!players[i].isDead() && !players[i].reachedGoal()) return false;
	}
	
	return true;
}
