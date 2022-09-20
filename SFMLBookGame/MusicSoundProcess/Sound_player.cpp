#include "../pch.h"
#include "Sound_player.h"

namespace {
    //sound coordinate system, point of view of a player in front of the screen:
    //X = left; Y = up; Z = back (out of the screen)
    const float listener_z{ 300.0f };
    const float attenuation{ 8.0f };
    const float min_distance_2d{ 200.0f };
    const float min_distance_3d{ std::sqrt(min_distance_2d * min_distance_2d + listener_z * listener_z) };
}

Sound_player::Sound_player()
{
    sound_buffers.load(Id_sound::Allied_gunfire, "GunFire1.wav");
    sound_buffers.load(Id_sound::enemy_gunfire, "GunFire2.wav");
    sound_buffers.load(Id_sound::explosion1, "BBS Scroll.wav");
    sound_buffers.load(Id_sound::explosion2, "BBS Back.wav");
    sound_buffers.load(Id_sound::launch_missile, "BBS Menu.wav");
    sound_buffers.load(Id_sound::collect_pickup, "BBS Get Item.wav");
    sound_buffers.load(Id_sound::button, "BBS Confirm.wav");

    sf::Listener::setPosition(0.0f, 0.0f, -1.0f);
}

void Sound_player::play(Id_sound iid)
{
    play(iid, get_listener_position());
}

void Sound_player::play(Id_sound iid, sf::Vector2f pposition)
{
    active_sounds.push_back(sf::Sound{});
    sf::Sound& sound{ active_sounds.back() };
    sound.setBuffer(sound_buffers.get_resource(iid));
    sound.setPosition(pposition.x, -pposition.y, 0.0f);
    sound.setAttenuation(attenuation);
    sound.setMinDistance(min_distance_3d);
    sound.play();
}

void Sound_player::remove_stopped_sounds()
{
    active_sounds.remove_if([](const sf::Sound& ssound)
        {
            return ssound.getStatus() == sf::Sound::Stopped;
        });
}

void Sound_player::set_listener_position(sf::Vector2f pposition)
{
    sf::Listener::setPosition(pposition.x, -pposition.y, listener_z);
}

sf::Vector2f Sound_player::get_listener_position() const
{
    sf::Vector3f position{ sf::Listener::getPosition() };
    return sf::Vector2f{ position.x, -position.y };
}
