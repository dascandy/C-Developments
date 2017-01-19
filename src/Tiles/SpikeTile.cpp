#include "Tiles\SpikeTile.h"
#include "Entities\Player.h"


SpikeTile::SpikeTile(SharedContext& sharedContext, const std::string& name, const sf::Vector2f& pos, const int ID)
	: InteractiveTile(sharedContext, name, pos, ID, TileType::Spike)
{

}


SpikeTile::~SpikeTile()
{
}

void SpikeTile::activate(Player & player)
{
	player.reduceLife();
}