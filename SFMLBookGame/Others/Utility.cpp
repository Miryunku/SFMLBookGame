#include "../pch.h"
#include "Utility.h"
#include "../Beautification/Animation.h"

namespace {
    std::default_random_engine random_engine {static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())};
}

void center_origin(sf::Sprite& ssprite)
{
    sf::FloatRect bounds {ssprite.getLocalBounds()};
    ssprite.setOrigin(std::floor(bounds.left + bounds.width / 2.0f), std::floor(bounds.top + bounds.height / 2.0f));
}

void center_origin(sf::Text& ttext)
{
    sf::FloatRect bounds {ttext.getLocalBounds()};
    ttext.setOrigin(std::floor(bounds.left + bounds.width / 2.0f), std::floor(bounds.top + bounds.height / 2.0f));
}

void center_origin(Animation& aanimation)
{
    sf::FloatRect bounds {aanimation.get_local_bounds()};
    aanimation.setOrigin(std::floor(bounds.left + bounds.width / 2.0f), std::floor(bounds.top + bounds.height / 2.0f));
}

float to_radian(float ddegree)
{
    return ddegree * (180.0f / pi);
}

float to_degree(float rradian)
{
    return rradian * (pi / 180.0f);
}

float vector_length(sf::Vector2f vvector)
{
    return std::sqrt((vvector.x * vvector.x) + (vvector.y * vvector.y));
}

sf::Vector2f unit_vector(sf::Vector2f vvector)
{
    if(vvector.x == 0.0f && vvector.y == 0.0f) {
        throw std::logic_error {"Utility.h - unit_vector"};
    }
    return vvector / vector_length(vvector);
}

int random_int(int mmax)
{
    std::uniform_int_distribution<int> dist {0, mmax - 1};
    return dist(random_engine);
}
