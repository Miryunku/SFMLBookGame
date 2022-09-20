#include "../pch.h"
#include "Settings_state.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"

Settings_state::Settings_state(State_stack& sstack, Context ccontext)
    : State(sstack, ccontext)
{
    background_sprite.setTexture(ccontext.textures->get_resource(Id_textures::title_screen));
    //build key binding buttons and labels (the order must be according to Input_player enum class "Action")
    add_button_and_label(Input_player::Action::moveup, 150.0f, "Move up", ccontext);
    add_button_and_label(Input_player::Action::movedown, 200.0f, "Move down", ccontext);
    add_button_and_label(Input_player::Action::moveleft, 250.0f, "Move left", ccontext);
    add_button_and_label(Input_player::Action::moveright, 300.0f, "Move right", ccontext);
    add_button_and_label(Input_player::Action::fire_bullet, 350.0f, "Fire bullet", ccontext);
    add_button_and_label(Input_player::Action::fire_missile, 400.0f, "Fire missile", ccontext);

    update_labels();

    std::shared_ptr<gui::Button> back_button {std::make_shared<gui::Button>(ccontext)};
    back_button->setPosition(400.0f, 400.0f);
    back_button->set_text("Back");
    back_button->set_callback([this]()
        {
            request_stack_pop();
        });

    gui_container.pack(back_button);
}

bool Settings_state::handle_event(const sf::Event& eevent)
{
    bool is_key_binding {false};
    for(size_t i {0}; i < buttons_binding.size(); ++i) {
        if(buttons_binding[i]->is_active()) {
            is_key_binding = true;
            if(eevent.type == sf::Event::KeyReleased) {
                get_context().input_player->assign_key(eevent.key.code, static_cast<Input_player::Action>(i));
                buttons_binding[i]->deactivate();
            }
            break;
        }
    }
    if(is_key_binding) {
        update_labels();
    }
    else {
        gui_container.handle_event(eevent);
    }
    return false;
}

bool Settings_state::update(sf::Time ffixed_timestep)
{
    return false;
}

void Settings_state::draw()
{
    sf::RenderWindow& window {*get_context().window};
    window.draw(background_sprite);
    window.draw(gui_container);
}

void Settings_state::add_button_and_label(Input_player::Action aaction, float yy, const std::string& ttext, Context ccontext)
{
    buttons_binding[static_cast<int>(aaction)] = std::make_shared<gui::Button>(ccontext);
    buttons_binding[static_cast<int>(aaction)]->setPosition(80.0f, yy);
    buttons_binding[static_cast<int>(aaction)]->set_text(ttext);
    buttons_binding[static_cast<int>(aaction)]->set_toggle(true);

    labels_binding[static_cast<int>(aaction)] = std::make_shared<gui::Label>("", *ccontext.fonts);
    labels_binding[static_cast<int>(aaction)]->setPosition(300.0f, yy + 15.0f);

    gui_container.pack(buttons_binding[static_cast<int>(aaction)]);
    gui_container.pack(labels_binding[static_cast<int>(aaction)]);
}

void Settings_state::update_labels()
{
    Input_player& input_player {*get_context().input_player};
    for(size_t i {0}; i < labels_binding.size(); ++i) {
        sf::Keyboard::Key key {input_player.get_assigned_key(static_cast<Input_player::Action>(i))};
        labels_binding[i]->set_text(std::to_string(key));
    }
}
