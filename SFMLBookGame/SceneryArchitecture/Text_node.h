#pragma once

#include "Scene_node.h"
#include "../IDs/Id_resources.h"

class Text_node : public Scene_node {
public:
    Text_node(const Font_holder& ffonts);

    void set_string(const std::string& ttext);
private:
    void draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const override;

    sf::Text text;
};
