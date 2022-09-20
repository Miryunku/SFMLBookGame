#include "../pch.h"
#include "Pickup.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"
#include "Aircraft.h"
#include "../Others/Utility.h"
#include "../IDs/Categories.h"

std::vector<Pickup_data> tables::initialize_pickup_table()
{
    std::vector<Pickup_data> table(static_cast<int>(Pickup::Type::typecount));
    table[static_cast<int>(Pickup::Type::health_refill)].texture_id = Id_textures::entities;
    table[static_cast<int>(Pickup::Type::health_refill)].texture_rect = sf::IntRect {0, 64, 40, 40};
    table[static_cast<int>(Pickup::Type::health_refill)].action = [](Aircraft& aaircraft)
    {
        aaircraft.repair(25);
    };

    table[static_cast<int>(Pickup::Type::missile_refill)].texture_id = Id_textures::entities;
    table[static_cast<int>(Pickup::Type::missile_refill)].texture_rect = sf::IntRect {40, 64, 40, 40};
    table[static_cast<int>(Pickup::Type::missile_refill)].action = [](Aircraft& aaircraft)
    {
        aaircraft.refill_missiles(2);
    };

    table[static_cast<int>(Pickup::Type::fire_spread)].texture_id = Id_textures::entities;
    table[static_cast<int>(Pickup::Type::fire_spread)].texture_rect = sf::IntRect {80, 64, 40, 40};
    table[static_cast<int>(Pickup::Type::fire_spread)].action = [](Aircraft& aaircraft)
    {
        aaircraft.increase_spread_level();
    };

    table[static_cast<int>(Pickup::Type::fire_rate)].texture_id = Id_textures::entities;
    table[static_cast<int>(Pickup::Type::fire_rate)].texture_rect = sf::IntRect {120, 64, 40, 40};
    table[static_cast<int>(Pickup::Type::fire_rate)].action = [](Aircraft& aaircraft)
    {
        aaircraft.increase_fire_rate();
    };

    return table;
}

namespace {
    std::vector<Pickup_data> pickup_table {tables::initialize_pickup_table()};
}

Pickup::Pickup(Type ttype, const Texture_holder& ttextures, Sound_player& ssound_player)
    : Entity {1, ssound_player},
    type {ttype},
    sprite {ttextures.get_resource(pickup_table[static_cast<int>(ttype)].texture_id), pickup_table[static_cast<int>(ttype)].texture_rect}
{
    center_origin(sprite);
}

void Pickup::apply(Aircraft& aaircraft)
{
    pickup_table[static_cast<int>(type)].action(aaircraft);
}

unsigned int Pickup::get_category() const
{
    return entity_categories::pickup;
}

sf::FloatRect Pickup::get_bounding_rect() const
{
    return get_world_transform().transformRect(sprite.getGlobalBounds());
}

bool Pickup::is_removal() const
{
    return is_destroyed();
}

void Pickup::draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
{
    ttarget.draw(sprite, sstates);
}
