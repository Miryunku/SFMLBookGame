#pragma once

#include "Scene_node.h"

class Sprite_node : public Scene_node {
public:
    explicit Sprite_node(const sf::Texture& ttexture);
    Sprite_node(const sf::Texture& ttexture, const sf::IntRect& rrect);
private:
    void draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const override;

    sf::Sprite sprite;
};
