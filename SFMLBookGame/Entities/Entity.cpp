#include "../pch.h"
#include "Entity.h"

Entity::Entity(int hhealth_points, Sound_player& ssound_player)
    : health_points {hhealth_points},
    entity_sound_player {ssound_player}
{
    //empty by default
}

void Entity::set_velocity(sf::Vector2f vvelocity)
{
    velocity = vvelocity;
}

void Entity::set_velocity(float xx, float yy)
{
    velocity.x = xx;
    velocity.y = yy;
}

sf::Vector2f Entity::get_velocity() const
{
    return velocity;
}

void Entity::update_current(sf::Time ffixed_timestep, Command_queue& ccommand_queue)
{
    move(velocity * ffixed_timestep.asSeconds());
}

void Entity::accelerate(sf::Vector2f vvelocity)
{
    velocity += vvelocity;
}

void Entity::accelerate(float xx, float yy)
{
    velocity.x += xx;
    velocity.y += yy;
}

void Entity::repair(int hhealth_points)
{
    if(health_points <= 0) {
        return;
    }
    health_points += hhealth_points;

}
void Entity::damage(int hhealth_points)
{
    if(health_points <= 0) {
        return;
    }
    health_points -= hhealth_points;
}

void Entity::destroy()
{
    health_points = 0;
}

void Entity::remove()
{
    health_points = 0;
    //special operations go here
}

int Entity::get_health_points()
{
    return health_points;
}

bool Entity::is_destroyed() const
{
    return health_points <= 0;
}

Sound_player& Entity::get_dj() const
{
    return entity_sound_player;
}
