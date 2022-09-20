#pragma once

class Scene_node;

struct Command {
    Command();

    std::function<void(Scene_node&, sf::Time)> action;
    unsigned int category;
};
