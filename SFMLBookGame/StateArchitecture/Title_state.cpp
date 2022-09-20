#include "../pch.h"
#include "Title_state.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"
#include "../Others/Utility.h"

Title_state::Title_state(State_stack& sstack, Context ccontext)
    : State {sstack, ccontext}
{
    background_sprite.setTexture(ccontext.textures->get_resource(Id_textures::title_screen));
    text.setFont(ccontext.fonts->get_resource(Id_fonts::main));
    text.setString("Press any key to start");
    center_origin(text);
    text.setPosition(320.0f, 300.0f);
}

bool Title_state::handle_event(const sf::Event& eevent)
{
    if(eevent.type == sf::Event::KeyReleased) {
        request_stack_pop();
        request_stack_push(Id_states::menu);
    }
    return false;
}

bool Title_state::update(sf::Time ffixed_timestep)
{
    text_effect_time += ffixed_timestep;
    if(text_effect_time >= sf::seconds(0.5f)) {
        show_text = !show_text;
        text_effect_time = sf::Time::Zero;
    }
    return false;
}

void Title_state::draw()
{
    sf::RenderWindow& window {*get_context().window};
    window.draw(background_sprite);
    if(show_text) {
        window.draw(text);
    }
}
