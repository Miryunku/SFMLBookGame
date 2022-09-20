#include "../pch.h"
#include "World.h"
#include "../SceneryArchitecture/Sprite_node.h"
#include "../IDs/Categories.h"
#include "../Entities/Pickup.h"
#include "../SceneryArchitecture/Particle_node.h"
#include "../MusicSoundProcess/Sound_player.h"

World::World(sf::RenderWindow& wwindow, Font_holder& ffonts, Sound_player& ssound_player)
    : main_window {wwindow},
    world_view {wwindow.getDefaultView()},
    fonts {ffonts},
    sound_player {ssound_player},
    world_bounds {0.0f, 0.0f, world_view.getSize().x, 5000.0f},
    spawn_position {world_bounds.width / 2.0f, world_bounds.height - world_view.getSize().y / 2.0f}
{
    scene_texture.create(main_window.getSize().x, main_window.getSize().y);
    load_textures();
    build_scene();
    world_view.setCenter(spawn_position);
}

void World::load_textures()
{
    world_textures.load(Id_textures::entities, "Entities.png");
    world_textures.load(Id_textures::jungle, "Jungle.png");
    world_textures.load(Id_textures::explosion, "Explosion.png");
    world_textures.load(Id_textures::particle, "Particle.png");
    world_textures.load(Id_textures::finish_line, "FinishLine.png");
}

void World::build_scene()
{
    //initialize the different layers
    for(int i {0}; i < scene_layers.size(); ++i) {
        entity_categories::Categories category = (i == static_cast<int>(Layers::lower_air)) ? entity_categories::scene : entity_categories::none;
        std::unique_ptr<Scene_node> layer {std::make_unique<Scene_node>(category)};
        scene_layers[i] = layer.get();
        scene_graph.attach_child(std::move(layer));
    }
    //add the background sprite to the scene
    sf::Texture& jungle_texture {world_textures.get_resource(Id_textures::jungle)};
    jungle_texture.setRepeated(true);
    sf::IntRect jungle_rect {world_bounds};
    jungle_rect.height += static_cast<int>(world_view.getSize().y);
    std::unique_ptr<Sprite_node> jungle_sprite {std::make_unique<Sprite_node>(jungle_texture, jungle_rect)};
    jungle_sprite->setPosition(world_bounds.left, world_bounds.top - world_view.getSize().y);
    scene_layers[static_cast<int>(Layers::background)]->attach_child(std::move(jungle_sprite));
    //add finish line to the scene
    sf::Texture& finish_texture {world_textures.get_resource(Id_textures::finish_line)};
    std::unique_ptr<Sprite_node> finish_sprite {std::make_unique<Sprite_node>(finish_texture)};
    finish_sprite->setPosition(0.0f, -76.0f);
    scene_layers[static_cast<int>(Layers::background)]->attach_child(std::move(finish_sprite));
    //add particle systems
    std::unique_ptr<Particle_node> smoke_system {std::make_unique<Particle_node>(Particle::Type::smoke, world_textures)};
    scene_layers[static_cast<int>(Layers::lower_air)]->attach_child(std::move(smoke_system));
    std::unique_ptr<Particle_node> fire_system {std::make_unique<Particle_node>(Particle::Type::propelland, world_textures)};
    scene_layers[static_cast<int>(Layers::lower_air)]->attach_child(std::move(fire_system));
    //add player's aircraft
    std::unique_ptr<Aircraft> player {std::make_unique<Aircraft>(Aircraft::Type::eagle, world_textures, fonts, sound_player)};
    player->setPosition(spawn_position);
    player_aircraft = player.get();
    scene_layers[static_cast<int>(Layers::upper_air)]->attach_child(std::move(player));

    add_enemies();
}

void World::update(sf::Time ffixed_timestep)
{
    //scroll the world and reset the player's velocity
    world_view.move(0.0f, scroll_speed * ffixed_timestep.asSeconds());
    player_aircraft->set_velocity(0.0f, 0.0f);

    destroy_outside_entities();
    guide_missiles();
    //forward commands to the scene_graphs
    while(!command_queue.is_empty()) {
        scene_graph.on_commmand(command_queue.pop(), ffixed_timestep);
    }
    adapt_player_velocity();
    handle_collisions();
    scene_graph.clean_removals();
    spaw_enemies();
    //regular update step and correct the player's position if it goes outside the view
    scene_graph.update(ffixed_timestep, command_queue);
    adapt_player_position();
    //update sounds
    update_sounds();
}

void World::draw()
{
    scene_texture.clear();
    scene_texture.setView(world_view);
    scene_texture.draw(scene_graph);
    scene_texture.display();
    bloom_effect.apply(scene_texture, main_window);
}

Command_queue& World::get_command_queue()
{
    return command_queue;
}

bool World::is_player_alive() const
{
    return !player_aircraft->is_removal();
}

bool World::has_player_reached_end() const
{
    return !world_bounds.contains(player_aircraft->getPosition());
}

void World::adapt_player_position()
{
    //keep player's position inside the screen bounds, at least border_distance units from the border
    sf::FloatRect view_bounds {world_view.getCenter() - world_view.getSize() / 2.0f, world_view.getSize()};
    sf::Vector2f position {player_aircraft->getPosition()};
    constexpr float border_distance {40.0f};

    position.x = std::max(position.x, view_bounds.left + border_distance);
    position.x = std::min(position.x, view_bounds.left + view_bounds.width - border_distance);
    position.y = std::max(position.y, view_bounds.top + border_distance);
    position.y = std::min(position.y, view_bounds.top + view_bounds.height - border_distance);
    player_aircraft->setPosition(position);
}

void World::adapt_player_velocity()
{
    sf::Vector2f velocity {player_aircraft->get_velocity()};
    //if moving diagonally, reduce velocity (to have always the same velocity)
    if(velocity.x != 0.0f && velocity.y != 0.0f) {
        player_aircraft->set_velocity(velocity / std::sqrt(2.0f));
    }
    player_aircraft->accelerate(0.0f, scroll_speed);
}

sf::FloatRect World::get_viewbounds()
{
    return sf::FloatRect {world_view.getCenter() - world_view.getSize() / 2.f, world_view.getSize()};
}

sf::FloatRect World::get_battlefield_bounds()
{
    sf::FloatRect battlefield_bounds {get_viewbounds()};
    battlefield_bounds.top -= 100.0f;
    battlefield_bounds.height += 100.0f;
    return battlefield_bounds;
}

void World::add_enemy(Aircraft::Type ttype, float xx, float yy)
{
    enemy_spawnpoints.emplace_back(ttype, spawn_position.x + xx, spawn_position.y - yy);
}

void World::add_enemies()
{
    add_enemy(Aircraft::Type::raptor, 0.0f, 500.0f);
    add_enemy(Aircraft::Type::raptor, 0.0f, 1000.0f);
    add_enemy(Aircraft::Type::raptor, 100.0f, 1150.0f);
    add_enemy(Aircraft::Type::raptor, -100.0f, 1150.0f);
    add_enemy(Aircraft::Type::avenger, 70.0f, 1500.0f);
    add_enemy(Aircraft::Type::avenger, -70.0f, 1500.0f);
    add_enemy(Aircraft::Type::avenger, -70.0f, 1710.0f);
    add_enemy(Aircraft::Type::avenger, 70.0f, 1700.0f);
    add_enemy(Aircraft::Type::avenger, 30.0f, 1850.0f);
    add_enemy(Aircraft::Type::raptor, 300.0f, 2200.0f);
    add_enemy(Aircraft::Type::raptor, -300.0f, 2200.0f);
    add_enemy(Aircraft::Type::raptor, 0.0f, 2200.0f);
    add_enemy(Aircraft::Type::raptor, 0.0f, 2500.0f);
    add_enemy(Aircraft::Type::avenger, -300.0f, 2700.0f);
    add_enemy(Aircraft::Type::avenger, -300.0f, 2700.0f);
    add_enemy(Aircraft::Type::raptor, 0.0f, 3000.0f);
    add_enemy(Aircraft::Type::raptor, 250.0f, 3250.0f);
    add_enemy(Aircraft::Type::raptor, -250.0f, 3250.0f);
    add_enemy(Aircraft::Type::avenger, 0.0f, 3500.0f);
    add_enemy(Aircraft::Type::avenger, 0.0f, 3700.0f);
    add_enemy(Aircraft::Type::raptor, 0.0f, 3800.0f);
    add_enemy(Aircraft::Type::avenger, 0.0f, 4000.0f);
    add_enemy(Aircraft::Type::avenger, -200.0f, 4200.0f);
    add_enemy(Aircraft::Type::raptor, 200.0f, 4200.0f);
    add_enemy(Aircraft::Type::raptor, 0.0f, 4400.0f);

    //sort all enemies according to their y value, such that lower enemies are checked first for spawning
    std::sort(enemy_spawnpoints.begin(), enemy_spawnpoints.end(), [](Spawnpoint lhs, Spawnpoint rhs)
        {
            return lhs.y < rhs.y;
        });
}

void World::spaw_enemies()
{
    while(!enemy_spawnpoints.empty() && enemy_spawnpoints.back().y > get_battlefield_bounds().top) {
        Spawnpoint spawnpoint {enemy_spawnpoints.back()};
        std::unique_ptr<Aircraft> enemy {std::make_unique<Aircraft>(spawnpoint.type, world_textures, fonts, sound_player)};
        enemy->setPosition(spawnpoint.x, spawnpoint.y);
        enemy->setRotation(180.0f);
        scene_layers[static_cast<int>(Layers::upper_air)]->attach_child(std::move(enemy));
        enemy_spawnpoints.pop_back();
    }
}

void World::guide_missiles()
{
    //command that stores all enemies in active_enemies data member
    Command collect_enemies;
    collect_enemies.category = entity_categories::enemy_aircraft;
    collect_enemies.action = [this](Scene_node& nnode, sf::Time ffixed_timestep)
    {
        Aircraft& aircraft {static_cast<Aircraft&>(nnode)};
        if(!aircraft.is_destroyed()) {
            active_enemies.push_back(&aircraft);
        }
    };
    command_queue.push(collect_enemies);

    //command that searches the closest enemy
    Command steer_missile;
    steer_missile.category = entity_categories::allied_projectile;
    steer_missile.action = [this](Scene_node& nnode, sf::Time ffixed_timestep)
    {
        Projectile& projectile {static_cast<Projectile&>(nnode)};
        if(!projectile.is_guided()) {
            return;
        }
        float min_distance {std::numeric_limits<float>::max()};
        Aircraft* closest_enemy {nullptr};
        for(Aircraft* aircraft : active_enemies) {
            float enemy_distance {distance(projectile, *aircraft)};
            if(enemy_distance < min_distance) {
                min_distance = enemy_distance;
                closest_enemy = aircraft;
            }
        }
        if(closest_enemy) {
            projectile.guide_towards(closest_enemy->get_world_position());
        }
    };
    command_queue.push(steer_missile);
    active_enemies.clear();
}

bool matches_categories(std::pair<Scene_node*, Scene_node*>& ccolliders, entity_categories::Categories ccategory1, entity_categories::Categories ccategory2)
{
    unsigned int ctg1 = ccolliders.first->get_category();
    unsigned int ctg2 = ccolliders.second->get_category();
    if(ccategory1 & ctg1 && ccategory2 & ctg2) {
        return true;
    }
    if(ccategory1 & ctg2 && ccategory2 & ctg1) {
        std::swap(ccolliders.first, ccolliders.second);
        return true;
    }
    return false;
}

void World::handle_collisions()
{
    std::set<std::pair<Scene_node*, Scene_node*>> collisions;
    scene_graph.check_scene_collision(scene_graph, collisions);
    for(std::pair<Scene_node*, Scene_node*> pair : collisions) {
        if(matches_categories(pair, entity_categories::player_aircraft, entity_categories::enemy_aircraft)) {
            Aircraft& player = static_cast<Aircraft&>(*pair.first);
            Aircraft& enemy = static_cast<Aircraft&>(*pair.second);
            player.damage(enemy.get_health_points());
            enemy.destroy();
        }
        if(matches_categories(pair, entity_categories::player_aircraft, entity_categories::enemy_projectile)) {
            Aircraft& player = static_cast<Aircraft&>(*pair.first);
            Projectile& enemy_bullet = static_cast<Projectile&>(*pair.second);
            player.damage(enemy_bullet.get_damage_points());
            enemy_bullet.destroy();
        }
        if(matches_categories(pair, entity_categories::enemy_aircraft, entity_categories::allied_projectile)) {
            Aircraft& enemy = static_cast<Aircraft&>(*pair.first);
            Projectile& player_bullet = static_cast<Projectile&>(*pair.second);
            enemy.damage(player_bullet.get_damage_points());
            player_bullet.destroy();
        }
        if(matches_categories(pair, entity_categories::player_aircraft, entity_categories::pickup)) {
            Aircraft& player = static_cast<Aircraft&>(*pair.first);
            Pickup& pickup = static_cast<Pickup&>(*pair.second);
            pickup.apply(player);
            pickup.destroy();
            player.get_dj().play(Id_sound::collect_pickup, player.get_world_position());
        }
    }
}

void World::destroy_outside_entities()
{
    Command command;
    command.category = entity_categories::enemy_aircraft | entity_categories::projectile | entity_categories::pickup;
    command.action = [this](Scene_node& nnode, sf::Time ffixed_timestep)
    {
        Entity& entity {static_cast<Entity&>(nnode)};
        if(!get_battlefield_bounds().intersects(entity.get_bounding_rect())) {
            entity.remove();
        }
    };
    command_queue.push(command);
}

void World::update_sounds()
{
    sound_player.set_listener_position(player_aircraft->get_world_position());
    sound_player.remove_stopped_sounds();
}
