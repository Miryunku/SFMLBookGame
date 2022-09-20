#include "../pch.h"
#include "Post_effect.h"

Post_effect::~Post_effect()
{
    //empty by default
}

void Post_effect::apply_shader(const sf::Shader& sshader, sf::RenderTarget& ttarget)
{
    sf::Vector2f target_size {ttarget.getSize()};
    sf::VertexArray vertices {sf::TriangleStrip, 4};
    vertices[0] = sf::Vertex {sf::Vector2f {0.0f, 0.0f}, sf::Vector2f {0.0f, 1.0f}};
    vertices[1] = sf::Vertex {sf::Vector2f {target_size.x, 0.0f}, sf::Vector2f {1.0f, 1.0f}};
    vertices[2] = sf::Vertex {sf::Vector2f {0.0f, target_size.y}, sf::Vector2f {0.0f, 0.0f}};
    vertices[3] = sf::Vertex {sf::Vector2f {target_size}, sf::Vector2f {1.0f, 0.0f}};

    sf::RenderStates states;
    states.shader = &sshader;
    states.blendMode = sf::BlendNone;

    ttarget.draw(vertices, states);
}
