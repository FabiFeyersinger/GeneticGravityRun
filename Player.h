#pragma once

#include <SFML/Graphics.hpp>
#include "DNA.h";

using namespace sf;
class Player
{
	DNA dna;
	RectangleShape shape;
	Vector2f position;
	float gravity, speed, playerLenght = 32, fitness = 0, dtcounter = 0;
	bool up = false, jumping = false, dead = false;
	int nectarCounter = 0;

public:
	//common methods
	float updatePosition(float);
	void updateGravity(float);
	void setColor(Color);
	Color getColor();
	Vector2f getPosition();
	void platformCollide(float);
	bool getGravity();
	bool isDead();
	bool reachedGoal();
	float getLenght();
	void jump();
	void falls();
	void reset();
	RectangleShape getShape();
	Player();
	~Player();

	//algorithm methods used in both algorithms
	float calculateFitness();
	void normalizeFitness(float);
	float getFitness();
	void useDNA(float);
	DNA getDNA();
	void setDNA(DNA);

	//Genetic algorithm methods

	void mutate(DNA &parentOne, DNA &parentTwo);
	void mutate(DNA parent);

	// ABC algorithm methods
	int getNectarCounter();
	void nectarPlusPlus();
};

