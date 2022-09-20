#pragma once

#include "Scene_node.h"
#include "Particle.h"
#include "../DataTables/Particle_data.h"
#include "../IDs/Id_resources.h"

class Particle_node : public Scene_node { //Particle_node acts as the particle system
public:
    Particle_node(Particle::Type ttype, const Texture_holder& ttextures);

    void add_particle(sf::Vector2f pposition);
    Particle::Type particle_type() const;

    unsigned int get_category() const override;
private:
    void add_vertex(float wworld_x, float wworld_y, float ttexture_x, float ttexture_y, const sf::Color& ccolor) const;
    void compute_vertices() const;

    void update_current(sf::Time ffixed_timestep, Command_queue& ccomand_queue) override;
    void draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const override;

    Particle::Type type;
    std::deque<Particle> particles;
    const sf::Texture& texture;
    mutable sf::VertexArray vertex_array {sf::Quads};
    mutable bool needs_vertex_update {true};
};

namespace tables {
    std::vector<Particle_data> initialize_particle_table();
}
