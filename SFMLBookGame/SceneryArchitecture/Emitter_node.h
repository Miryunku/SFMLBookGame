#pragma once

#include "Scene_node.h"
#include "Particle.h"

class Particle_node;

class Emitter_node : public Scene_node {
public:
    explicit Emitter_node(Particle::Type ttype);
private:
    void emit_particle(sf::Time ffixed_timestep);

    void update_current(sf::Time ffixed_timestep, Command_queue& ccommand_queue) override;

    sf::Time accumulated_time;
    Particle::Type type;
    Particle_node* particle_system {nullptr};
};
