
#ifndef GAME_HPP
#define GAME_HPP
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>
#include "Area.h";
#include "Population.h";

using json = nlohmann::json;

class Game
{
public:
	Game();
	~Game();
	void run();

private:

	void processEvents();
	void processKeys(sf::Event t_event);
	void update(sf::Time t_deltaTime);
	void updatePopulation(sf::Time t_deltaTime);
	void render();
	void createNewPopulation();
	void setupSprite();
	void createNextSwarm();
	void showImprovement();
	void renderJSON();
	void resetAreas();
	void updateJSON(sf::Time);
	void prepareForWatchmode();
	void showInstructions();

	json averageScores = { {"start", "today" } };
	sf::RenderWindow m_window;
	sf::Font m_ArialBlackfont;
	sf::Text m_welcomeMessage;
	sf::Texture playerTexture; 
	sf::VertexArray graphArray;
	sf::Image graph;

	Area area1, area2, area3, area4, area5;
	std::vector<Area> areas;

	Population population, newGen;
	bool m_exitGame;
	bool visualize;
	bool gameIsFinished = false;
	bool watchmode = false;
	int genWithMaxFitness = 0;
	int genCount = 1;
	int gameDuration = 0;
	int updateFrequency = 15;
	float maxGenAverage = 1.f;
	int limit = 1;
	int progress = 0;

	bool abc = true;
	bool elite = false;
	bool instructionsshown = false;
	std::string algorithmName;
	std::string jsonFileName = "nothing";
	std::ifstream ifs;
};

#endif

