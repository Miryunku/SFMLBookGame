#include "../pch.h"
#include "Menu_state.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"
#include "../Others/Utility.h"
#include "../GUIArchitecture/Button.h"
#include "../MusicSoundProcess/Music_player.h"

Menu_state::Menu_state(State_stack& sstack, Context ccontext)
    : State {sstack, ccontext}
{
    sf::Texture& texture {ccontext.textures->get_resource(Id_textures::title_screen)};
    background_sprite.setTexture(texture);

    std::shared_ptr<gui::Button> play_button {std::make_shared<gui::Button>(ccontext)};
    play_button->setPosition(100.0f, 250.0f);
    play_button->set_text("Play");
    play_button->set_callback([this]()
        {
            request_stack_pop();
            request_stack_push(Id_states::game);
        });
    //----
    std::shared_ptr<gui::Button> settings_button {std::make_shared<gui::Button>(ccontext)};
    settings_button->setPosition(100.0f, 300.0f);
    settings_button->set_text("Settings");
    settings_button->set_callback([this]()
        {
            request_stack_push(Id_states::settings);
        });
    //----
    std::shared_ptr<gui::Button> exit_button {std::make_shared<gui::Button>(ccontext)};
    exit_button->setPosition(100.0f, 350.0f);
    exit_button->set_text("Exit");
    exit_button->set_callback([this]()
        {
            request_stack_pop();
        });

    gui_container.pack(play_button);
    gui_container.pack(settings_button);
    gui_container.pack(exit_button);

    ccontext.music_player->play(Id_music::menu_theme);
}

bool Menu_state::handle_event(const sf::Event& eevent)
{
    gui_container.handle_event(eevent);
    return false;
}

bool Menu_state::update(sf::Time ffixed_timestep)
{
    return false;
}

void Menu_state::draw()
{
    sf::RenderWindow& window{ *get_context().window };
    window.setView(window.getDefaultView());
    window.draw(background_sprite);
    window.draw(gui_container);
}
