#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;
class Player
{
	RectangleShape shape;
	Vector2f position;
	float gravity, speed, playerLenght = 32;
	bool up = false, jumping = false, dead = false;


private:



public:
	float updatePosition(float);
	void updateGravity(float);
	Vector2f getPosition();
	void platformCollide(float);
	bool getGravity();
	bool isDead();
	float getLenght();
	void jump();
	void falls();
	RectangleShape getShape();
	Player();
	~Player();
};

