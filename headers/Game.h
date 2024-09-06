#pragma once

#include <SFML/Graphics.hpp>
#include <cmath> 
#include <vector>
#include "Bandit.h"
#include "Actor.h"

class Actor;
class Game {
private:
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;

    // Define tile types
    enum TileType {
        Green, // normal tile
        Red,   // pits
        Black  // obstacle
    };

    //tile map's height and width
    static const int mapWidth = 3;
    static const int mapHeight = 3;

    //Max stairs height
    static const int stairs = 2;

    //Threshold matrix
    int threshold[mapWidth][mapHeight];

    //tileMap and appropriate bandits
    TileType tileMap[mapWidth][mapHeight];

    //Bandits and their parameters
    Bandit* bandits[mapWidth][mapHeight];

    //Values for black (pit) tiles
    int blackValues[mapWidth][mapHeight];
    
    //Rewards
    std::vector<double> rewards;
    
    //Moved into black tile
    std::vector<bool> bl;

    //Actor
    Actor actor;

    void initVariables();
    void initWindow();
    void initTileMap();

public:
    Game();
    ~Game();

    std::vector<double> getRewards();
    std::vector<bool> getBl();
    int getMapWidth();
    int getMapHeight();
    int getThreshold(int i, int j);
    sf::Color getTileColor(TileType type);

    void setActor(Actor NewActor);

    const bool running() const;

    void pollEvents();
    void render();
    void update();

    void drawActor();
    void updateActor();
};
