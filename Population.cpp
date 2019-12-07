#include "Population.h"

/*
Constructor that initializes players according to the size that is desired
*/
Population::Population(int size)
{
	this->size = size;
	players.resize(size);
	for (int i = 0; i < players.size(); i++) {
		players[i] = Player();
	}
}

/*
Destructor
*/
Population::~Population()
{
}

/*
update method 
*/
void Population::update(float dt, Area& area)
{
	for (int i = 0; i < players.size(); i++) {
		players[i].updatePosition(dt); // update all Players
		area.check4Collision(players[i]); // check every Player if it collides with a Platform of the area they are in
	}

}
/*
reset function if a population has finished
*/
void Population::reset()
{
	fitnessSum = 0;
	for (int i = 0; i < players.size(); i++) {
		players[i].reset();
	}
}

/*
replaces the old population with the new one
*/
void Population::replace(Population& newGen)
{
	for (int i = 0; i < players.size(); i++) {
		players[i] = newGen.players[i];
		players[i].setColor(newGen.players[i].getColor()); // this is called to ensure that the color gets into the next generation, sometime it did not work
	}
}

/*
checks if all players are dead
*/
bool Population::allDotsDead()
{
	for (int i = 0; i < players.size(); i++) {
		if (!players[i].isDead() && !players[i].reachedGoal()) return false; // if one player is not dead and has not reached the end the loop breaks
	}
	return true;
}

/*

methods both algorithm use

*/

/*
calculates fitness
*/
void Population::calculateFitness()
{
	float fitnessScore = fitnessSum;
	for (int i = 0; i < players.size(); i++) {
		if (players[i].getFitness() > champ.getFitness()) champ = players[i]; champ.crown(sf::Color::Red);
		fitnessScore += players[i].calculateFitness(); //Fitness Calculation can differ from the actual fitness this is why here are two floats
		fitnessSum += players[i].getFitness();
	}

	for (int i = 0; i < players.size(); i++) {
		players[i].normalizeFitness(fitnessScore); // score gets normalized so all fitness values now sum up to 1
	}
	std::cout << "FitnessSum: " << fitnessSum / players.size() << std::endl;
}





/*

Genetic algorithm methods

*/


/*
Natural Selection
*/
Player Population::naturalSelection(Population &pop)
{
	std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	srand(ms.count()); //Seed to make random numbers unique as otherwise c++ always produces the same "random" numbers
	float pool = (float)(rand() % 10000) / 10000.f; // generate number between 0 and 1
	for (int i = 0; i < pop.players.size(); i++) {
		if (pop.players[i].getFitness() > pool) { // if generated number is lower than the players fitness choose this player
			return pop.players[i];
		}
		else {
			pool -= pop.players[i].getFitness(); // otherwise substract this players fitness from the generated number
		}
	}
	return champ; // it should never reach this point, but if it does return the champ
}

/*
Crossover and Mutation in one method to reduce overhead
*/
Player Population::mutate(Player &parentOne, Player &parentTwo)
{
	Player child = Player(); 
	child.setColor(parentOne.getColor()); // inherit color of parent one
	DNA DNA1 = parentOne.getDNA();
	DNA DNA2 = parentTwo.getDNA();
	child.mutate(DNA1, DNA2);
	return child;
}




/* 

ABC algorithm methods 

*/

/*
compares Fitness of employed bee and its candidate solution, returns the better one
*/
Player Population::compareFitness(Player& employed, Player& candidate)
{
	if (employed.getFitness() > candidate.getFitness()) {
		return employed;
	}
	else {
		return candidate;
	}
}

/*
each bee creates a candidate solution to see if it improves the path it choose from the waggle dance
*/
Player Population::generateCanditate(Population& pop, Player& employedBee) {
	std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	srand(ms.count()); // random seed
	Player candidate = employedBee;
	candidate.setColor(employedBee.getColor());
	DNA newCandidate = employedBee.getDNA();
	int randomPartner = rand() % size / 2; // gets a random other path to make adjustments
	int randomInput = rand() % (employedBee.getDNA().getStep() + 1); // changes an input between the first one and the last one the employed bee used before
	if (pop.players[randomPartner].getNectarCounter() < 20 - champ.getStep()) { // if one bees solution is choosen to often, the bees become scouts
		float phi = rand() % 10000;
		phi = (phi / 5000.f) - 1.f;

		newCandidate.inputs[randomInput] = employedBee.getDNA().inputs[randomInput] + phi * (employedBee.getDNA().inputs[randomInput] - pop.players[randomPartner].getDNA().inputs[randomInput]);
		pop.players[randomPartner].nectarPlusPlus(); 
	}
	else {
		newCandidate.randomize(randomInput);
	}

	candidate.setDNA(newCandidate);

	return candidate;
}

/*
basically Natural Selection
*/
Player Population::waggleDance(Population& pop)
{
	return this->naturalSelection(pop);
}

/*
Emergency behaviour if the swarm is not able to improve over a certain amount of cycles
*/
void Population::abandonFoodResource(Population& pop, Player& champ)
{
	std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	srand(ms.count()); //random seed
	DNA champDNA = champ.getDNA();
	for (int i = 0; i < pop.players.size(); i++) { // each bee searches a new food resource at the point before the champ got stuck
		float phi = rand() % 10000;
		phi = phi / 10000.f;
		pop.players[i].setDNA(champDNA.getStep() - 1, champDNA.getLowerBound() + phi * (champDNA.getUpperBound() - champDNA.getLowerBound()));
	}

}
