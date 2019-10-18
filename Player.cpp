#include "Player.h"

Player::Player()
{
	shape = RectangleShape();
	shape.setSize(Vector2f(32.f, 32.f));
	shape.setFillColor(Color::Red);
	speed = 0.f;
	gravity = 416.f;
	position = Vector2f(10, 256);
	dna = DNA(100);
}

Player::~Player()
{
}

float Player::updatePosition(float dt)
{
	if (position.y < 0 || position.y > 512) dead = true;
	if (!dead) {
		fitness += dt;
		useDNA(dt);
	}
	position.x += speed * dt;
	updateGravity(dt);
	shape.setPosition(position);
	return 0.0f;
}

void Player::updateGravity(float dt)
{
	if (up) {
			position.y -= gravity * dt;
			jumping = true;
	}
	else {
			position.y += gravity * dt;
			jumping = true;
	}

}

void Player::setColor(Color fillColor)
{
	shape.setFillColor(fillColor);
}

Vector2f Player::getPosition()
{
	return position;
}

void Player::platformCollide(float y)
{
	this->position.y = y;
	jumping = false;
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
	if (fitness > 2048) {
		return true;
	}
	return false;
}

float Player::getLenght()
{
	return playerLenght;
}

float Player::calculateFitness()
{
	return fitness * 30000;
}

void Player::normalizeFitness(float sum)
{
	fitness = fitness / sum;
}

float Player::getFitness()
{
	return fitness;
}

void Player::jump()
{
	if (!jumping) {
		up = !up;
	}
}

void Player::falls()
{
	jumping = true;
}

void Player::useDNA(float dt)
{
	if (dna.getLenght() > dna.step) {
		if (dna.inputs[dna.step] < dt) {
			std::cout << "I jump now \n";
			jump();
			dna.step++;
		}
		else {
			dna.inputs[dna.step] -= dt;
		}
	}


}

void Player::reset()
{
	up = false;
	dead = false;
	position = Vector2f(10, 256);

}

DNA Player::getDNA()
{
	return dna;
}

void Player::mutate(DNA parentOne, DNA parentTwo)
{
	std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	srand(ms.count());
	float mutationRate = 0.01;
	for (int i = 0; i < dna.getLenght(); i++) {
		if (rand() % 2) dna.inputs[i] = parentOne.inputs[i];
		else dna.inputs[i] = parentTwo.inputs[i];

		float mutation = (float)(rand() % 10000) / 10000.f;
		if (mutation < mutationRate) {
			dna.inputs[i] = (float)(rand() % dna.getPosV()) / dna.getDivider();
		}
	}
}

RectangleShape Player::getShape()
{
	return shape;
}


