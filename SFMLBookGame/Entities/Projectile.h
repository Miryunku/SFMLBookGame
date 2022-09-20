#pragma once

#include "Entity.h"
#include "../DataTables/Projectile_data.h"

class Projectile : public Entity {
public:
    enum class Type {
        allied_bullet, enemy_bullet, missile, typecount
    };

    Projectile(Type ttype, const Texture_holder& ttextures, Sound_player& ssound_player);

    bool is_guided() const;
    void guide_towards(sf::Vector2f pposition);
    float get_maxspeed() const;
    int get_damage_points() const;

    unsigned int get_category() const override;
    sf::FloatRect get_bounding_rect() const override;
    bool is_removal() const override;
private:
    void update_current(sf::Time ffixed_timestep, Command_queue& ccomand_queue) override;
    void draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const override;

    Type type;
    sf::Sprite sprite;
    sf::Vector2f target_direction;
};

namespace tables {
    std::vector<Projectile_data> initialize_projectile_table();
}
