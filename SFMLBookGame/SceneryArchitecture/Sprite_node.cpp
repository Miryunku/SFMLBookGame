#include "../pch.h"
#include "Sprite_node.h"

Sprite_node::Sprite_node(const sf::Texture& ttexture)
    : sprite {ttexture}
{
    //nothing by default
}

Sprite_node::Sprite_node(const sf::Texture& ttexture, const sf::IntRect& rrect)
    : sprite {ttexture, rrect}
{
    //nothing by default
}

void Sprite_node::draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
{
    ttarget.draw(sprite, sstates);
}
