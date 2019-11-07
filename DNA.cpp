#include "DNA.h"
#include <SFML\System\Time.hpp>

DNA::DNA(int size)
{
	inputs.resize(size);
	randomize();

}

DNA::~DNA()
{
}

int DNA::getLenght()
{
	return inputs.size();
}


float DNA::getInput(int i)
{
	return inputs[i];
}

void DNA::randomize()
{
	std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
	srand(ms.count());
	for (int i = 0; i < inputs.size(); i++) {
		float ran = rand() % possibleValues;
		ran = ran / divider;
		if (ran < 0.5) ran += 0.5;
		if (ran > 5) ran = 4.9;
		inputs[i] = ran;
	}

	//std::cout << "First jump should be :" << inputs[0] << std::endl;
}

float DNA::getDivider()
{
	return 0.0f;
}

int DNA::getPosV()
{
	return possibleValues;
}
