#pragma once

#include "TPT Resource_holder.h"
#include "Input_player.h"
#include "../StateArchitecture/State_stack.h"
#include "../MusicSoundProcess/Music_player.h"
#include "../MusicSoundProcess/Sound_player.h"

class Application {
public:
    Application();

    void run();
private:
    void process_input();
    void update(sf::Time ffixed_timestep);
    void render();
    void register_states();

    sf::RenderWindow main_window;
    Texture_holder state_textures;
    Font_holder app_fonts;
    Input_player input_player;
    State_stack state_stack;
    Music_player music_player;
    Sound_player sound_player;
    const sf::Time fixed_timestep {sf::seconds(1.0f / 60.0f)};
};
