#pragma once

#include "State.h"

class Gameover_state : public State {
public:
    Gameover_state(State_stack& sstack, Context ccontext);

    bool handle_event(const sf::Event& eevent) override;
    bool update(sf::Time ffixed_timestep) override;
    void draw() override;
private:
    sf::Text text;
    sf::Time elapsed_time;
};
