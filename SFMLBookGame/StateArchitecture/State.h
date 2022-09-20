#pragma once

#include "../IDs/Id_resources.h"
#include "../IDs/Id_states.h"

class State_stack;
class Input_player;
class Music_player;
class Sound_player;

class State {
public:
    struct Context {
        Context(sf::RenderWindow& wwindow, Texture_holder& ttextures, Font_holder& ffonts, Input_player& iinput_player, Music_player& mmusic_player, Sound_player& ssound_player);

        sf::RenderWindow* window;
        Texture_holder* textures;
        Font_holder* fonts;
        Input_player* input_player;
        Music_player* music_player;
        Sound_player* sound_player;
    };

    State(State_stack& sstack, Context ccontext);
    virtual ~State();

    virtual bool handle_event(const sf::Event& eevent) = 0;
    virtual bool update(sf::Time ffixed_timestep) = 0;
    virtual void draw() = 0;
protected:
    void request_stack_push(Id_states iid_state);
    void request_stack_pop();
    void request_state_clear();
    Context get_context() const;
private:
    State_stack* stack;
    Context context;
};
