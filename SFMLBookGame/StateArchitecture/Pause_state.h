#pragma once

#include "State.h"
#include "../GUIArchitecture/Container.h"

class Pause_state : public State {
public:
    Pause_state(State_stack& sstack, Context ccontext);
    ~Pause_state();

    bool handle_event(const sf::Event& eevent) override;
    bool update(sf::Time ffixed_timestep) override;
    void draw() override;
private:
    sf::Text paused_text;
    gui::Container gui_container;
};
