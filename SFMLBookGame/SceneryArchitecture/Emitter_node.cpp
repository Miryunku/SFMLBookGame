#include "../pch.h"
#include "Emitter_node.h"
#include "Particle_node.h"
#include "../ProcessArchitecture/Command_queue.h"

Emitter_node::Emitter_node(Particle::Type ttype)
    : type {ttype}
{
    //empty by default
}

void Emitter_node::emit_particle(sf::Time ffixed_timestep)
{
    const float emission_rate {30.0f};
    const sf::Time interval {sf::seconds(1.0f) / emission_rate};
    accumulated_time += ffixed_timestep;
    while(accumulated_time > interval) {
        particle_system->add_particle(get_world_position());
        accumulated_time -= interval;
    }
}

void Emitter_node::update_current(sf::Time ffixed_timestep, Command_queue& ccommand_queue)
{
    if(particle_system) {
        emit_particle(ffixed_timestep);
    }
    else {
        //find particle node with the same type as emitter node
        Command find;
        find.category = entity_categories::particle_system;
        find.action = [this](Scene_node& nnode, sf::Time ffixed_timestep)
        {
            Particle_node& particle_node {static_cast<Particle_node&>(nnode)};
            if(particle_node.particle_type() == type) {
                particle_system = &particle_node;
            }
        };
        ccommand_queue.push(find);
    }
}
