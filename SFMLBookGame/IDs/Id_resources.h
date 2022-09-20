#pragma once

enum class Id_textures {
    entities, jungle, title_screen, buttons, explosion, particle, finish_line
};

enum class Id_shaders {
    brightness_pass, down_sample_pass, gaussian_blur_pass, add_pass
};

enum class Id_fonts {
    main
};

enum class Id_music {
    menu_theme, mission_theme
};

enum class Id_sound {
    Allied_gunfire, enemy_gunfire, explosion1, explosion2, launch_missile, collect_pickup, button
};

//forward declaration for name aliases
template<typename Resource, typename Identifier>
class Resource_holder;

using Texture_holder = Resource_holder<sf::Texture, Id_textures>;
using Shader_holder = Resource_holder<sf::Shader, Id_shaders>;
using Font_holder = Resource_holder<sf::Font, Id_fonts>;
using Soundbuffer_holder = Resource_holder<sf::SoundBuffer, Id_sound>;
