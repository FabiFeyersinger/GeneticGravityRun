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
		if (players[i].getFitness() > champ.getFitness()) champ = players[i];
		fitnessSum += players[i].calculateFitness();
	}
	for (int i = 0; i < players.size(); i++) {
		players[i].normalizeFitness(fitnessSum);
	}
	std::cout << "Fitness average: " << fitnessSum / 200 << std::endl;
}

Player Population::naturalSelection(Population &pop)
{
	std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	srand(ms.count());
	float pool = (float)(rand() % 10000) / 10000.f;
	for (int i = 0; i < pop.players.size(); i++) {
		if (pop.players[i].getFitness() > pool) {
			//std::cout << "Parent First jump :" << players[i].getDNA().inputs[0] << std::endl;
			return pop.players[i];
		}
		else {
			pool -= pop.players[i].getFitness();
		}
	}
	return champ;
}

Player Population::mutate(Player &parentOne, Player &parentTwo)
{
	Player child = Player();
	//std::cout << "Color ParentOne:" << parentOne.getColor().toInteger() << std::endl;
	child.setColor(parentOne.getColor());
	//std::cout << "Color Child:" << child.getColor().toInteger() << std::endl;
	DNA DNA1 = parentOne.getDNA();
	DNA DNA2 = parentTwo.getDNA();
	child.mutate(DNA1, DNA2);
	return child;
}

Player Population::mutate(Player& parent)
{
	Player child = Player();
	//std::cout << "Color ParentOne:" << parentOne.getColor().toInteger() << std::endl;
	child.setColor(parent.getColor());
	//std::cout << "Color Child:" << child.getColor().toInteger() << std::endl;
	child.mutate(parent.getDNA());
	std::cout << parent.getDNA().inputs[0] << std::endl;
	std::cout << child.getDNA().inputs[0] << std::endl;
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
		players[i].setColor(newGen.players[i].getColor());
	}
}

bool Population::allDotsDead()
{
	for (int i = 0; i < players.size(); i++) {
		if (!players[i].isDead() && !players[i].reachedGoal()) return false;
	}
	
	return true;
}
