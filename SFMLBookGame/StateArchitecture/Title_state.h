#pragma once

#include "State.h"

class Title_state : public State {
public:
    Title_state(State_stack& sstack, Context ccontext);

    bool handle_event(const sf::Event& eevent) override;
    bool update(sf::Time ffixed_timestep) override;
    void draw() override;
private:
    sf::Sprite background_sprite;
    sf::Text text;
    bool show_text {true};
    sf::Time text_effect_time {sf::Time::Zero};
};
