#include "Platform.h"
#include <iostream>;


/*
Constructor that loads texture
*/
Platform::Platform()
{
	if (!spritesheet.loadFromFile("Images\\Level\\Blocks.png")) std::cout << "spritesheet not loaded \n";
}

/*
Destructor
*/
Platform::~Platform()
{
}

/*
Init method to set all positions and set the right sprites
*/
void Platform::init(int size, Vector2f position, bool isCeiling)
{
	this->isCeiling = isCeiling;
	platformLenght = size * 32;
	blocks.resize(size); // resize to match size
	for (int i = 0; i < size; i++) {
		blocks[i] = FillSprites(i, size-1); // Fill each block with a sprite
		blocks[i].setPosition(position.x + 32 * i, position.y); // set position of each block
	}
	hitbox.setSize(Vector2f(size * 32, 32));
	hitbox.setPosition(position);
	originalPosition = position;
	visible = false;
	passed = false;
}

/*
classic update Position method
*/
void Platform::updatePosition(float)
{
	hitbox.move(-2, 0); // Position is handled with the Rectangle shape Hitbox
	for (int i = 0; i < blocks.size(); i++) 
	{
		blocks[i].move(-2, 0);
	}
	if (hitbox.getPosition().x <= 1024U) this->visible = true; // Bools to reduce loops in area methods
	if (hitbox.getPosition().x + hitbox.getSize().x <= 0) this->passed = true;
}


/*
checks the collison with the player
if there is a collision the player is called to adjust his position
*/
bool Platform::checkCollision(Player &player)
{
	//Checks if it collides with ceiling
	if (hitbox.getPosition().x <= player.getPosition().x + player.getLenght() && player.getPosition().x <= hitbox.getPosition().x + hitbox.getSize().x && player.getGravity() == isCeiling) {
		if (player.getGravity()) {
			if (hitbox.getPosition().y + hitbox.getSize().y > player.getPosition().y && hitbox.getPosition().y + hitbox.getSize().y / 2 < player.getPosition().y) {
				player.platformCollide(hitbox.getPosition().y + hitbox.getSize().y);
				return true;
			}
		}
		//checks if it collides with bottom
		else {
			if (hitbox.getPosition().y < player.getPosition().y + player.getLenght() && hitbox.getPosition().y + hitbox.getSize().y / 2 > player.getPosition().y + player.getLenght()) {
				player.platformCollide(hitbox.getPosition().y - player.getLenght());
				return true;
			}
		}

	}
	return false;
}


/*
Method is called by render function each loop to return sprites
*/
std::vector<Sprite> Platform::drawBlocks()
{
	std::vector<Sprite> visibleBlocks;

	for (int i = 0; i < blocks.size(); i++) {
		if (blocks[i].getPosition().x <= 1024U) visibleBlocks.push_back(blocks[i]);
	}


	return visibleBlocks;
}

/*
resets platforms
*/
void Platform::reset()
{
	init(blocks.size(), originalPosition, isCeiling);
}

/*
Decides depending on where the block is which sprite it uses
*/
Sprite Platform::FillSprites(int i, int size)
{
	Sprite sprite = Sprite();
	sprite.setTexture(spritesheet);
	int a;
	int b;
	if (isCeiling) {  // if it is the ceiling, the lower row of sprites is used, else the upper row
		b = 35;
	}
	else {
		b = 2;
	}

	if (size == 1) {
		sprite.setTextureRect(IntRect(36, b, 32, 32));
		return sprite;
	}

	if (i == 0) { // if it is the first block use sprite for beginning of a Platform
		a = 2;
	}
	else if (i == size) { // if last block use sprite for end of a Platform
		a = 104;
	}
	else if (i % 2 == 0) { // if anything in between alternate between to sprites
		a = 36;
	}
	else {
		a = 70;
	}

	sprite.setTextureRect(IntRect(a, b, 32, 32)); // set Sprite
	return sprite;
}


/*
GETTER and SETTER
*/

bool Platform::getIsCeiling()
{
	return isCeiling;
}

RectangleShape Platform::getHitbox()
{
	return hitbox;
}

Vector2f Platform::getPosition()
{
	return blocks[0].getPosition();
}

float Platform::getLenght()
{
	return platformLenght;
}

bool Platform::isVisible()
{
	return visible;
}

void Platform::setVisible()
{
	this->visible = true;
}

bool Platform::hasPassed()
{
	return passed;
}

float Platform::lenght(Vector2f vector) {

	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}