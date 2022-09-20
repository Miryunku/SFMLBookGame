#include "../pch.h"
#include "Loading_state.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"
#include "../Others/Utility.h"

Loading_state::Loading_state(State_stack& sstack, Context ccontext)
    : State {sstack, ccontext}
{
    sf::Font& font {ccontext.fonts->get_resource(Id_fonts::main)};
    loading_text.setFont(font);
    loading_text.setString("Loading resources");
    center_origin(loading_text);
    sf::RenderWindow& window {*get_context().window};
    sf::Vector2f view_size {window.getView().getSize()};
    loading_text.setPosition(view_size.x / 2.0f, view_size.y / 2.0f + 50.0f);

    progressbar_background.setSize(sf::Vector2f {view_size.x - 20.0f, 10.0f});
    progressbar_background.setFillColor(sf::Color::White);
    progressbar_background.setPosition(10.0f, loading_text.getPosition().y - 40.0f);

    //progressbar.setSize(progressbar_background.getSize());
    progressbar.setFillColor(sf::Color::Green);
    progressbar.setPosition(progressbar_background.getPosition());

    set_completion(0.0f);
    loading_task.execute();
}

void Loading_state::set_completion(float ppercent)
{
    if(ppercent > 1.0f) { //clamp
        ppercent = 1.0f;
    }
    sf::Vector2f size {progressbar_background.getSize().x * ppercent, progressbar_background.getSize().y};
    progressbar.setSize(size);
}

bool Loading_state::handle_event(const sf::Event& eevent)
{
    return false;
}

bool Loading_state::update(sf::Time ffixed_timestep)
{
    if(loading_task.is_finished()) {
        request_stack_pop();
        request_stack_push(Id_states::game);
    }
    else {
        set_completion(loading_task.get_completion());
    }
    return false;
}

void Loading_state::draw()
{
    sf::RenderWindow& window {*get_context().window};
    window.setView(window.getDefaultView());
    window.draw(loading_text);
    window.draw(progressbar_background);
    window.draw(progressbar);
}
