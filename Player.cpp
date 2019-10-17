#include "Player.h"

Player::Player()
{
	shape = RectangleShape();
	shape.setSize(Vector2f(32.f, 32.f));
	shape.setFillColor(Color::Red);
	speed = 0.f;
	gravity = 416.f;
	position = Vector2f(10, 256);
}

Player::~Player()
{
}

float Player::updatePosition(float dt)
{
	if (position.y < 0 || position.y > 512) dead = true;
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

float Player::getLenght()
{
	return playerLenght;
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

RectangleShape Player::getShape()
{
	return shape;
}


