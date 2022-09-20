#include "../pch.h"
#include "Gameover_state.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"
#include "../ProcessArchitecture/Input_player.h"
#include "../Others/Utility.h"

Gameover_state::Gameover_state(State_stack& sstack, Context ccontext)
    : State {sstack, ccontext}
{
    text.setFont(ccontext.fonts->get_resource(Id_fonts::main));
    if(ccontext.input_player->get_mission_status() == Input_player::Mission_status::mission_failure) {
        text.setString("Mission failure");
    }
    if(ccontext.input_player->get_mission_status() == Input_player::Mission_status::mission_success) {
        text.setString("Mission success");
    }
    text.setCharacterSize(70);
    center_origin(text);
    sf::Vector2f window_size {ccontext.window->getSize()};
    text.setPosition(window_size.x * 0.5f, window_size.y * 0.4f);
}

bool Gameover_state::handle_event(const sf::Event& eevent)
{
    return false;
}

bool Gameover_state::update(sf::Time ffixed_timestep)
{
    elapsed_time += ffixed_timestep;
    if(elapsed_time > sf::seconds(3.0f)) {
        request_state_clear();
        request_stack_push(Id_states::menu);
    }
    return false;
}

void Gameover_state::draw()
{
    sf::RenderWindow& window {*get_context().window};
    window.setView(window.getDefaultView());
    //create dark, semi-transparent background
    sf::RectangleShape rectangle;
    rectangle.setFillColor(sf::Color(0, 0, 0, 150));
    rectangle.setSize(window.getView().getSize());
    window.draw(rectangle);
    window.draw(text);
}
