#define _CRT_SECURE_NO_WARNINGS

#include "Game.h"
#include <iostream>


/*
Default constructor for the game
Initializes Window, Population and Sprites
*/
Game::Game() :
	m_window{ sf::VideoMode{ 1024U, 512U, 32U }, "SFML Game" },
	m_exitGame{ false } //when true game will exit
{
	setupSprite(); // load texture

	for (int i = 0; i < 200; i++) { // each loop one player gets added to the first population
		srand(time(NULL));
		Player player = Player();
		player.setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
		population.players.push_back(player);
	}
	newGen = Population(200); // initializes newGen for later use
}

/* 
default destructor
*/
Game::~Game()
{
}


/*
Main game loop
60 updates per second -> standard
can be increased by pressing Up
60 fps
*/


void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const float fps{ 60.0f };
	visualize = false;
	graphArray.setPrimitiveType(sf::PrimitiveType::LineStrip);

	sf::Time timePerFrame = sf::seconds(1.0f / fps); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		if (!watchmode) {	// switch between observing the whole population training and observing one particular champs replay
			while (timeSinceLastUpdate > timePerFrame)
			{
				timeSinceLastUpdate -= timePerFrame;
				processEvents(); // at least 60 fps
				for (int i = 0; i < updateFrequency; i++) {  //updates more often if needed
					update(timePerFrame); //60 fps
				}
				if (updateFrequency == 0 && !instructionsshown) showInstructions();
			}
			render(); // as many as possible
		}
		else {
			while (timeSinceLastUpdate > timePerFrame)
			{
				timeSinceLastUpdate -= timePerFrame;
				processEvents(); // at least 60 fps
				for (int i = 0; i < updateFrequency; i++) {
					updateJSON(timePerFrame); //60 fps
				}
			}
			renderJSON(); // as many as possible
		}

	}
}

/*
standard handle user/system events
*/

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

/*
deal with certain pressed keys
*/
void Game::processKeys(sf::Event t_event)
{
	if (sf::Keyboard::Escape == t_event.key.code) // Exit game
	{
		m_exitGame = true;
	}

	if (sf::Keyboard::Up == t_event.key.code) {  // Increase update frequency (+60), standard = 60/s, if higher than 240/s then game will not render anymore
		updateFrequency++;
	}
	if (sf::Keyboard::Down == t_event.key.code) { // Decrease update frequency (-60), if 0/s the game pauses
		updateFrequency--;
	}

	if (sf::Keyboard::L == t_event.key.code) {  // You can choose to load in an json file from one of the previous runs. If so you can observe how it beat the level
		if (watchmode) {
			watchmode = false;
			updateFrequency = 0;
		}
		else {
			watchmode = true;
			prepareForWatchmode();

		}
	}

	if (sf::Keyboard::V == t_event.key.code) {  // Small visualization of the improvement, does not work really well because there is no scale
		if (visualize) {
			visualize = false;
		}
		else {
			visualize = true;
		}
	}

	if (sf::Keyboard::E == t_event.key.code) { // Activate/Deactivate Elitism during a run
		if (elite) {
			elite = false;
			std::cout << "Elitism disabled";
		}
		else {
			elite = true;
			std::cout << "Elitism enabled";
		}
	}

	if (sf::Keyboard::G == t_event.key.code) {  // Select Genetic Algorithm as the algorithm to train the population
		abc = false;
		updateFrequency = 1;
	}

	if (sf::Keyboard::A == t_event.key.code) { // Select ABC Algorithm as the algorithm to train the population
		abc = true;
		updateFrequency = 1;
	}
}


/*
checks if the game has finished, otherwise calls update population
*/
void Game::update(sf::Time dt)
{
	if (m_exitGame)  // If the game has finished, by one player reaching the goal
	{
		std::time_t end_time = std::time(0);
		tm* dt = localtime(&end_time);
		std::string month = std::to_string(dt->tm_mon + 1);
		std::string day = std::to_string(dt->tm_mday);
		std::string year = std::to_string(dt->tm_year + 1900);
		std::string hours = std::to_string(dt->tm_hour + 1);
		std::string minutes = std::to_string(dt->tm_min + 1);
		std::string seconds = std::to_string(dt->tm_sec);
		std::string dateTime = day + "." + month + "." + year + "_" + hours + "." + minutes + "." + seconds; //Time is captured to make a unique JSON file
		std::string fileName = "Runs\\" + algorithmName + dateTime + "_Fitness.json"; // Save in folder Runs
		std::ofstream j(fileName);
		averageScores["ChampInputs"] = population.getChamp().getDNA().getInputs();	// Add Champs input to the JSON so it can be observed later on
		averageScores["ChampFitness"] = population.getChamp().getFitness();		    // Add final fitness value of the champ to JSON
		if (gameIsFinished) {
			averageScores["Generations needed"] = genCount;	// Add counter to JSON to see how many Generations were needed
		}

		j << std::setw(4) << averageScores << std::endl; // JSON-Object --> JSON file

		m_window.close();
	}

	updatePopulation(dt);

}

/*
small method to handle all updates related to the game
*/
void Game::updatePopulation(sf::Time dt)
{
	if (areas[progress].getPosition().x <= -2000 && progress +1 < areas.size()) {  //if the current area is getting to an end tell the game we made progress
		progress++;
	}
	population.update(dt.asSeconds(), areas[progress]); // updates the population, uses the current area to check if collisions happen
	gameDuration += 2;	// counter for gameduration

	for (int i = 0; i < areas.size(); i++) {
		areas[i].updatePosition(dt.asSeconds());  // updates all areas
	}

}

/*
draw frames if frequency is not too high and determine when to start a new population
*/
void Game::render()
{
	m_window.clear(sf::Color::Black);  // clear window

	if (population.allDotsDead()) { // depending of the algorithm that is used
		if (abc) {
			createNextSwarm(); // ABC Algorithm
		}
		else {
			createNewPopulation(); //Genetic Algorithm
		}
		progress = 0;
		population.reset(); // reset everything
		resetAreas();
	}

	if (updateFrequency <= 4) {   // if updateFrequency is too high rendering does not feel smooth anymore, therefore it gets disabled
		m_window.draw(areas[progress].getBackground());  // Background
		
		std::vector<Sprite> areaSprites = areas[progress].drawBlocks();  // Platforms
		for (int i = 0; i < areaSprites.size(); i++) {
			m_window.draw(areaSprites[i]);
		}
		if (progress + 1 < areas.size()) {	// if we are not at the end of the level
			m_window.draw(areas[progress + 1].getBackground()); // draw background of next part
			areaSprites = areas[progress + 1].drawBlocks();		
			for (int i = 0; i < areaSprites.size(); i++) {
				m_window.draw(areaSprites[i]);		// draw Platforms of next part
			}
		}

		for (int i = 0; i < population.players.size(); i++) {
			m_window.draw(population.players[199 -i].getShape());  //draw whole population, starting with last one in vector so we can observe the champ if necessary
		}

	}

	if (visualize) {  // if needed visualizes the improvement of this particular training session
		showImprovement();
	}

	m_window.display();
}


void Game::showImprovement()  // small method to draw a line
{
	float width = 512;
	graphArray.resize(genCount);
	graphArray.append(Vertex(Vector2f(0, 512)));
	for (int i = 1; i < genCount - 1; i++) {
		int genAverage = averageScores.at("Population " + std::to_string(i));
		//graph.setPixel(i + 10, genAverage / maxGenAverage * 190, sf::Color::Black);
		graphArray.append(Vertex(Vector2f(i + 10, 512 - (genAverage / maxGenAverage * 190))));
	}
	m_window.draw(graphArray);
}


//resets all areas if one population is finished
void Game::resetAreas()
{
	for (int i = 0; i < areas.size(); i++) {
		areas[i].reset();
	}
}



/* 
load the texture and setup the sprite for the game
*/
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

	area3.init(1, Vector2f(2048 * 2, 480));
	area3.addPlatform(10, Vector2f(200, 0));
	area3.addPlatform(6, Vector2f(544, 480));
	area3.addPlatform(6, Vector2f(600, 140));
	area3.addPlatform(9, Vector2f(736, 0));
	area3.addPlatform(5, Vector2f(1024, 480));
	area3.addPlatform(5, Vector2f(1200, 0));
	area3.addPlatform(5, Vector2f(1400, 300));
	area3.addPlatform(5, Vector2f(1600, 480));
	area3.addPlatform(9, Vector2f(1800, 0));

	area4.init(1, Vector2f(2048 * 3, 480));
	area4.addPlatform(10, Vector2f(200, 0));
	area4.addPlatform(6, Vector2f(544, 480));
	area4.addPlatform(6, Vector2f(600, 140));
	area4.addPlatform(9, Vector2f(736, 0));
	area4.addPlatform(5, Vector2f(1024, 480));
	area4.addPlatform(5, Vector2f(1200, 0));
	area4.addPlatform(5, Vector2f(1400, 300));
	area4.addPlatform(5, Vector2f(1600, 480));
	area4.addPlatform(9, Vector2f(1800, 0));

	area5.init(1, Vector2f(2048 * 4, 480));
	area5.addPlatform(10, Vector2f(200, 0));
	area5.addPlatform(6, Vector2f(544, 480));
	area5.addPlatform(6, Vector2f(600, 140));
	area5.addPlatform(9, Vector2f(736, 0));
	area5.addPlatform(5, Vector2f(1024, 480));
	area5.addPlatform(5, Vector2f(1200, 0));
	area5.addPlatform(5, Vector2f(1400, 300));
	area5.addPlatform(5, Vector2f(1600, 480));
	area5.addPlatform(9, Vector2f(1800, 0));

	areas.push_back(area1);
	areas.push_back(area2);
	areas.push_back(area3);
	areas.push_back(area4);
	areas.push_back(area5);
}


/*

ABC Methods

*/

/*
If new Population is needed
*/
void Game::createNextSwarm()
{
	population.calculateFitness();  // calculates and normalizes fitness
	int swarmSize = 200;
	Population employed = Population();
	for (int i = 0; i < swarmSize; i++) {
		Player employedBee = population.compareFitness(population.players[i], population.players[++i]);  // checks if the candidate or the employed bee has a better fitness value
		employed.players.push_back(employedBee);	// better bee gets to do waggle dance
	}
	int start = 0;
	int k = 0;
	Player champ = population.getChamp();
	if (elite) {   // Elitism
		champ.crown(sf::Color::Black);
		newGen.players[0] = champ;
		newGen.players[1] = employed.generateCanditate(employed, champ);
		start = 1;
		int k = 2;
	}

	for (int i = start; i < employed.players.size(); i++) {  
		Player onlooker = population.waggleDance(employed);  // each winnerbee is allowed to perform waggle dance, onlookers decide which path they follow
		onlooker.crown(sf::Color::Red);		// onlookers are now employed and are colored red
		newGen.players[k++] = onlooker;
		Player candidate = employed.generateCanditate(employed, onlooker);  // employed bees create candidates which they race against 
		candidate.crown(sf::Color::Blue);  // candidates are colored blue
		newGen.players[k++] = candidate;
	}

	population.replace(newGen);  // the new Population now gets to replace the old one
	float averageScore = population.getFitnessAverage();
	averageScores["Population " + std::to_string(genCount)] = averageScore;  // add Average score of this population to JSON object
	averageScores["Champ at " + std::to_string(genCount)] = population.getChamp().getFitness();  // Add score of current champ to JSON object (current champ can be not from current population)
	genCount++;
	std::cout << "New Swarm: " << genCount << std::endl;

	if (averageScore > maxGenAverage + 0.01) { maxGenAverage = averageScore; genWithMaxFitness = genCount; }  // if the score of the current population is higher than the best one it replaces the best one
	if (genCount > genWithMaxFitness + limit) {  // if there is a local maxima, this function deals with it
		population.abandonFoodResource(population, champ);  // last input is forgotten and every bee needs to find a different input
		std::cout << "Food Source abandond \n";
	}


	if (gameDuration >= 2048 * areas.size()) {  // if game is finished
		m_exitGame = true;
		algorithmName = "ABC_";  // Name jsonfile to make it easier for later use
		if (elite) {
			algorithmName += "E_";
		}
		gameIsFinished = true;
		genCount--;
	}
	else {
		gameDuration = 0;
	}
}


/*

Genetic Algorithm methods

*/

/*
If new Population is needed
*/
void Game::createNewPopulation()
{
	population.calculateFitness();  // fitness calculation and normalization
	int start = 0;
	Player champ = population.getChamp();
	if (elite) {
		newGen.players[0] = population.getChamp(); // if elitism is activated champ gets to new gen
		start = 1;
	}
	for (int i = start; i < population.players.size(); i++) {
		Player parentOne = population.naturalSelection(population); // Select Parent 1
		Player parentTwo = population.naturalSelection(population); // Select Parent 2
		Player child = population.mutate(parentOne, parentTwo);  // combine DNA of Parent 1 & 2 and then mutate dem babies
		newGen.players[i] = child;
	}
	population.replace(newGen);  // replace old population with new one
	float averageScore = population.getFitnessAverage();  // from now on out same as ABC
	averageScores["Population " + std::to_string(genCount)] = averageScore;
	averageScores["Champ at " + std::to_string(genCount)] = population.getChamp().getFitness();
	genCount++;
	std::cout << "New Population: " << genCount << std::endl;

	if (averageScore > maxGenAverage + 0.01) { maxGenAverage = averageScore; genWithMaxFitness = genCount; }  // if the score of the current population is higher than the best one it replaces the best one

	if (gameDuration >= 2048 * areas.size()) {
		m_exitGame = true;
		algorithmName = "GA_";
		if (elite) {
			algorithmName += "E_";
		}
		gameIsFinished = true;
	}
	else {
		gameDuration = 0;
	}
}




/*

The following methods are used for inspectation of the JSON Files that are created when a Run is finished

*/



void Game::updateJSON(sf::Time dt) // standard update
{
	updatePopulation(dt);
}


/*
This method is here to wait for an filename of a JSON file
*/
void Game::prepareForWatchmode() 
{

	std::cout << "Please choose a JSON file within the Runs directory and paste the only name in here: ";
	std::cin >> jsonFileName;
	std::string fileName = jsonFileName.c_str();
	ifs.open("Runs\\" + fileName);
	ifs >> averageScores;  // turns JSON file into JSON object
	updateFrequency = 0; 

	population.players.resize(1); // only champ is needed
	DNA champDNA = DNA(100);
	for (int i = 0; i < champDNA.inputs.size(); i++) {
		champDNA.inputs[i] = averageScores["ChampInputs"][i]; // get inputs from JSON object and inject it to DNA
	}
	Player champ = Player();
	champ.setDNA(champDNA); // give Player the dna of the champ
	population.players[0] = champ;
	population.reset();
	resetAreas();

	graphArray.clear();
	graphArray.resize(averageScores["Generations needed"]);
	graphArray.append(Vertex(Vector2f(0, 512)));
	float highestScore = averageScores["ChampFitness"];

	for (int i = 1; i < averageScores["Generations needed"]; i++) {
		float generationAverage = averageScores["Population " + std::to_string(i)];
		graphArray.append(Vertex(Vector2f(i, generationAverage / highestScore * 512)));
	}
}


/*
Method to give the user instructions in the console
*/
void Game::showInstructions()
{
	instructionsshown = true;
	std::cout << "Press G to choose Genetic Algorithm or presss A to choose ABC Algorithm \n";
	std::cout << "If you want to enable/disable Elitism for any of the algorithms press E \n";
	std::cout << "Press Up or Down to speed up / slow down the speed of the game\n";
	std::cout << "If you want to load a previous Run press L \n";

}


/*
render method for loaded json champ, similiar to normal render method, but loops instead of creating a new Population
*/
void Game::renderJSON()
{
	m_window.clear(sf::Color::Black);

	if (population.allDotsDead()) {
		population.reset();
		progress = 0;
		resetAreas();
	}
	m_window.draw(areas[0].getBackground());
	m_window.draw(areas[1].getBackground());
	m_window.draw(areas[2].getBackground());
	m_window.draw(areas[3].getBackground());
	m_window.draw(areas[4].getBackground());

	std::vector<Sprite> areaSprites = areas[progress].drawBlocks();
	for (int i = 0; i < areaSprites.size(); i++) {
		m_window.draw(areaSprites[i]);
	}
	if (progress + 1 < areas.size()) {
		areaSprites = areas[progress + 1].drawBlocks();
		for (int i = 0; i < areaSprites.size(); i++) {
			m_window.draw(areaSprites[i]);
		}
	}
	

	m_window.draw(population.players[0].getShape());

	if (visualize) {
		m_window.draw(graphArray);
	}

	m_window.display();

}

