#pragma once

#include "../IDs/Id_resources.h"

struct Projectile_data {
    int damage_points;
    float speed;
    Id_textures texture_id;
    sf::IntRect texture_rect;
};
