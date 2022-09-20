#pragma once

struct Particle {
    enum class Type {
        propelland, smoke, particle_count
    };

    sf::Vector2f position;
    sf::Color color;
    sf::Time lifetime;
};
