#include "../pch.h"
#include "Application.h"
#include "../StateArchitecture/Title_state.h"
#include "../StateArchitecture/Menu_state.h"
#include "../StateArchitecture/Settings_state.h"
#include "../StateArchitecture/Game_state.h"
#include "../StateArchitecture/Pause_state.h"
#include "../StateArchitecture/Gameover_state.h"

Application::Application()
    : main_window {sf::VideoMode {640, 480}, "Shoot them up"},
    state_stack{ State::Context {main_window, state_textures, app_fonts, input_player, music_player, sound_player} }
{
    main_window.setKeyRepeatEnabled(false);
    state_textures.load(Id_textures::title_screen, "TitleScreen.png");
    state_textures.load(Id_textures::buttons, "Buttons.png");
    app_fonts.load(Id_fonts::main, "Sansation.ttf");
    register_states();
    state_stack.push_state(Id_states::title);
}

void Application::run()
{
    sf::Time time_since_last_update;
    sf::Time deltatime;
    sf::Clock clock;
    while(main_window.isOpen()) {
        deltatime = clock.restart();
        time_since_last_update += deltatime;
        while(time_since_last_update > fixed_timestep) {
            time_since_last_update -= fixed_timestep;
            process_input();
            update(fixed_timestep);
            if(state_stack.is_empty()) {
                main_window.close();
            }
        }
        render();
    }
}

void Application::process_input()
{
    sf::Event sf_event;
    while(main_window.pollEvent(sf_event)) {
        state_stack.handle_event(sf_event);
        if(sf_event.type == sf::Event::Closed) {
            main_window.close();
        }
    }
}

void Application::update(sf::Time ffixed_timestep)
{
    state_stack.update(ffixed_timestep);
}

void Application::render()
{
    main_window.clear();
    state_stack.draw();
    main_window.display();
}

void Application::register_states()
{
    state_stack.register_state<Title_state>(Id_states::title);
    state_stack.register_state<Menu_state>(Id_states::menu);
    state_stack.register_state<Settings_state>(Id_states::settings);
    state_stack.register_state<Game_state>(Id_states::game);
    state_stack.register_state<Pause_state>(Id_states::pause);
    state_stack.register_state<Gameover_state>(Id_states::gameover);
}
