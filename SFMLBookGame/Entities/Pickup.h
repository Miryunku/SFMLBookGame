#pragma once

#include "Entity.h"
#include "../IDs/Id_resources.h"
#include "../DataTables/Pickup_data.h"

class Aircraft;

class Pickup : public Entity {
public:
    enum class Type {
        health_refill, missile_refill, fire_spread, fire_rate, typecount
    };

    Pickup(Type ttype, const Texture_holder& ttextures, Sound_player& ssound_player);

    void apply(Aircraft& aaircraft);

    unsigned int get_category() const override;
    sf::FloatRect get_bounding_rect() const override;
    bool is_removal() const override;
private:
    void draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const override;

    Type type;
    sf::Sprite sprite;
};

namespace tables {
    std::vector<Pickup_data> initialize_pickup_table();
}
