#pragma once

#include "State.h"
#include "../GUIArchitecture/Container.h"

class Menu_state : public State {
public:
    Menu_state(State_stack& sstack, Context ccontext);

    bool handle_event(const sf::Event& eevent) override;
    bool update(sf::Time ffixed_timestep) override;
    void draw() override;
private:
    sf::Sprite background_sprite;
    gui::Container gui_container;
};
