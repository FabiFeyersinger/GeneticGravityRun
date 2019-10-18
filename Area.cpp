#include "Area.h"
#include <iostream>





Area::Area()
{
	position = Vector2f(0, 0);
	this->totalLenght = 2048.f;
	if (!backgroundTexture.loadFromFile("Images\\Level\\background.png")) std::cout << "Background not loaded \n";
	background.setTexture(backgroundTexture);
	background.setPosition(position);

}


Area::~Area()
{
}

void Area::init(int size, Vector2f position)
{
	platforms.resize(size);
	position = calculatePlatformPosition(position);
	for (int i = 0; i < size; i++) {
		platforms[i].init(10, Vector2f(position.x, position.y + i *64), false);
	}
	this->position = position;
	originalPosition = calculatePlatformPosition(position);
	originalPosition.y = 0;
	std::cout << "Area created \n";
}

std::vector<Sprite> Area::drawBlocks()
{
	std::vector<Sprite> draw(50);
	int k = 0;
	for(int i = 0; i < platforms.size(); i++)
	{
		std::vector<Sprite> platformSprites = platforms[i].drawBlocks();
		for (int j = 0; j < platformSprites.size(); j++) {
			draw[k] = platformSprites[j];
			k++;
		}
	}
	return draw;
}

void Area::addPlatform(int size, Vector2f position)
{
	bool isCeiling = true;
	position = calculatePlatformPosition(position);
	std::vector<Platform> oldPlatforms = platforms;
	if (position.y >= 256) isCeiling = false;
	int formerSize = platforms.size();
	platforms.resize(formerSize + 1);
	for (int i = 0; i < formerSize; i++) {
		platforms[i].init(oldPlatforms[i].blocks.size(), oldPlatforms[i].blocks[0].getPosition(), oldPlatforms[i].getIsCeiling());
	}
	platforms[formerSize].init(size, position, isCeiling);
}

void Area::updatePosition(float dt)
{
	for (int i = 0; i < platforms.size(); i++) {
		platforms[i].updatePosition(dt);
	}
	this->position.x -= 2;
	background.move(-2, 0);
}







Vector2f Area::calculatePlatformPosition(Vector2f position)
{
	int moduloX = (int)position.x % 32;
	int moduloY = (int)position.y % 32;

	if (moduloX == 0 && moduloY == 0) return position;
	if (moduloX >= 16) position.x += moduloX;
	else position.x -= moduloX;
	if (moduloY >= 16) position.y += moduloY;
	else position.y -= moduloY;
	std::cout << "corrected Platform input\n";
	return position;
}

RectangleShape Area::getHitbox()
{
	return platforms[0].getHitbox();
}

float Area::getLenght()
{
	return this->totalLenght;
}

Vector2f Area::getPosition()
{
	return this->position;
}

Sprite Area::getBackground()
{
	return background;
}

void Area::check4Collision(Player &player)
{
	bool platformFound = false;
	for (int i = 0; i < platforms.size(); i++) {
			platformFound = platforms[i].checkCollision(player);
			if (platformFound) break;
	}
	if (!platformFound) player.falls();
}


float Area::lenght(Vector2f vector) {

	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}

void Area::reset()
{
	background.setPosition(originalPosition);
	for (int i = 0; i < platforms.size(); i++) {
		platforms[i].reset();
	}

}
