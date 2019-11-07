#pragma once
#include <SFML/Graphics.hpp>;
#include <iostream>;
#include "Player.h";

using namespace sf;


class Platform
{
private: 
	Texture spritesheet;
	Vector2f originalPosition;
	Sprite FillSprites(int,int);
	bool isCeiling = false;
	bool visible = false;
	bool passed = false;
	float platformLenght = 0;
	RectangleShape hitbox;


public:
	Platform();
	~Platform();
	std::vector<Sprite> blocks;
	void init(int, Vector2f, bool);
	void updatePosition(float);
	bool checkCollision(Player &player);
	std::vector<Sprite> drawBlocks();
	float lenght(Vector2f);
	RectangleShape getHitbox();
	Vector2f getPosition();
	float getLenght();
	bool isVisible();
	void setVisible();
	bool hasPassed();
	bool getIsCeiling();

	void reset();

};

