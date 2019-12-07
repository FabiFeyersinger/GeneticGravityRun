#pragma once
#include <vector>;
#include <chrono>;
#include <iostream>;

class DNA
{

public:
	std::vector<float> inputs;
	int step = 0;
	int upperBound = 10;
	int lowerBound = 0.5;
	DNA() {};
	DNA(int);
	~DNA();

	int getLenght();
	float getInput(int);
	std::vector<float> getInputs() {return inputs;};
	void randomize(int);
	int getStep() { return step; };
	void setStep(int i) {step = i;};

	int getLowerBound() { return lowerBound; };
	int getUpperBound() { return upperBound; };

};

