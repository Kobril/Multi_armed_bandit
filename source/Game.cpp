#include "Game.h"
#include "Actor.h"
#include <exception>
#include <vector>

Game::Game() 
{
    this->initVariables();
    this->initWindow();
    this->initTileMap();
}

Game::~Game() 
{
    delete this->window;
    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            delete bandits[i][j];
        }
    }
}

std::vector<double> Game::getRewards()
{
    return rewards;
}

std::vector<bool> Game::getBl()
{
    return bl;
}

int Game::getMapWidth()
{
    return mapWidth; 
}

int Game::getMapHeight()
{
    return mapHeight;
}

int Game::getThreshold(int i, int j)
{
    if (i >= mapWidth || j >= mapHeight)
        throw std::exception("Index out of range");
    return threshold[i][j];
}

void Game::initVariables() 
{
    this->window = nullptr;
    this->rewards = std::vector<double>();
    this->bl = std::vector<bool>();
}

void Game::initWindow() 
{
    this->videoMode.height = 600;
    this->videoMode.width = 800;

    this->window = new sf::RenderWindow(this->videoMode, "Discete field", sf::Style::Titlebar | sf::Style::Close);
}

void Game::initTileMap() 
{
    double epsilon = 0.2;
    double learning_rate = 0.2;
    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            // Assign green tiles
            tileMap[i][j] = Green;

            // Bandit initialization
            switch (i) {
            case 0:
            case mapWidth - 1:
                switch (j) {
                case 0:
                case mapHeight - 1:
                    bandits[i][j] = new Bandit(2 + stairs, epsilon, learning_rate);
                    threshold[i][j] = 2;
                    break;
                default:
                    bandits[i][j] = new Bandit(3 + stairs, epsilon, learning_rate);
                    threshold[i][j] = 3;
                    break;
                }
                break;
            default:
                switch (j) {
                case 0:
                case mapHeight - 1:
                    bandits[i][j] = new Bandit(3 + stairs, epsilon, learning_rate);
                    threshold[i][j] = 3;
                    break;
                default:
                    bandits[i][j] = new Bandit(4 + stairs, epsilon, learning_rate);
                    threshold[i][j] = 4;
                    break;
                }
                break;
            }
        }
    }

    // Assign other tile types
    tileMap[1][1] = Red;

    tileMap[1][2] = Black;
    blackValues[1][2] = 1;
}

void Game::setActor(Actor NewActor) 
{
    actor = NewActor;
}

const bool Game::running() const 
{
    return this->window->isOpen();
}

void Game::pollEvents() {
    // event polling
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (ev.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        }
    }
}

void Game::render() {
    window->clear();

    for (int i = 0; i < mapWidth; ++i) {
        for (int j = 0; j < mapHeight; ++j) {
            sf::RectangleShape tile(sf::Vector2f(80.f, 60.f)); // Tile
            tile.setPosition(i * 80.f, j * 60.f);
            tile.setFillColor(getTileColor(tileMap[i][j]));
            tile.setOutlineThickness(2.f);
            tile.setOutlineColor(sf::Color::Black);
            window->draw(tile);
        }
    }

    drawActor();

    this->window->display();
}

void Game::update() {
    this->pollEvents();
}

void Game::drawActor() {
    sf::Vector2i actorPosition = actor.getPosition();
    sf::CircleShape actorShape(20.f); // Adjust the size as needed
    actorShape.setFillColor(sf::Color::Blue);

    // Set actor's position in the center of the tile
    actorShape.setPosition((actorPosition.x) * 80.f, (actorPosition.y) * 60.f);

    window->draw(actorShape);

    // Display actor position
    sf::Text text;
    text.setString("Actor Position: (" + std::to_string(actorPosition.x) + ", " + std::to_string(actorPosition.y) + ")");
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 10); // Adjust the position as needed
    window->draw(text);
}

void Game::updateActor()
{
    sf::Vector2i actorPosition = actor.getPosition();
    Bandit* currentTileBandit = bandits[actorPosition.x][actorPosition.y];
    actor.setTileBandit(currentTileBandit);
    int action = actor.move();
    std::cout << "action: " << action << "\n" << "Position: " << actorPosition.x << " " << actorPosition.y << "\n";
    double reward = 0;
    
    if (action < threshold[actorPosition.x][actorPosition.y]) //if actor moves
    {
        sf::Vector2i actorPositionNew = actor.getPosition();
        Bandit* currentTileBanditNew = bandits[actorPositionNew.x][actorPositionNew.y];
        TileType currentCellType = tileMap[actorPositionNew.x][actorPositionNew.y];

        int OldDistance = std::abs((mapWidth - 1) - actorPosition.x) + std::abs((mapHeight - 1) - actorPosition.y);
        int NewDistance = std::abs((mapWidth - 1) - actorPositionNew.x) + std::abs((mapHeight - 1) - actorPositionNew.y);
        
        int actionNew;

        switch (currentCellType)
        {
        case Green: //Actor stepped on a green tile
            if (OldDistance - NewDistance > 0)
                reward = 0.1;
            bl.push_back(0);
            break;
        case Red: //Actor stepped on a red tile
            reward = -100;
            bl.push_back(0);
            break;
        case Black: //Actor stepped on a black tile
            actor.setTileBandit(currentTileBanditNew);
            actionNew = actor.blackMove();
            if (actionNew < blackValues[actorPositionNew.x][actorPositionNew.y]) //Actor doesn't surpass the obstacle
            {
                if (OldDistance - NewDistance > 0)
                    reward = 0.1;
                actor.setCurrentPosition(actorPosition.x, actorPosition.y);
                actor.setTileBandit(currentTileBandit);
            }
            else //Actor surpasses the obstacle
            {
                reward = 10;
                switch (actor.getActionSelectionStrategy()) {
                case 0: // Epsilon-greedy action
                case 1:
                    // UCB action
                    currentTileBanditNew->update_q(reward, action);
                    currentTileBanditNew->print_q();
                    break;
                case 2:
                    // Gradient bandit action
                    currentTileBanditNew->update_avg_reward(currentTileBanditNew->number_of_actions_taken(), reward);
                    currentTileBanditNew->update_action_preferences(reward, action);
                    currentTileBanditNew->print_action_preferences();
                    break;
                default:
                    // Epsilon-greedy action
                    currentTileBanditNew->update_q(reward, action);
                    break;
                }
            }
            bl.push_back(1);
            break;
        default:
            bl.push_back(0);
            break;
        }
        if (actorPositionNew.x == mapWidth - 1 && actorPositionNew.y == mapHeight - 1) //If actor reaches the end
        {
            actor.setCurrentPosition(0, 0);
            std::cout << "=================================You win!==========================";
            reward = 100;
        }
    }
    else
    {
        bl.push_back(0);
        reward = -0.1;
    }
    std::cout << "reward: " << reward;
    switch (actor.getActionSelectionStrategy()) {
    case 0: // Epsilon-greedy action
    case 1:
        // UCB action
        currentTileBandit->update_q(reward, action);
        currentTileBandit->print_q();
        break;
    case 2:
        // Gradient bandit action
        currentTileBandit->update_avg_reward(currentTileBandit->number_of_actions_taken(), reward);
        currentTileBandit->update_action_preferences(reward, action);
        currentTileBandit->print_action_preferences();
        break;
    default:
        // Epsilon-greedy action
        currentTileBandit->update_q(reward, action);
        break;
    }
    rewards.push_back(reward);
}

sf::Color Game::getTileColor(TileType type) {
    switch (type) {
    case Green:
        return sf::Color::Green;
    case Red:
        return sf::Color::Red;
    case Black:
        return sf::Color::Black;
    default:
        return sf::Color::White;
    }
}
