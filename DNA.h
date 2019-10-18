#pragma once
#include <vector>;
#include <chrono>;
#include <iostream>;

class DNA
{

public:
	std::vector<float> inputs;
	int step = 0;
	int possibleValues = 500;
	float divider = 100;
	DNA() {};
	DNA(int);
	~DNA();

	int getLenght();
	float getInput(int);
	void randomize();
	float getDivider();
	int getPosV();
};

