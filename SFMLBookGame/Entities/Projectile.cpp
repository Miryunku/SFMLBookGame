#include "../pch.h"
#include "Projectile.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"
#include "../Others/Utility.h"
#include "../IDs/Categories.h"
#include "../SceneryArchitecture/Emitter_node.h"

std::vector<Projectile_data> tables::initialize_projectile_table()
{
    std::vector<Projectile_data> table(static_cast<int>(Projectile::Type::typecount));
    table[static_cast<int>(Projectile::Type::allied_bullet)].damage_points = 10;
    table[static_cast<int>(Projectile::Type::allied_bullet)].speed = 300.0f;
    table[static_cast<int>(Projectile::Type::allied_bullet)].texture_id = Id_textures::entities;
    table[static_cast<int>(Projectile::Type::allied_bullet)].texture_rect = sf::IntRect {175, 64, 3, 14};

    table[static_cast<int>(Projectile::Type::enemy_bullet)].damage_points = 10;
    table[static_cast<int>(Projectile::Type::enemy_bullet)].speed = 300.0f;
    table[static_cast<int>(Projectile::Type::enemy_bullet)].texture_id = Id_textures::entities;
    table[static_cast<int>(Projectile::Type::enemy_bullet)].texture_rect = sf::IntRect {178, 64, 3, 14};

    table[static_cast<int>(Projectile::Type::missile)].damage_points = 200;
    table[static_cast<int>(Projectile::Type::missile)].speed = 150.0f;
    table[static_cast<int>(Projectile::Type::missile)].texture_id = Id_textures::entities;
    table[static_cast<int>(Projectile::Type::missile)].texture_rect = sf::IntRect {160, 64, 15, 32};

    return table;
}

namespace {
    const std::vector<Projectile_data>& projectile_table {tables::initialize_projectile_table()};
}

Projectile::Projectile(Type ttype, const Texture_holder& ttextures, Sound_player& ssound_player)
    : Entity {1, ssound_player},
    type {ttype},
    sprite {ttextures.get_resource(projectile_table[static_cast<int>(ttype)].texture_id), projectile_table[static_cast<int>(ttype)].texture_rect}
{
    center_origin(sprite);
    if(is_guided()) {
        std::unique_ptr<Emitter_node> smoke_emitter {std::make_unique<Emitter_node>(Particle::Type::smoke)};
        smoke_emitter->setPosition(0.0f, get_bounding_rect().height / 2.0f);
        attach_child(std::move(smoke_emitter));

        std::unique_ptr<Emitter_node> fire_emitter {std::make_unique<Emitter_node>(Particle::Type::propelland)};
        fire_emitter->setPosition(0.0f, get_bounding_rect().height / 2.0f);
        attach_child(std::move(fire_emitter));
    }
}

bool Projectile::is_removal() const
{
    return is_destroyed();
}

bool Projectile::is_guided() const
{
    return type == Type::missile;
}

void Projectile::guide_towards(sf::Vector2f pposition)
{
    if(!is_guided()) {
        throw std::logic_error {"Projectile::guide_towards"};
    }
    target_direction = unit_vector(pposition - get_world_position());
}

float Projectile::get_maxspeed() const
{
    return projectile_table[static_cast<int>(type)].speed;
}

int Projectile::get_damage_points() const
{
    return projectile_table[static_cast<int>(type)].damage_points;
}

sf::FloatRect Projectile::get_bounding_rect() const
{
    return get_world_transform().transformRect(sprite.getGlobalBounds());
}

unsigned int Projectile::get_category() const
{
    switch(type) {
    case Type::allied_bullet:
    case Type::missile:
        return entity_categories::allied_projectile;
    case Type::enemy_bullet:
        return entity_categories::enemy_projectile;
    }
}

void Projectile::update_current(sf::Time ffixed_timestep, Command_queue& ccomand_queue)
{
    if(is_guided()) {
        constexpr float approach_rate {200.0f};
        sf::Vector2f new_velocity {unit_vector(get_velocity() + (approach_rate * ffixed_timestep.asSeconds() * target_direction))};
        new_velocity *= get_maxspeed();
        set_velocity(new_velocity);
        float angle {std::atan2(new_velocity.y, new_velocity.x)};
        setRotation(to_degree(angle) + 90.0f);
    }
    Entity::update_current(ffixed_timestep, ccomand_queue);
}

void Projectile::draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
{
    ttarget.draw(sprite, sstates);
}
