#pragma once

#include "Entity.h"
#include "../IDs/Id_resources.h"
#include "../SceneryArchitecture/Text_node.h"
#include "../DataTables/Aircraft_data.h"
#include "../ProcessArchitecture/Command_queue.h"
#include "Projectile.h"
#include "../Beautification/Animation.h"

class Aircraft : public Entity {
public:
    enum class Type {
        avenger, eagle, raptor, type_count
    };

    Aircraft(Type ttype, const Texture_holder& ttextures, const Font_holder& ffonts, Sound_player& ssound_player);

    bool is_player() const;
    float get_max_speed() const;
    //= = = = =
    void fire();
    void launch_missile();
    //= = = =
    void increase_fire_rate();
    void increase_spread_level();
    void refill_missiles(int rrefill);

    unsigned int get_category() const override;
    sf::FloatRect get_bounding_rect() const override;
    bool is_removal() const override;
    //= = = = =
    void remove() override;
private:
    void check_projectile_launch(sf::Time ffixed_timestep, Command_queue& ccommand_queue);
    void check_pickup_drop(Command_queue& ccommand_queue);
    //= = = = =
    void create_bullets(Scene_node& nnode, const Texture_holder& ttextures) const;
    void create_projectile(Scene_node& nnode, Projectile::Type ttype, const Texture_holder& ttextures, float xx_offset, float yy_offset) const;
    void create_pickup(Scene_node& nnode, const Texture_holder& ttextures) const;
    //= = = = =
    void update_movement_patter(sf::Time ffixed_timestep);
    void update_texts();
    void update_roll_animation();

    void update_current(sf::Time ffixed_timestep, Command_queue& ccommand_queue) override;
    void draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const override;

    Type type;
    sf::Sprite sprite;
    //= = = = = =
    const std::vector<Direction>& directions;
    float travelled_distance {0.0f};
    int directions_index {0};
    //= = = = = =
    Text_node* health_display {nullptr};
    Text_node* missile_display {nullptr};
    //= = = = = =
    bool is_firing {false};
    bool is_lauching_missile {false};
    bool removal_marker {false};
    bool show_explosion{ true };
    bool spawned_pickup{ false };
    bool played_explosion_sound{ false };
    //= = = = = =
    sf::Time fire_countdown;
    float fire_rate_level {1.0f};
    int spread_level {1};
    int missile_ammo {2};
    //= = = = = =
    Command bullet_command;
    Command projectile_command;
    Command pickup_command;
    //= = = = = =
    Animation explosion;
};

namespace tables {
    std::vector<Aircraft_data> initialize_aircraft_table();
}
