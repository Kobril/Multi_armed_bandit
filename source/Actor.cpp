#include "Actor.h"
#include "Game.h"

Actor::Actor() : X(0), Y(0), currentTileBandit(nullptr), game(nullptr) {}

Actor::~Actor() {}

void Actor::setTileBandit(Bandit* bandit) {
    currentTileBandit = bandit;
}

void Actor::setGame(Game* gameInstance) {
    game = gameInstance;
}

void Actor::setActionSelectionStrategy(int i) {
    actionSelectionStrategy = i;
}

void Actor::setX(int x)
{
    X = x;
}

void Actor::setY(int y)
{
    Y = y;
}

void Actor::setCurrentPosition(int x, int y)
{
    X = x;
    Y = y;
}

int Actor::move() {
    int mapWidth = game->getMapWidth();
    int mapHeight = game->getMapHeight();
    // Use the bandit to decide the movement direction
    int action;
    switch (actionSelectionStrategy) {
    case 0:
        // Random action
        action = currentTileBandit->take_action();
        break;
    case 1:
        // UCB action
        action = currentTileBandit->UCB(currentTileBandit->number_of_actions_taken(), 0.1);
        break;
    case 2:
        // Gradient bandit action
        action = currentTileBandit->gradientBanditAction();
        break;
        // Add more cases for additional action selection strategies if needed
    default:
        // Default to random action
        action = currentTileBandit->take_action();
        break;
    }
    // Update the position based on the action
    if (action < game->getThreshold(X, Y)) //Movement
    {
        switch (action) {
        case 0:
            if (X == 0 && Y == 0) // Top left
            {
                Y++;
                break;
            }
            else if (X == 0) // Bottom left and Middle left
            {
                Y--;
                break;
            }
            X--; // Move left
            break;
        case 1:
            if (X == mapWidth - 1 && Y == 0) // Top right
            {
                Y++;
                break;
            }
            else if (X == mapWidth - 1 && Y == mapHeight - 1) // Bottom right
            {
                Y--;
                break;
            }
            else if (X == mapWidth - 1 && (Y != 0 && Y != mapHeight - 1)) // Middle right
            {
                Y++;
                break;
            }
            X++; // Move right
            break;
        case 2:
            if ((X != 0 && X != mapWidth - 1) && Y == 0) // middle top
            {
                Y++;
                break;
            }
            else if (X == 0 && (Y != 0 && Y != mapHeight - 1)) // Middle left
            {
                Y++;
                break;
            }
            Y--; // Move up
            break;
        case 3:
            Y++; // Move down
            break;
        }
    }
    if (X == mapWidth - 1 && Y == mapHeight - 1) {
        // Teleport to the top left cell
        
    }
    return action;
}

int Actor::blackMove() //Value of a bandit, when moving into a black tile
{
    // Use the bandit to decide the movement direction
    int action;
    switch (actionSelectionStrategy) {
    case 0:
        // Epsilon-greedy action
        action = currentTileBandit->take_action();
        break;
    case 1:
        // UCB action
        action = currentTileBandit->UCB(currentTileBandit->number_of_actions_taken(), 0.5);
        break;
    case 2:
        // Gradient bandit action
        action = currentTileBandit->gradientBanditAction();
        break;
    default:
        // Epsilon-greedy action
        action = currentTileBandit->take_action();
        break;
    }
    // Update the position based on the action
    if (action < game->getThreshold(X, Y))
    {
        return 0; 
    }
    else
    {
        return action - game->getThreshold(X, Y) + 1;
    }
}

sf::Vector2i Actor::getPosition() const {
    return sf::Vector2i(X, Y);
}

int Actor::getActionSelectionStrategy() const
{
    return actionSelectionStrategy;
}
