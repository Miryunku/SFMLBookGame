#pragma once

#include "../IDs/Id_resources.h"
#include "Direction.h"

struct Aircraft_data {
    int health_points;
    float speed;
    bool support_roll;
    Id_textures texture_id;
    sf::IntRect texture_rect;
    sf::Time fire_interval;
    std::vector<Direction> directions;
};
