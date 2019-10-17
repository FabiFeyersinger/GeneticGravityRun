#include "Platform.h"
#include <iostream>;


Platform::Platform()
{
	if (!spritesheet.loadFromFile("Images\\Level\\Blocks.png")) std::cout << "spritesheet not loaded \n";
	platformLenght = 0;

}

Platform::~Platform()
{
}

void Platform::init(int size, Vector2f position, bool isCeiling)
{
	this->isCeiling = isCeiling;
	platformLenght = size * 32;
	std::cout << blocks.size() << std::endl;
	blocks.resize(size);
	std::cout << blocks.size() << std::endl;
	for (int i = 0; i < size; i++) {
		blocks[i] = FillSprites(i, size-1);
		blocks[i].setPosition(position.x + 32 * i, position.y);
	}
	hitbox.setSize(Vector2f(size * 32, 32));
	hitbox.setPosition(position);
	std::cout << "Platform created \n";
}

void Platform::updatePosition(float)
{
	hitbox.move(-2, 0);
	for (int i = 0; i < blocks.size(); i++) 
	{
		blocks[i].move(-2, 0);
	}
}

bool Platform::checkCollision(Player &player)
{
	if (hitbox.getPosition().x <= player.getPosition().x + player.getLenght() && player.getPosition().x <= hitbox.getPosition().x + hitbox.getSize().x && player.getGravity() == isCeiling) {
		if (player.getGravity()) {
			if (hitbox.getPosition().y + hitbox.getSize().y > player.getPosition().y && hitbox.getPosition().y + hitbox.getSize().y / 2 < player.getPosition().y) {
				player.platformCollide(hitbox.getPosition().y + hitbox.getSize().y);
				// std::cout << "collide ceiling \n";
				return true;
			}
		}
		else {
			if (hitbox.getPosition().y < player.getPosition().y + player.getLenght() && hitbox.getPosition().y + hitbox.getSize().y / 2 > player.getPosition().y + player.getLenght()) {
				player.platformCollide(hitbox.getPosition().y - player.getLenght());
				// std::cout << "collide bottom \n";
				return true;
			}
		}

	}
	return false;
}

std::vector<Sprite> Platform::drawBlocks()
{
	return blocks;
}

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

Sprite Platform::FillSprites(int i, int size)
{
	Sprite sprite = Sprite();
	sprite.setTexture(spritesheet);
	int a;
	int b;
	if (isCeiling) {
		b = 35;
	}
	else {
		b = 2;
	}

	if (size == 1) {
		sprite.setTextureRect(IntRect(36, b, 32, 32));
		return sprite;
	}

	if (i == 0) {
		a = 2;
	}
	else if (i == size) {
		a = 104;
	}
	else if (i % 2 == 0) {
		a = 36;
	}
	else {
		a = 70;
	}

	sprite.setTextureRect(IntRect(a, b, 32, 32));
	return sprite;
}

float Platform::lenght(Vector2f vector) {

	return std::sqrt(vector.x * vector.x + vector.y * vector.y);
}