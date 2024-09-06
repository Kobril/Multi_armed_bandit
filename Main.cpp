#pragma once
#include <iostream>
#include <random>
#include <array>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include "Actor.h"
#include "Game.h"
#include "Bandit.h"

int main()
{
	//Init game engine
	Game game;

	//Actor
	Actor actor;

	actor.setGame(&game);

	//Type of bandit
	actor.setActionSelectionStrategy(2);

	//Setting the acton
	game.setActor(actor);

	//Game loop
	while (game.running())
	{	
		// Update actor's decision
		game.updateActor();
		//update
		game.update();
		//Render
		game.render();

		//sf::sleep(sf::seconds(0.1f));
	}
	
	// Data for plotting
	std::vector<double> temp1 = game.getRewards();
	std::cout << "Vector elements: ";
	for (const auto& element : temp1) {
		std::cout << element << ", ";
	}
	std::cout << "\n===========================================\n";
	std::vector<bool> temp2 = game.getBl();
	std::cout << "Vector elements: ";
	for (const auto& element : temp2) {
		std::cout << element << ", ";
	}
	return 0;
}