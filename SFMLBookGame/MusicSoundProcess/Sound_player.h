#pragma once

#include "../IDs/Id_resources.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"

class Sound_player : public sf::NonCopyable {
public:
    Sound_player();

    void play(Id_sound iid);
    void play(Id_sound iid, sf::Vector2f pposition);
    void remove_stopped_sounds();
    void set_listener_position(sf::Vector2f pposition);
    sf::Vector2f get_listener_position() const;
private:
    Soundbuffer_holder sound_buffers;
    std::list<sf::Sound> active_sounds;
};
