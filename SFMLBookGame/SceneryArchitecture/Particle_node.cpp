#include "../pch.h"
#include "Particle_node.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"

std::vector<Particle_data> tables::initialize_particle_table()
{
    std::vector<Particle_data> table(static_cast<int>(Particle::Type::particle_count));
    table[static_cast<int>(Particle::Type::propelland)].color = sf::Color {255, 255, 50};
    table[static_cast<int>(Particle::Type::propelland)].lifetime = sf::seconds(0.6f);

    table[static_cast<int>(Particle::Type::smoke)].color = sf::Color {50, 50, 50};
    table[static_cast<int>(Particle::Type::smoke)].lifetime = sf::seconds(4.0f);

    return table;
}

namespace {
    std::vector<Particle_data> particle_table {tables::initialize_particle_table()};
}

Particle_node::Particle_node(Particle::Type ttype, const Texture_holder& ttextures)
    : type {ttype},
    texture {ttextures.get_resource(Id_textures::particle)}
{
    //nothing by default
}

void Particle_node::add_particle(sf::Vector2f pposition)
{
    Particle particle;
    particle.position = pposition;
    particle.color = particle_table[static_cast<int>(type)].color;
    particle.lifetime = particle_table[static_cast<int>(type)].lifetime;
    particles.push_back(particle);
}

Particle::Type Particle_node::particle_type() const
{
    return type;
}

unsigned int Particle_node::get_category() const
{
    return entity_categories::particle_system;
}

void Particle_node::add_vertex(float wworld_x, float wworld_y, float ttexture_x, float ttexture_y, const sf::Color& ccolor) const
{
    sf::Vertex vertex {sf::Vector2f {wworld_x, wworld_y}, ccolor, sf::Vector2f {ttexture_x, ttexture_y}};
    vertex_array.append(vertex);
}

void Particle_node::compute_vertices() const
{
    sf::Vector2f size {texture.getSize()};
    sf::Vector2f half {size / 2.0f};
    //clear, then refill vertex array
    vertex_array.clear();
    for(const Particle& particle : particles) {
        sf::Color color {particle.color};
        float ratio {particle.lifetime.asSeconds() / particle_table[static_cast<int>(type)].lifetime.asSeconds()};
        color.a = static_cast<sf::Uint8>(255 * std::max(ratio, 0.0f));
        sf::Vector2f pos {particle.position};

        add_vertex(pos.x - half.x, pos.y - half.y, 0.0f, 0.0f, color);
        add_vertex(pos.x + half.x, pos.y - half.y, size.x, 0.0f, color);
        add_vertex(pos.x + half.x, pos.y + half.y, size.x, size.y, color);
        add_vertex(pos.x - half.x, pos.y + half.y, 0.0f, size.y, color);
    }
}

void Particle_node::update_current(sf::Time ffixed_timestep, Command_queue& ccomand_queue)
{
    while(!particles.empty() && particles.front().lifetime <= sf::Time::Zero) {
        particles.pop_front();
    }
    for(Particle& particle : particles) {
        particle.lifetime -= ffixed_timestep;
    }
    needs_vertex_update = true;
}

void Particle_node::draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
{
    if(needs_vertex_update) {
        compute_vertices();
        needs_vertex_update = false;
    }
    //apply particle texture
    sstates.texture = &texture;
    ttarget.draw(vertex_array, sstates);
}
