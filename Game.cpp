/// <summary>
/// @author Peter Lowe
/// @date May 2019
///
/// you need to change the above lines or lose marks
/// </summary>

#include "Game.h"
#include <iostream>



/// <summary>
/// default constructor
/// setup the window properties
/// load and setup the text 
/// load and setup thne image
/// </summary>
Game::Game() :
	m_window{ sf::VideoMode{ 1024U, 512U, 32U }, "SFML Game" },
	m_exitGame{ false } //when true game will exit
{
	setupFontAndText(); // load font 
	setupSprite(); // load texture

	for (int i = 0; i < 200; i++) {
		srand(time(NULL));
		Player player = Player();
		player.setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		population.players.push_back(player);
		//std::cout << "PopulationMember Nr" << i << " : " << population.players[i].getDNA().inputs[0] << std::endl;
	}

	//BEE ALGORITHM ONLY
	//population.generateFirstGeneration(population);

}

/// <summary>
/// default destructor we didn't dynamically allocate anything
/// so we don't need to free it, but mthod needs to be here
/// </summary>
Game::~Game()
{
}


/// <summary>
/// main game loop
/// update 60 times per second,
/// process update as often as possible and at least 60 times per second
/// draw as often as possible but only updates are on time
/// if updates run slow then don't render frames
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			for (int i = 0; i < updateFrequency; i++) {
				update(timePerFrame); //60 fps
			}
		}
		render(); // as many as possible
	}
}
/// <summary>
/// handle user and system events/ input
/// get key presses/ mouse moves etc. from OS
/// and user :: Don't do game update here
/// </summary>
void Game::processEvents()
{
	sf::Event newEvent;
	while (m_window.pollEvent(newEvent))
	{
		if (sf::Event::Closed == newEvent.type) // window message
		{
			m_exitGame = true;
		}
		if (sf::Event::KeyPressed == newEvent.type) //user pressed a key
		{
			processKeys(newEvent);
		}
	}
}


/// <summary>
/// deal with key presses from the user
/// </summary>
/// <param name="t_event">key press event</param>
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code)
	{
		m_exitGame = true;
	}

	if (sf::Keyboard::Up == t_event.key.code) {
		updateFrequency++;
	}
	if (sf::Keyboard::Down == t_event.key.code) {
		updateFrequency--;
	}
}

/// <summary>
/// Update the game world
/// </summary>
/// <param name="t_deltaTime">time interval per frame</param>
void Game::update(sf::Time dt)
{
	if (m_exitGame)
	{
		m_window.close();
	}
	Area currentArea = area1;
	if (area1.getPosition().x <= -2048) {
		currentArea = area2;
	}
	else {
		currentArea = area1;
	}

	population.update(dt.asSeconds() / updateFrequency, currentArea);
	area1.updatePosition(dt.asSeconds() / updateFrequency);
	area2.updatePosition(dt.asSeconds() / updateFrequency);
}

/// <summary>
/// draw the frame and then switch buffers
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color::Black);
	m_window.draw(area1.getBackground());
	m_window.draw(area2.getBackground());
	if (population.allDotsDead()) {
		createNewPopulation(); //Genetic Algorithm
		//createNextSwarm(); // ABC Algorithm
		population.reset();
		area1.reset();
		area2.reset();
	}
	std::vector<Sprite> areaSprites = area1.drawBlocks();
	for (int i = 0; i < areaSprites.size(); i++) {
		m_window.draw(areaSprites[i]);
	}
	areaSprites = area2.drawBlocks();
	for (int i = 0; i < areaSprites.size(); i++) {
		m_window.draw(areaSprites[i]);
	}
	for (int i = 0; i < population.players.size(); i++) {
		m_window.draw(population.players[i].getShape());
	}
	m_window.display();
}



/// <summary>
/// load the font and setup the text message for screen
/// </summary>
void Game::setupFontAndText()
{
}

/// <summary>
/// load the texture and setup the sprite for the logo
/// </summary>
void Game::setupSprite()
{
	area1.init(1, Vector2f(10, 480));
	area1.addPlatform(7, Vector2f(320, 0));
	area1.addPlatform(6, Vector2f(544, 480));
	area1.addPlatform(6, Vector2f(600, 140));
	area1.addPlatform(9, Vector2f(736, 0));
	area1.addPlatform(5, Vector2f(1024, 480));
	area1.addPlatform(5, Vector2f(1200, 0));
	area1.addPlatform(5, Vector2f(1400, 300));
	area1.addPlatform(5, Vector2f(1600, 480));
	area1.addPlatform(9, Vector2f(1700, 0));
	area1.addPlatform(4, Vector2f(1900, 400));


	area2.init(1, Vector2f(2048, 480));
	area2.addPlatform(10, Vector2f(200, 0));
	area2.addPlatform(6, Vector2f(544, 480));
	area2.addPlatform(6, Vector2f(600, 140));
	area2.addPlatform(9, Vector2f(736, 0));
	area2.addPlatform(5, Vector2f(1024, 480));
	area2.addPlatform(5, Vector2f(1200, 0));
	area2.addPlatform(5, Vector2f(1400, 300));
	area2.addPlatform(5, Vector2f(1600, 480));
	area2.addPlatform(9, Vector2f(1800, 0));
}

void Game::createFirstSwarm()
{
	for (int i = 0; i < 200; i++) {
		srand(time(NULL));
		Player player = Player();
		player.setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		population.players.push_back(player);
		population.generateFirstGeneration(population);
		//std::cout << "PopulationMember Nr" << i << " : " << population.players[i].getDNA().inputs[0] << std::endl;
	}
}

void Game::createNextSwarm()
{
	population.calculateFitness();
	int swarmSize = 200;
	Population nextSwarm = Population();
	Population employed = Population();
	for (int i = 0; i < swarmSize; i++) {
		Player employedBee = population.compareFitness(population.players[i], population.players[++i]);
		employed.players.push_back(employedBee);
	}
	for (int i = 0; i < employed.players.size(); i++) {
		Player onlooker = population.waggleDance(employed);
		nextSwarm.players.push_back(onlooker);
		Player candidate = employed.generateCanditate(employed, onlooker);
		nextSwarm.players.push_back(candidate);
	}
	population.replace(nextSwarm);
	genCount++;
	std::cout << "New Swarm: " << genCount << std::endl;
}

void Game::createNewPopulation()
{
	population.calculateFitness();
	Population newGen = Population();
	for (int i = 0; i < population.players.size(); i++) {
		//std::cout << "PopulationMember Nr" << i << " : " << population.players[i].getDNA().inputs[0] << std::endl;
		Player parentOne = population.naturalSelection(population);
		Player parentTwo = population.naturalSelection(population);
		Player child = population.mutate(parentOne, parentTwo);
		//Player child = population.mutate(parentOne);
		newGen.players.push_back(child);
		//std::cout << i << " Child in Game.cpp jump: " << newGen.players[i].getDNA().inputs[0] << std::endl;
	}
	population.replace(newGen);
	genCount++;
	std::cout << "New Population: " << genCount << std::endl;
}

