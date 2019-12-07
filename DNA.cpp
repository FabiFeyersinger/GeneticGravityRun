#include "DNA.h"
#include <SFML\System\Time.hpp>



/*
Constructor
*/
DNA::DNA(int size)
{
	inputs.resize(size);
	for (int i = 0; i < inputs.size(); i++) {
		randomize(i); //randomize each input
	}
}

/*
Destructor
*/
DNA::~DNA()
{
}

//Getters
int DNA::getLenght()
{
	return inputs.size();
}


float DNA::getInput(int i)
{
	return inputs[i];
}


/*
randomizer function to make each player unique*/
void DNA::randomize(int i)
{
	std::chrono::microseconds ms = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()); 
	srand(ms.count()); //random seed
	float phi = rand() % 10000;
	phi = phi / 10000.f;
	inputs[i] = lowerBound + phi * (upperBound - lowerBound); // upper bound and lower bound decide which values are allowed
}

