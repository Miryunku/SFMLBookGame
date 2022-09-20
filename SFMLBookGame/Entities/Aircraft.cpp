#include "../pch.h"
#include "Aircraft.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"
#include "../IDs//Categories.h"
#include "../Others/Utility.h"
#include "Pickup.h"
#include "../MusicSoundProcess/Sound_player.h"

std::vector<Aircraft_data> tables::initialize_aircraft_table()
{
    std::vector<Aircraft_data> table(static_cast<int>(Aircraft::Type::type_count));
    table[static_cast<int>(Aircraft::Type::avenger)].health_points = 40;
    table[static_cast<int>(Aircraft::Type::avenger)].speed = 50.0f;
    table[static_cast<int>(Aircraft::Type::avenger)].support_roll = false;
    table[static_cast<int>(Aircraft::Type::avenger)].texture_id = Id_textures::entities;
    table[static_cast<int>(Aircraft::Type::avenger)].texture_rect = sf::IntRect {228, 0, 60, 59};
    table[static_cast<int>(Aircraft::Type::avenger)].fire_interval = sf::seconds(2.0f);
    table[static_cast<int>(Aircraft::Type::avenger)].directions.emplace_back(45.0f, 50.0f);
    table[static_cast<int>(Aircraft::Type::avenger)].directions.emplace_back(0.0f, 50.0f);
    table[static_cast<int>(Aircraft::Type::avenger)].directions.emplace_back(-45.0f, 100.0f);
    table[static_cast<int>(Aircraft::Type::avenger)].directions.emplace_back(0.0f, 50.0f);
    table[static_cast<int>(Aircraft::Type::avenger)].directions.emplace_back(45.0f, 50.0f);

    table[static_cast<int>(Aircraft::Type::eagle)].health_points = 100;
    table[static_cast<int>(Aircraft::Type::eagle)].speed = 200.0f;
    table[static_cast<int>(Aircraft::Type::eagle)].support_roll = true;
    table[static_cast<int>(Aircraft::Type::eagle)].texture_id = Id_textures::entities;
    table[static_cast<int>(Aircraft::Type::eagle)].texture_rect = sf::IntRect {0, 0, 48, 64};
    table[static_cast<int>(Aircraft::Type::eagle)].fire_interval = sf::seconds(1.0f);

    table[static_cast<int>(Aircraft::Type::raptor)].health_points = 20;
    table[static_cast<int>(Aircraft::Type::raptor)].speed = 80.0f;
    table[static_cast<int>(Aircraft::Type::raptor)].support_roll = false;
    table[static_cast<int>(Aircraft::Type::raptor)].texture_id = Id_textures::entities;
    table[static_cast<int>(Aircraft::Type::raptor)].texture_rect = sf::IntRect {144, 0, 84, 64};
    table[static_cast<int>(Aircraft::Type::raptor)].fire_interval = sf::Time::Zero;
    table[static_cast<int>(Aircraft::Type::raptor)].directions.emplace_back(45.0f, 80.0f);
    table[static_cast<int>(Aircraft::Type::raptor)].directions.emplace_back(-45.0f, 160.0f);
    table[static_cast<int>(Aircraft::Type::raptor)].directions.emplace_back(45.0f, 80.0f);

    return table;
}

namespace {
    const std::vector<Aircraft_data>& aircraft_table {tables::initialize_aircraft_table()};
}

Aircraft::Aircraft(Type ttype, const Texture_holder& ttextures, const Font_holder& ffonts, Sound_player& ssound_player)
    : Entity {aircraft_table[static_cast<int>(ttype)].health_points, ssound_player},
    type {ttype},
    sprite {ttextures.get_resource(aircraft_table[static_cast<int>(ttype)].texture_id), aircraft_table[static_cast<int>(ttype)].texture_rect},
    explosion {ttextures.get_resource(Id_textures::explosion)},
    directions {aircraft_table[static_cast<int>(ttype)].directions}
{
    explosion.set_total_frames(16);
    explosion.set_frame_size(sf::Vector2i {256, 256});
    explosion.set_duration(sf::seconds(1.0f));
    //----
    center_origin(sprite);
    center_origin(explosion);
    //----
    std::unique_ptr<Text_node> health_text {std::make_unique<Text_node>(ffonts)};
    health_text->setPosition(0.0f, 50.0f);
    health_display = health_text.get();
    attach_child(std::move(health_text));
    //----
    if(get_category() == entity_categories::player_aircraft) {
        std::unique_ptr<Text_node> missile_text {std::make_unique<Text_node>(ffonts)};
        missile_text->setPosition(0.0f, 70.0f);
        missile_display = missile_text.get();
        attach_child(std::move(missile_text));
    }
    //----
    bullet_command.category = entity_categories::scene;
    bullet_command.action = [this, &ttextures](Scene_node& nnode, sf::Time ffixed_timestep)
    {
        create_bullets(nnode, ttextures);
    };

    projectile_command.category = entity_categories::scene;
    projectile_command.action = [this, &ttextures](Scene_node& nnode, sf::Time ffixed_timestep)
    {
        create_projectile(nnode, Projectile::Type::missile, ttextures, 0.0f, 0.5f);
    };

    pickup_command.category = entity_categories::scene;
    pickup_command.action = [this, &ttextures](Scene_node& nnode, sf::Time ffixed_timestep)
    {
        create_pickup(nnode, ttextures);
    };
}

bool Aircraft::is_player() const
{
    return type == Type::eagle;
}

bool Aircraft::is_removal() const
{
    return removal_marker;
}

void Aircraft::remove()
{
    Entity::remove();
    show_explosion = false;
    removal_marker = true;
}

float Aircraft::get_max_speed() const
{
    return aircraft_table[static_cast<int>(type)].speed;
}

sf::FloatRect Aircraft::get_bounding_rect() const
{
    return get_world_transform().transformRect(sprite.getGlobalBounds());
}

void Aircraft::fire()
{
    //only aircrafts with fire_interval != 0 can fire
    if(aircraft_table[static_cast<int>(type)].fire_interval != sf::Time::Zero) {
        is_firing = true;
    }
}

void Aircraft::launch_missile()
{
    if(missile_ammo > 0) {
        is_lauching_missile = true;
        --missile_ammo;
    }
}

void Aircraft::increase_fire_rate()
{
    if(fire_rate_level < 5) {
        ++fire_rate_level;
    }
}

void Aircraft::increase_spread_level()
{
    if(spread_level < 3) {
        ++spread_level;
    }
}

void Aircraft::refill_missiles(int rrefill)
{
    missile_ammo += rrefill;
}

unsigned int Aircraft::get_category() const
{
    switch(type) {
    case Type::eagle:
        return entity_categories::player_aircraft;
    default:
        return entity_categories::enemy_aircraft;
    }
}

void Aircraft::update_movement_patter(sf::Time ffixed_timestep)
{
    if(!directions.empty()) {
        if(travelled_distance > directions[directions_index].distance) {
            directions_index = (directions_index == directions.size() - 1) ? 0 : directions_index + 1;
            travelled_distance = 0.0f;
        }
        float radian {to_radian(directions[directions_index].angle + 90.0f)};
        float vx {get_max_speed() * std::cos(radian)};
        float vy {get_max_speed() * std::sin(radian)};
        set_velocity(vx, vy);
        travelled_distance += get_max_speed() * ffixed_timestep.asSeconds();
    }
}

void Aircraft::update_texts()
{
    health_display->set_string(std::to_string(get_health_points()) + "HP");
    health_display->setRotation(-getRotation());
    if(missile_display) {
        if(missile_ammo == 0) {
            missile_display->set_string("NULL");
        }
        else {
            missile_display->set_string("M:" + std::to_string(missile_ammo));
        }
    }
}

void Aircraft::update_roll_animation()
{
    if(aircraft_table[static_cast<int>(type)].support_roll) {
        sf::IntRect texture_rect {aircraft_table[static_cast<int>(type)].texture_rect}; //by copy, if neither of the if are true, the original texture is used
        if(get_velocity().x < 0.0f) {
            texture_rect.left += texture_rect.width;
        }
        else if(get_velocity().x > 0.0f) {
            texture_rect.left += texture_rect.width * 2;
        }
        sprite.setTextureRect(texture_rect);
    }
}

void Aircraft::check_projectile_launch(sf::Time ffixed_timestep, Command_queue& ccommand_queue)
{
    //enemies fires all the time
    if(!is_player()) { //"if it isn't the player's aircraft"
        fire();
    }
    if(is_firing && fire_countdown <= sf::Time::Zero) {
        ccommand_queue.push(bullet_command);
        Id_sound id = is_player() ? Id_sound::Allied_gunfire : Id_sound::enemy_gunfire;
        get_dj().play(id, get_world_position());
        fire_countdown += aircraft_table[static_cast<int>(type)].fire_interval / (fire_rate_level + 1.0f);
        is_firing = false;
    }
    if(fire_countdown > sf::Time::Zero) {
        fire_countdown -= ffixed_timestep;
    }
    if(is_lauching_missile) {
        ccommand_queue.push(projectile_command);
        get_dj().play(Id_sound::launch_missile, get_world_position());
        is_lauching_missile = false;
    }
}

void Aircraft::check_pickup_drop(Command_queue& ccommand_queue)
{
    if(!is_player() && random_int(3) == 0 && !spawned_pickup) {
        ccommand_queue.push(pickup_command);
    }
    spawned_pickup = true;
}

void Aircraft::create_bullets(Scene_node& nnode, const Texture_holder& ttextures) const
{
    Projectile::Type type = (is_player()) ? Projectile::Type::allied_bullet : Projectile::Type::enemy_bullet;
    switch(spread_level) {
    case 1:
        create_projectile(nnode, type, ttextures, 0.0f, 0.5f);
        break;
    case 2:
        create_projectile(nnode, type, ttextures, -0.33f, 0.33f);
        create_projectile(nnode, type, ttextures, 0.33f, 0.33f);
        break;
    case 3:
        create_projectile(nnode, type, ttextures, -0.5f, 0.33f);
        create_projectile(nnode, type, ttextures, 0.0f, 0.5f);
        create_projectile(nnode, type, ttextures, 0.5f, 0.33f);
        break;
    }
}

void Aircraft::create_projectile(Scene_node& nnode, Projectile::Type ttype, const Texture_holder& ttextures, float xx_offset, float yy_offset) const
{
    std::unique_ptr<Projectile> projectile {std::make_unique<Projectile>(ttype, ttextures, get_dj())};
    sf::Vector2f offset {sprite.getGlobalBounds().width * xx_offset, sprite.getGlobalBounds().height * yy_offset};
    float orientation = (is_player()) ? -1.0f : 1.0f;
    projectile->setPosition(get_world_position() + offset * orientation);
    sf::Vector2f velocity {0.0f, projectile->get_maxspeed()};
    projectile->set_velocity(velocity * orientation);
    nnode.attach_child(std::move(projectile));
}

void Aircraft::create_pickup(Scene_node& nnode, const Texture_holder& ttextures) const
{
    Pickup::Type random_type = static_cast<Pickup::Type>(random_int(static_cast<int>(Pickup::Type::typecount)));
    std::unique_ptr<Pickup> pickup {std::make_unique<Pickup>(random_type, ttextures, get_dj())};
    pickup->setPosition(get_world_position());
    pickup->set_velocity(0.0f, 0.2f);
    nnode.attach_child(std::move(pickup));
}

void Aircraft::update_current(sf::Time ffixed_timestep, Command_queue& ccommand_queue)
{
    if(is_destroyed()) {
        check_pickup_drop(ccommand_queue);
        if(!show_explosion) {
            removal_marker = true;
            return;
        }
        explosion.update(ffixed_timestep);
        if (!played_explosion_sound) {
            Id_sound id = (random_int(2) == 0) ? Id_sound::explosion1 : Id_sound::explosion2;
            get_dj().play(id, get_world_position());
            played_explosion_sound = true;
        }
        if(explosion.is_finished()) {
            removal_marker = true;
            return;
        }
        return;
    }
    check_projectile_launch(ffixed_timestep, ccommand_queue);
    update_movement_patter(ffixed_timestep);
    Entity::update_current(ffixed_timestep, ccommand_queue);
    update_texts();
    update_roll_animation();
}

void Aircraft::draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
{
    if(is_destroyed() && show_explosion) {
        ttarget.draw(explosion, sstates);
    }
    else {
        ttarget.draw(sprite, sstates);
    }
}
