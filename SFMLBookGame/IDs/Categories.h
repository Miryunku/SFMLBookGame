#pragma once

namespace entity_categories {
    enum Categories {
        none = 0,
        scene = 1 << 0,
        player_aircraft = 1 << 1,
        allied_aircraft = 1 << 2,
        enemy_aircraft = 1 << 3,
        pickup = 1 << 4,
        allied_projectile = 1 << 5,
        enemy_projectile = 1 << 6,
        particle_system = 1 << 7,

        aircraft = player_aircraft | allied_aircraft | enemy_aircraft,
        projectile = allied_projectile | enemy_projectile
    };
}
