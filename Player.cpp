#include "Player.h"

/*
Constructor inits DNA and sets position, shape and gravity
*/
Player::Player()
{
	shape = RectangleShape();
	shape.setSize(Vector2f(32.f, 32.f));
	shape.setFillColor(Color::Red);
	speed = 0.f;
	gravity = 6.f;
	position = Vector2f(10, 256);
	dna = DNA(100);
}

/*
Destructor
*/
Player::~Player()
{
}

/*
standard update method, also increases fitness value as long as it is alive
*/
float Player::updatePosition(float dt)
{
	if (position.y < 0 || position.y > 512) dead = true;
	if (!dead) {
		fitness++;
		useDNA(dt);
	}
	updateGravity(dt);
	shape.setPosition(position);
	return 0.0f;
}

/*
update gravity
*/
void Player::updateGravity(float dt)
{
	if (up) {
			position.y -= gravity;
			jumping = true; // to avoid players abusing jumping infinetly
	}
	else {
			position.y += gravity;
			jumping = true;
	}

}

/*
gets called by platform and tells the player to adjust his y position
*/
void Player::platformCollide(float y)
{
	this->position.y = y;
	jumping = false; // player can now jump again
}


/*
jump method, called by useDNA
*/
void Player::jump()
{
	if (!jumping) {
		up = !up;
	}
}

/*
resets important variables
*/
void Player::reset()
{
	up = false;
	dead = false;
	jumping = true;
	position = Vector2f(10, 256);
	fitness = 0;
	nectarCounter = 0;
	dtcounter = 0;
	dna.setStep(0);
}

/*
crown best player to make it easier to recognize
*/
void Player::crown(sf::Color color)
{
	shape.setOutlineColor(color);
	shape.setOutlineThickness(4);
}

/*

Methods used in both algorithms

*/


/*
Calculate Fitness
*/
float Player::calculateFitness()
{
	//return fitness * 10; // plain fitness
	return std::pow(2,fitness); //exponential fitness
	//return fitness * fitness; // squared fitness
}

/*
normalize Fitness for pool selection
*/
void Player::normalizeFitness(float sum)
{
	fitness = fitness / sum;
}

/*
This method is a core part for the whole project, detemines when a player decides to jump
*/
void Player::useDNA(float dt)
{
	if (dna.getLenght() > dna.step) {
		if (dna.inputs[dna.step] < dt + dtcounter) { // if player input is lower than deltatime + counter then he jumps
			jump();
			dna.step++;
			dtcounter = 0;
		}
		else {
			dtcounter += dt;
		}
	}


}


//Genetic algorithm methods


/*
Crossover and mutation function
*/
void Player::mutate(DNA &parentOne, DNA &parentTwo)
{
	std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	srand(ms.count()); //random seed
	float mutationRate = 0.01;
	for (int i = 0; i < dna.getLenght(); i++) { // uniform crossover, each input is independly decided from which parent it is
		if (rand() % 2) setDNA(i, parentOne.inputs[i]);
		else setDNA(i, parentTwo.inputs[i]);

		float mutation = rand() % 10000;
		mutation = mutation / 10000.f;
		if (mutation < mutationRate || std::isinf(dna.inputs[i]) || std::isnan(dna.inputs[i])) {
			dna.randomize(i); // if mutation happens
		}
	}
}


//ABC algorithm methods

void Player::nectarPlusPlus() {
	nectarCounter++;
}



/*
GETTER and SETTER
*/
int Player::getNectarCounter()
{
	return nectarCounter;
}

void Player::setColor(Color fillColor)
{
	shape.setFillColor(fillColor);
}

Color Player::getColor()
{
	return shape.getFillColor();
}


RectangleShape Player::getShape()
{
	return shape;
}

Vector2f Player::getPosition()
{
	return position;
}

bool Player::getGravity()
{
	return up;
}

bool Player::isDead()
{
	return dead;
}

bool Player::reachedGoal()
{
	if (fitness > 2048 * 5) {
		return true;
	}
	return false;
}

float Player::getLenght()
{
	return playerLenght;
}


DNA Player::getDNA()
{
	return dna;
}

void Player::setDNA(DNA newDNA) {
	this->dna = newDNA;
}

void Player::setDNA(int i, float f)
{
	this->dna.inputs[i] = f;
}


float Player::getFitness()
{
	return fitness;
}

void Player::falls()
{
	jumping = true;
}
