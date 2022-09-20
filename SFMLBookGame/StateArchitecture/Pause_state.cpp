#include "../pch.h"
#include "Pause_state.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"
#include "../Others/Utility.h"
#include "../GUIArchitecture/Button.h"
#include "../MusicSoundProcess/Music_player.h"

Pause_state::Pause_state(State_stack& sstack, Context ccontext)
    : State {sstack, ccontext}
{
    sf::Font& font {ccontext.fonts->get_resource(Id_fonts::main)};
    paused_text.setFont(font);
    paused_text.setString("Game Paused");
    paused_text.setCharacterSize(70);
    center_origin(paused_text);
    sf::Vector2f view_size {ccontext.window->getView().getSize()};
    paused_text.setPosition(0.5f * view_size.x, 0.4f * view_size.y);

    std::shared_ptr<gui::Button> continue_button {std::make_shared<gui::Button>(ccontext)};
    continue_button->setPosition(0.5f * view_size.x - 100.0f, 0.4f * view_size.y + 75.0f);
    continue_button->set_text("Continue");
    continue_button->set_callback([this]()
        {
            request_stack_pop();
        });

    std::shared_ptr<gui::Button> to_menu_button {std::make_shared<gui::Button>(ccontext)};
    to_menu_button->setPosition(0.5f * view_size.x - 100.0f, 0.4f * view_size.y + 125.0f);
    to_menu_button->set_text("Back to menu");
    to_menu_button->set_callback([this]()
        {
            request_state_clear();
            request_stack_push(Id_states::menu);
        });

    gui_container.pack(continue_button);
    gui_container.pack(to_menu_button);

    get_context().music_player->set_paused(true);
}

Pause_state::~Pause_state()
{
    get_context().music_player->set_paused(false);
}

bool Pause_state::handle_event(const sf::Event& eevent)
{
    gui_container.handle_event(eevent);
    return false;
}

bool Pause_state::update(sf::Time ffixed_timestep)
{
    return false;
}

void Pause_state::draw()
{
    sf::RenderWindow& window {*get_context().window};
    window.setView(window.getDefaultView());

    sf::RectangleShape background_shape;
    background_shape.setFillColor(sf::Color::Transparent);
    background_shape.setSize(window.getView().getSize());

    window.draw(background_shape);
    window.draw(paused_text);
    window.draw(gui_container);
}
