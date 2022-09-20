#pragma once

#include "State.h"
#include "../ProcessArchitecture/Input_player.h"
#include "../GUIArchitecture/Container.h"
#include "../GUIArchitecture/Button.h"
#include "../GUIArchitecture/Label.h"

class Settings_state : public State {
public:
    Settings_state(State_stack& sstack, Context ccontext);

    bool handle_event(const sf::Event& eevent) override;
    bool update(sf::Time ffixed_timestep) override;
    void draw() override;
private:
    void add_button_and_label(Input_player::Action aaction, float yy, const std::string& ttext, Context ccontext);
    void update_labels();

    sf::Sprite background_sprite;
    gui::Container gui_container;
    std::array<std::shared_ptr<gui::Button>, static_cast<int>(Input_player::Action::action_count)> buttons_binding;
    std::array<std::shared_ptr<gui::Label>, static_cast<int>(Input_player::Action::action_count)> labels_binding;
};
