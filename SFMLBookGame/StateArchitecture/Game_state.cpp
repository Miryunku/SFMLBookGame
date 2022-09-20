#include "../pch.h"
#include "Game_state.h"
#include "../MusicSoundProcess/Music_player.h"

Game_state::Game_state(State_stack& sstack, Context ccontext)
    : State {sstack, ccontext},
    world {*ccontext.window, *ccontext.fonts, *ccontext.sound_player},
    input_player {*ccontext.input_player}
{
    input_player.set_mission_status(Input_player::Mission_status::mission_running);
    ccontext.music_player->play(Id_music::mission_theme);
}

bool Game_state::handle_event(const sf::Event& eevent)
{
    Command_queue& command_queue {world.get_command_queue()};
    input_player.handle_event(eevent, command_queue);
    if(eevent.type == sf::Event::KeyPressed && eevent.key.code == sf::Keyboard::Escape) {
        request_stack_push(Id_states::pause);
    }
    return false;
}

bool Game_state::update(sf::Time ffixed_timestep)
{
    world.update(ffixed_timestep);
    if(!world.is_player_alive()) {
        input_player.set_mission_status(Input_player::Mission_status::mission_failure);
        request_stack_push(Id_states::gameover);
    }
    else if(world.has_player_reached_end()) {
        input_player.set_mission_status(Input_player::Mission_status::mission_success);
        request_stack_push(Id_states::gameover);
    }
    Command_queue& command_queue {world.get_command_queue()};
    input_player.handle_realtime_input(command_queue);
    return false;
}

void Game_state::draw()
{
    world.draw();
}
