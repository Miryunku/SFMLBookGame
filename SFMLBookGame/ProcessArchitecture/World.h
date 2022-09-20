#pragma once

#include "TPT Resource_holder.h"
#include "../SceneryArchitecture/Scene_node.h"
#include "../Entities/Aircraft.h"
#include "Command_queue.h"
#include "../IDs/Categories.h"
#include "../Beautification/Bloom_effect.h"

class World : private sf::NonCopyable {
public:
    explicit World(sf::RenderWindow& wwindow, Font_holder& ffonts, Sound_player& ssound_player);

    void update(sf::Time ffixed_timestep);
    void draw();
    //= = = = =
    Command_queue& get_command_queue();
    //= = = = = 
    bool is_player_alive() const;
    bool has_player_reached_end() const;
private:
    enum class Layers {
        background, lower_air, upper_air, layercount
    };

    struct Spawnpoint {
        Spawnpoint(Aircraft::Type ttype, float xx, float yy)
            : type {ttype},
            x {xx},
            y {yy}
        {
            //empty by default
        }

        Aircraft::Type type;
        float x;
        float y;
    };

    void load_textures();
    void build_scene();
    //= = = = =
    void adapt_player_position();
    void adapt_player_velocity();
    //= = = = =
    sf::FloatRect get_viewbounds();
    sf::FloatRect get_battlefield_bounds();
    //= = = = =
    void add_enemy(Aircraft::Type ttype, float xx, float yy);
    void add_enemies();
    void spaw_enemies();
    //= = = = =
    void guide_missiles();
    void handle_collisions();
    void destroy_outside_entities();
    void update_sounds();

    sf::RenderWindow& main_window;
    sf::RenderTexture scene_texture;
    sf::View world_view;
    //= = = = =
    Texture_holder world_textures;
    Font_holder& fonts;
    Sound_player& sound_player;
    //= = = = =
    Scene_node scene_graph;
    std::array<Scene_node*, static_cast<int>(Layers::layercount)> scene_layers;
    Command_queue command_queue;
    //= = = = =
    sf::FloatRect world_bounds;
    sf::Vector2f spawn_position;
    Aircraft* player_aircraft {nullptr};
    float scroll_speed {-50.0f};
    std::vector<Spawnpoint> enemy_spawnpoints;
    std::vector<Aircraft*> active_enemies;
    Bloom_effect bloom_effect;
};

bool matches_categories(std::pair<Scene_node*, Scene_node*>& ccolliders, entity_categories::Categories ccategory1, entity_categories::Categories ccategory2);
