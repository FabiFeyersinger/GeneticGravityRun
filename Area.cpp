#include "Area.h"
#include <iostream>




/*
classic constructor for area, sets up textures and position
*/
Area::Area()
{
	position = Vector2f(0, 0);
	this->totalLenght = 2048.f;
	if (!backgroundTexture.loadFromFile("Images\\Level\\background.png")) std::cout << "Background not loaded \n";
	background.setTexture(backgroundTexture);
	background.setPosition(position);

}

/*
normal destructor
*/
Area::~Area()
{
}

/*
Initializes Area with first platform an the position
*/
void Area::init(int size, Vector2f position)
{
	platforms.resize(size); 
	position = calculatePlatformPosition(position); // comfy method to make everything modulo 32 for comfort (will be called normalize from now on)
	for (int i = 0; i < size; i++) {
		platforms[i].init(10, Vector2f(position.x, position.y + i *64), false); // init first platform with lenght of 10
	}
	this->position = position; // set position
	background.setPosition(position.x, 0); // set position of background
	originalPosition = calculatePlatformPosition(position); // normalize Position
	originalPosition.y = 0;
	std::cout << "Area created \n";
}

/*
Each time render will call this method to receive all Platform sprites that need to be drawn
*/
std::vector<Sprite> Area::drawBlocks()
{
	std::vector<Sprite> draw; // create vector of sprites
	int k = 0;
	for(int i = alreadyPassedPlatforms; i < platforms.size(); i++)  // determine how many platforms already passed
	{
		if (platforms[i].hasPassed()) alreadyPassedPlatforms = i; // if next platform has already passed ++ the integer
		if (platforms[i].isVisible()) {
			std::vector<Sprite> platformSprites = platforms[i].drawBlocks(); // if the platform is already visible on the screen then get all sprites 32x32 and store them in the vector
			for (int j = 0; j < platformSprites.size(); j++) {
				draw.push_back(platformSprites[j]);
			}
		}
		else break;
	}
	return draw;
}

/*
easy to use methdo to add platforms at certain positions
*/
void Area::addPlatform(int size, Vector2f position)
{
	bool isCeiling = true; // later used for deciding if sprites are upside down or not
	position = calculatePlatformPosition(position); //normalizing
	position += originalPosition; // For different Areas
	std::vector<Platform> oldPlatforms = platforms; // this has to be done because otherwise the vector of platforms would fuck up the sprites of the previous stored platforms
	if (position.y >= 256) isCeiling = false; 
	int formerSize = platforms.size();
	platforms.resize(formerSize + 1);
	for (int i = 0; i < formerSize; i++) {
		platforms[i].init(oldPlatforms[i].blocks.size(), oldPlatforms[i].blocks[0].getPosition(), oldPlatforms[i].getIsCeiling()); //Initializes the old platforms once again
		if (platforms[i].getHitbox().getPosition().x <= 1024U)platforms[i].setVisible();
	}
	platforms[formerSize].init(size, position, isCeiling); // initializes the new platform
	if (platforms[formerSize].getHitbox().getPosition().x <= 1024U)platforms[formerSize].setVisible(); 
}

/*
standard update position function
*/
void Area::updatePosition(float dt)
{
	for (int i = 0; i < platforms.size(); i++) {
		if(!platforms[i].hasPassed()) platforms[i].updatePosition(dt); // updates all its platforms
	}
	this->position.x -= 2;
	background.move(-2, 0); // move background as well
}



/*
method to make everything easier for the eye
if a int is not %32-able it gets normlized (e.g: 325 --> 320)
*/
Vector2f Area::calculatePlatformPosition(Vector2f position)
{
	int moduloX = (int)position.x % 32;
	int moduloY = (int)position.y % 32;

	if (moduloX == 0 && moduloY == 0) return position;
	if (moduloX >= 16) position.x += moduloX;
	else position.x -= moduloX;
	if (moduloY >= 16) position.y += moduloY;
	else position.y -= moduloY;
	return position;
}

/*
resets area to original Position
*/
void Area::reset()
{
	background.setPosition(originalPosition);
	this->position = originalPosition;
	alreadyPassedPlatforms = 0;
	for (int i = 0; i < platforms.size(); i++) {
		platforms[i].reset();
	}

}

/*
This method is used for collision detection
*/
void Area::check4Collision(Player& player)
{
	bool platformFound = false;
	for (int i = 0; i < platforms.size(); i++) {
		if (!platforms[i].hasPassed() && platforms[i].isVisible()) {
			platformFound = platforms[i].checkCollision(player); // check each platform if it collides with a player
			if (platformFound) break;
		}
	}
	if (!platformFound) player.falls();
}


/*

GETTER and SETTER

*/

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

float Area::lenght(Vector2f vector) {

	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}


