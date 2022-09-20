#pragma once

#include "../IDs/Id_resources.h"

class Music_player : public sf::NonCopyable {
public:
    Music_player();

    void play(Id_music iid);
    void stop();
    void set_paused(bool ppaused);
    void set_volume(float vvolume);
private:
    sf::Music music;
    std::map<Id_music, std::string> file_names;
    float volume{ 100.0f };
};
