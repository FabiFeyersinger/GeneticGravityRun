#pragma once
#include <SFML/Graphics.hpp>
#include "Platform.h";
using namespace sf;

class Area
{
private:
	std::vector<Platform> platforms;
	Vector2f position;
	float totalLenght;
	Texture backgroundTexture;
	Sprite background;
	Vector2f calculatePlatformPosition(Vector2f);

public:
	Area();
	~Area();
	void init(int, Vector2f);
	std::vector<Sprite> drawBlocks();
	void addPlatform(int, Vector2f);
	void updatePosition(float);
	RectangleShape getHitbox();
	float getLenght();
	Vector2f getPosition();
	Sprite getBackground();
	void check4Collision(Player &player);
	float lenght(Vector2f);

};

