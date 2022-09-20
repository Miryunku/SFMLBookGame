#pragma once

#include "../IDs/Id_resources.h"

class Aircraft;

struct Pickup_data {
    Id_textures texture_id;
    sf::IntRect texture_rect;
    std::function<void(Aircraft&)> action;
};
