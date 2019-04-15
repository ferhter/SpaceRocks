#include "scene_game.h"
#include "../game.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "../input.h"
#include "system_renderer.h"
#include "maths.h"
#include <stdlib.h>
#include <time.h>
#include "system_resources.h"
#include <random>
#include "..\ship_factory.h"
#include "Box2D/Box2D.h"
#include "system_physics.h"
#include "..\asteroid_factory.h"
#include "..\components\components.h"
#include <ctime>
#include "..\ContactListener.h"

using namespace std;
using namespace sf;

std::shared_ptr<Entity> game;
std::shared_ptr<PanelComponent> gamePanel;

std::vector<std::shared_ptr<Entity>> asteroids;
std::shared_ptr<sf::Texture> ssAsteroids;
default_random_engine randomGenerator((int)time(NULL));
uniform_real_distribution<float> distrib(-1.0f, 1.0f);
float PSI = Physics::physics_scale_inv;
myContactListener contactListenerInstance;


void GameScene::Load() {
	cout << "Game Scene Load \n";	
	{
		// Game panel
		game = makeEntity();
		game->setPosition(sf::Vector2f(16.0f, 16.0f));
		gamePanel = game->addComponent<PanelComponent>(sf::Vector2f(0.0f, 0.0f));
		gamePanel->addText([]() -> std::string { time_t now = time(0); return std::ctime(&now); });
	}

	// Load spritesheets
	ssAsteroids = Resources::load<Texture>("asteroid-1.png");

	// Player ship
	auto player = ShipFactory::makePlayer();
	player->get_components<PhysicsComponent>()[0]->teleport(Vector2f(GAMEX / 2, GAMEY / 2));

	//Test Enemy
	auto test = ShipFactory::makeEnemy(2);
	test->get_components<PhysicsComponent>()[0]->teleport(Vector2f(GAMEX / 4, GAMEY / 2));

	//Creat edges
	createEdges();

	//Set contact listener
	auto body = player->get_components<PhysicsComponent>()[0]->getBody();
	auto world = body->GetWorld();
	world->SetContactListener(&contactListenerInstance);


	setLoaded(true);
}

void GameScene::SpawnAsteroid()
{	
	// Generate random position off screen
	float rx = distrib(randomGenerator);
	float ry = distrib(randomGenerator);
	cout << "rx: " << rx << "  ry: " << ry;
	//get normalised direction from random values
	sf::Vector2f dir = sf::Vector2f(rx, ry);
	dir = sf::normalize<float>(dir);
	//calculate center of screen
	sf::Vector2f center = sf::Vector2f(GAMEX/2, GAMEY/2);
	//Set asteroid starting position
	auto asteroid = AsteroidFactory::makeAsteroid(11, center + dir * 800.0f);

	//Set velocity back towards center
	//TODO: Random variation to prevent all asteroids heading straight to center.
	asteroid->get_components<PhysicsComponent>()[0]->setVelocity(sf::Vector2f(dir.x, -dir.y) * -25.0f);

	//Add to collection
	asteroids.push_back(asteroid);
}

void GameScene::createEdges()
{
	// Create entity
	auto edges = makeEntity();
	edges->setPosition(sf::Vector2f(0.0f, 0.0f));

	// Physics
	auto phys = edges->addComponent<PhysicsComponent>(false, sf::Vector2f(1.0f, 1.0f));
	//Get corners
	b2Vec2 upLeft = b2Vec2(0.0f, 0.0f);
	b2Vec2 downLeft = b2Vec2(0.0f, -GAMEY * PSI);
	b2Vec2 downRight = b2Vec2(GAMEX * PSI, -GAMEY * PSI);
	b2Vec2 upRight = b2Vec2(GAMEX * PSI, 0.0f);
	//Create Shape
	b2EdgeShape edgeShape;
	//Create Fixtures
	b2FixtureDef fixtureDef;
	fixtureDef.filter.categoryBits = EDGE;
	fixtureDef.filter.maskBits = PLAYER_SHIP;
	// left
	edgeShape.Set(upLeft, downLeft);
	fixtureDef.shape = &edgeShape;
	phys->setFixtureDef(fixtureDef);
	// bottom
	edgeShape.Set(downLeft, downRight);
	fixtureDef.shape = &edgeShape;
	phys->setFixtureDef(fixtureDef);
	// right
	edgeShape.Set(downRight, upRight);
	fixtureDef.shape = &edgeShape;
	phys->setFixtureDef(fixtureDef);
	// top
	edgeShape.Set(upRight, upLeft);
	fixtureDef.shape = &edgeShape;
	phys->setFixtureDef(fixtureDef);

}

void GameScene::Update(const double& dt) {
	 
	//If less than 5 total asteroids, spawn another big asteroid.
	for (int i = 0; i < asteroids.size(); i++)
	{
		if (!asteroids[i]->isAlive())
		{
			asteroids.erase(asteroids.begin() + i);
			asteroids.shrink_to_fit();
		}
	}
	
	if (asteroids.size() < 5)
	{
		SpawnAsteroid();
	}

	Scene::Update(dt);
}

