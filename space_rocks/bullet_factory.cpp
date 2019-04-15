#include "bullet_factory.h"

//Key is ColRow starting top left: 11
std::map < unsigned int, ObjectData > BulletFactory::_objectData =
{
	//2,4: Player Bullet
	{24,
		{
			{
				{0.0f * PSI4, 2.0f * PSI4},
				{-2.0f * PSI4, 0.0f * PSI4},
				{0.0f * PSI4, -4.0f * PSI4},
				{2.0f * PSI4, 0.0f * PSI4}
			},
			{
				Resources::load<sf::Texture>("projectiles.png")
			},
			{
				sf::IntRect(64, 192, 64, 64)
			}
		}
	}
};

//Make a bullet of the corresponding type
std::shared_ptr<Entity> BulletFactory::makeBullet(unsigned int id)
{
	switch (id)
	{
	//2,4: Player Bullet
	case 24:
		//Make entity
		std::shared_ptr<Entity> bullet = gameScene.makeEntity();

		// Physics
		{
			auto phys = bullet->addComponent<PhysicsComponent>(true, sf::Vector2f(16.0f, 16.0f));
			//Collider
			//Create fixturedef and shape
			b2FixtureDef fixtureDef;
			fixtureDef.filter.groupIndex = 1;
			fixtureDef.filter.categoryBits = PLAYER_BULLET;
			fixtureDef.filter.maskBits = ENEMY_BULLET | ENEMY_SHIP | ASTEROIDS;
			b2PolygonShape shape;
			//Set collider to sensor
			fixtureDef.isSensor = true;
			//Assign vertices to shape
			shape.Set(&_objectData[24]._coords.front(), _objectData[24]._coords.size());
			//Assign shape to fixtureDef
			fixtureDef.shape = &shape;
			//Assign fixtureDef to physics component
			phys->setFixtureDef(fixtureDef);
		}

		// Sprite
		{
			auto sprite = bullet->addComponent<SpriteComponent>();
			sprite->setTexure(_objectData[24]._tex);
			sprite->setTextureRect(_objectData[24]._texRect);
			sprite->SetAnchor(sf::Vector2f(0.5f, 0.5f));
		}

		//Destructible
		{
			auto des = bullet->addComponent<DestructibleComponent>(1.0f, 0);
		}

		//Bullet Component
		{
			//5 seconds lifetime
			auto des = bullet->addComponent<BulletComponent>(5.0f);		
		}

		return bullet;
		break;
	}

	std::cout << "Invalid ID" << std::endl;
	return NULL;
}