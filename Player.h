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

public:
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
	float calculateFitness();
	void normalizeFitness(float);
	float getFitness();
	void jump();
	void falls();
	void useDNA(float);
	void reset();
	DNA getDNA();
	void mutate(DNA &parentOne, DNA &parentTwo);
	void mutate(DNA parent);
	RectangleShape getShape();
	Player();
	~Player();
};

