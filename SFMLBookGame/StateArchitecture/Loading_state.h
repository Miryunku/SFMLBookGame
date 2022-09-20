#pragma once

#include "State.h"
#include "Parallel_task.h"

class Loading_state : public State {
public:
    Loading_state(State_stack& sstack, Context ccontext);

    void set_completion(float ppercent);

    bool handle_event(const sf::Event& eevent) override;
    bool update(sf::Time ffixed_timestep) override;
    void draw() override;
private:
    sf::Text loading_text;
    sf::RectangleShape progressbar_background;
    sf::RectangleShape progressbar;
    Parallel_task loading_task;
};
