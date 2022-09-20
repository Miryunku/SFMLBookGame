#include "../pch.h"
#include "Music_player.h"

Music_player::Music_player()
{
    file_names[Id_music::menu_theme] = "Syatten - Select.wav";
    file_names[Id_music::mission_theme] = "Yamajet - Select.wav";
}

void Music_player::play(Id_music iid)
{
    std::string music_file{ file_names[iid] };
    if (!music.openFromFile(music_file)) {
        throw std::runtime_error{ "Music_player::play() " + music_file };
    }
    music.setVolume(volume);
    music.setLoop(true);
    music.play();
}

void Music_player::stop()
{
    music.stop();
}

void Music_player::set_paused(bool ppaused)
{
    if (ppaused) {
        music.pause();
    }
    else {
        music.play();
    }
}

void Music_player::set_volume(float vvolume)
{
    volume = vvolume;
}
