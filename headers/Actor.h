#pragma once

#include "Bandit.h"
#include <SFML/Graphics.hpp>

class Game;
class Actor {
private:
    //Actor's position
    int X;
    int Y;
    //Type of bandit
    int actionSelectionStrategy;
    //Current bandit
    Bandit* currentTileBandit; // Pointer to the bandit associated with the current tile
    //For game's characteristics
    Game* game;

public:
    Actor();
    ~Actor();

    void setTileBandit(Bandit* bandit);
    void setGame(Game* gameInstance);
    void setActionSelectionStrategy(int i);
    void setX(int x);
    void setY(int y);
    void setCurrentPosition(int x, int y);

    sf::Vector2i getPosition() const;
    int getActionSelectionStrategy() const;

    int move();
    int blackMove();
};
