#pragma once
#include <SFML/Graphics.hpp>;
#include <iostream>;
#include "Player.h";

using namespace sf;


class Platform
{
private: 
	Texture spritesheet;
	Sprite FillSprites(int,int);
	bool isCeiling = false;
	float platformLenght;
	RectangleShape hitbox;


public:
	Platform();
	~Platform();
	std::vector<Sprite> blocks;
	void init(int, Vector2f, bool);
	void updatePosition(float);
	bool checkCollision(Player &player);
	std::vector<Sprite> drawBlocks();
	bool getIsCeiling();
	float lenght(Vector2f);
	RectangleShape getHitbox();
	Vector2f getPosition();
	float getLenght();
};

