#pragma once

#include "State.h"
#include "../ProcessArchitecture/World.h"
#include "../ProcessArchitecture/Input_player.h"

class Game_state : public State {
public:
    Game_state(State_stack& sstack, Context ccontext);

    bool handle_event(const sf::Event& eevent) override;
    bool update(sf::Time ffixed_timestep) override;
    void draw() override;
private:
    World world;
    Input_player& input_player;
};
