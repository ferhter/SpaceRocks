#include "ship_factory.h"
#include "ecm.h"
#include "engine.h"
#include "game.h"
#include "components\cmp_sprite.h"
#include "components\cmp_physics.h"
#include "components\cmp_ship.h"
#include "components\cmp_destructible.h"
#include "components\cmp_player.h"
#include "system_resources.h"
#include <string>

std::shared_ptr<Entity> ShipFactory::makePlayer()
{
	auto player = makeShip();

	player->addComponent<PlayerComponent>(1);

	return player;
}

std::shared_ptr<Entity> ShipFactory::makeEnemy()
{
	return makeShip();
}

std::shared_ptr<Entity> ShipFactory::makeShip()
{
	std::shared_ptr<Entity> entity = gameScene.makeEntity();

	// Sprite
	auto sprite = entity->addComponent<SpriteComponent>();
	//sprite->setTextureRect(sf::IntRect(0, 0, 256, 256));
	sprite->setTexure(Resources::load<sf::Texture>("player.png"));
	sprite->SetAnchor(sf::Vector2f(0.0f, 0.0f));
	sprite->setOrigin(sf::Vector2f(128, 128));

	// Physics
	entity->addComponent<PhysicsComponent>(true, sf::Vector2f(192.0f, 192.0f));
	// Ship
	entity->addComponent<ShipComponent>(20.0f, 2.0f, 1.0f);
	// Destructible
	entity->addComponent<DestructibleComponent>(100.0f);

	return entity;
}