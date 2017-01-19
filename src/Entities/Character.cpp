#include "Entities\Character.h"
#include "Utilities.h"
#include "Map\WorldMap.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <iostream>

Character::Character(SharedContext& sharedContext, const EntityType type, const sf::Vector2f& pos, const int ID, const std::string& name)
	: Entity(sharedContext, type, pos, ID, name),
	m_maxLives(0),
	m_lives(0),
	m_damage(0),
	m_hurtTime(0),
	m_jumpTime(0),
	m_doubleJumpTime(0),
	m_jumpVelocity(0),
	m_jumpReady(true)
{
	loadInDetails();
}

void Character::update(const float deltaTime)
{
	Entity::update(deltaTime);
	
	if ((std::abs(Entity::getVelocity().x) > 0.1f) || std::abs(Entity::getVelocity().y) > 5.0f)
	{
		setState(EntityState::Walking);
	}
	else
	{
		setState(EntityState::Idling);
	}

	determineAnimationType();
	handleTimers(deltaTime);
	checkBounds();
}

void Character::handleTileCollisions(const std::vector<CollisionElement*>& collisions)
{
	Entity::handleTileCollisions(collisions);

	if (Entity::isOnTile())
	{
		m_jumpReady = true;
	}
}

void Character::determineAnimationType()
{
	AnimationManager& animationManager = Entity::getSpriteSheet();
	const Direction dir = Entity::getCurrentDirection();

	switch (m_state)
	{
	case (EntityState::Idling) :
	{
		animationManager.setAnimationType("Idling", dir);
		break;
	}
	case (EntityState::Walking) :
	{
		animationManager.setAnimationType("Walking", dir);
		break;
	}
	case (EntityState::Hurt) :
	{
		const Animation* const animation = animationManager.getCurrentAnimation();
		if (animation && animation->isFinished())
		{
			animationManager.setAnimationType("Idling", dir);
			break;
		}
	
		animationManager.setAnimationType("Idling", dir);
		break;
	}
	case (EntityState::Dead) :
	{
		break;
	}
	}
}

void Character::setState(const EntityState state)
{
	if (m_state != EntityState::Dead)
	{
		m_state = state;
	}
}

void Character::loadInDetails()
{
	const Utilities& utilities = Entity::getSharedContext().m_utilities;
	std::ifstream file(utilities.getEntityDetails(Entity::getName()));
	assert(file.is_open());

	std::string line;
	while (std::getline(file, line))
	{
		std::stringstream keyStream(line);
		std::string type;
		keyStream >> type;

		if (type == "Animations")
		{
			std::string animationFileName;
			keyStream >> animationFileName;
			Entity::getAnimationManager().loadInAnimations(animationFileName);
		}
		else if (type == "Speed")
		{
			sf::Vector2f speed;
			keyStream >> speed.x >> speed.y;
			Entity::setSpeed(speed);
		}
		else if (type == "JumpTime")
		{
			keyStream >> m_jumpTime;
			m_jumpingTimer.setExpirationTime(m_jumpTime);
		}
		else if (type == "DoubleJumpTime")
		{
			keyStream >> m_doubleJumpTime;
			m_doubleJumpTimer.setExpirationTime(m_doubleJumpTime);
		}
		else if (type == "MaxVelocity")
		{
			sf::Vector2f maxVel;
			keyStream >> maxVel.x >> maxVel.y;
			Entity::setMaxVelocity(maxVel);
		}
		else if (type == "HurtTime")
		{
			keyStream >> m_hurtTime;
			m_hurtTimer.setExpirationTime(m_hurtTime);
		}
		else if (type == "Damage")
		{
			keyStream >> m_damage;
		}
		else if (type == "JumpVelocity")
		{
			keyStream >> m_jumpVelocity;
		}
		else if (type == "MaxLives")
		{
			keyStream >> m_maxLives;
			m_lives = m_maxLives;
		}
	}
}

void Character::reduceLife()
{
	//Only allow character to take damage periodic intervals
	if (m_currentState == EntityState::Dead || m_hurtTimer.isActive()) 
	{
		return;
	}

	--m_lives;
	std::cout << "Lives: " << m_lives << "\n";

	if(m_lives <= 0)	
	{ 
		m_lives = 0;
		killCharacter();
		
		//m_velocity.x = 0;
		//setState(EntityState::Dead);
		//m_spriteSheet.setAnimationType("Dead", m_currentDirection);	
	}
	else
	{
		//getSpriteSheet().setAnimationType(AnimationName::Hurt, getDirection());
		//getAudioPlayer().play("Hurt", false);
		setState(EntityState::Hurt);
		m_hurtTimer.activate();
		Entity::setVelocity(0, 0);
	}
}

void Character::jump()
{
	if (m_jumpReady && m_currentState != EntityState::Dead)
	{
		std::cout << "Hi";
		//getAudioPlayer().play("Jump", false);
		m_jumpReady = false;
		m_jumpingTimer.activate();
	}
}

void Character::doubleJump()
{
	if (m_jumpReady && m_currentState != EntityState::Dead)
	{
		m_jumpReady = false;
		m_doubleJumpTimer.activate();
	}
}

void Character::killCharacter()
{
	if (m_currentState != EntityState::Dead)
	{
		setState(EntityState::Dead);
		Entity::setVelocity(0, 0);
		//getAudioPlayer().play("Hurt", false);
		//getSpriteSheet().setAnimationType(AnimationName::Dead, getDirection());s
	}
}
void Character::handleTimers(const float deltaTime)
{
	//Jump Timer
	m_jumpingTimer.update(deltaTime);
	if (m_jumpingTimer.isActive())
	{
		addVelocity(0, -m_jumpVelocity);
		//std::cout << "hi";
		if (m_jumpingTimer.isExpired())
		{
			m_jumpingTimer.deactivate();
		}
	}

	//Double Jump Timer
	m_doubleJumpTimer.update(deltaTime);
	if (m_doubleJumpTimer.isActive())
	{
		addVelocity(0, -m_jumpVelocity);
		if (m_doubleJumpTimer.isExpired())
		{
			m_doubleJumpTimer.deactivate();
		}
	}

	//Hurt Timer
	m_hurtTimer.update(deltaTime);
	if (m_hurtTimer.isActive())
	{
		if (m_hurtTimer.isExpired())
		{
			m_hurtTimer.deactivate();
		}
	}

}

void Character::checkBounds()
{
	const MapDetails& mapDetails = Entity::getSharedContext().m_worldMap.getMapDetails();
	if (Entity::getPosition().y > (mapDetails.m_mapSize.y * mapDetails.m_tileSize))
	{
		reduceLife();
		//Entity::setToStartPosition();
	}
	else if (Entity::getPosition().x < 0)
	{
		Entity::setPosition(sf::Vector2f(0, Entity::getPosition().y));
	}
	else if (Entity::getPosition().x > (mapDetails.m_mapSize.x * mapDetails.m_tileSize))
	{
		Entity::setPosition(sf::Vector2f(mapDetails.m_mapSize.x * mapDetails.m_tileSize, Entity::getPosition().y));
	}
}
 
void Character::attack()
{
	//Disallow attacking within these states
	if (m_currentState == EntityState::Attacking||
		m_currentState == EntityState::Dead ||
		m_currentState == EntityState::Hurt)
	{
		return;
	}

	setState(EntityState::Attacking);
	//getSpriteSheet().setAnimationType(AnimationName::Attack, getDirection());
	//getAudioPlayer().play("Attack", false);
	stop();
	//m_attackManager.startAttack(*this);
}