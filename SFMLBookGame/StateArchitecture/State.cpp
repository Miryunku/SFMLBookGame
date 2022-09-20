#include "../pch.h"
#include "State.h"
#include "State_stack.h"

State::Context::Context(sf::RenderWindow& wwindow, Texture_holder& ttextures, Font_holder& ffonts, Input_player& iinput_player, Music_player& mmusic_player, Sound_player& ssound_player)
    : window {&wwindow},
    textures {&ttextures},
    fonts {&ffonts},
    input_player {&iinput_player},
    music_player{ &mmusic_player },
    sound_player {&ssound_player}
{
    //empty by default
}

State::State(State_stack& sstack, Context ccontext)
    : stack {&sstack},
    context {ccontext}
{
    //empty by default
}

State::~State()
{
    //empty by default
}

void State::request_stack_push(Id_states iid_states)
{
    stack->push_state(iid_states);
}

void State::request_stack_pop()
{
    stack->pop_state();
}

void State::request_state_clear()
{
    stack->clear_states();
}

State::Context State::get_context() const
{
    return context;
}
