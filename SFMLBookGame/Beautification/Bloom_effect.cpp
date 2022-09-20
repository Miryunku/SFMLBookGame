#include "../pch.h"
#include "Bloom_effect.h"

Bloom_effect::Bloom_effect()
{
    shader_holder.load(Id_shaders::brightness_pass, "Fullpass.vert", "Brightness.frag");
    shader_holder.load(Id_shaders::down_sample_pass, "Fullpass.vert", "DownSample.frag");
    shader_holder.load(Id_shaders::gaussian_blur_pass, "Fullpass.vert", "GuassianBlur.frag");
    shader_holder.load(Id_shaders::add_pass, "Fullpass.vert", "Add.frag");
}

void Bloom_effect::apply(const sf::RenderTexture& iinput, sf::RenderTarget& ttarget)
{
    prepare_textures(iinput.getSize());
    filter_bright(iinput, brightness_texture);
    downsample(brightness_texture, first_pass_textures[0]);
    blur_multipass(first_pass_textures);
    downsample(first_pass_textures[0], second_pass_texture[0]);
    blur_multipass(second_pass_texture);
    add(first_pass_textures[0], second_pass_texture[0], first_pass_textures[1]);
    first_pass_textures[1].display();
    add(iinput, first_pass_textures[1], ttarget);
}

void Bloom_effect::prepare_textures(sf::Vector2u ssize)
{
    if(brightness_texture.getSize() != ssize) {
        brightness_texture.create(ssize.x, ssize.y);
        brightness_texture.setSmooth(true);

        first_pass_textures[0].create(ssize.x / 2, ssize.y / 2);
        first_pass_textures[0].setSmooth(true);
        first_pass_textures[1].create(ssize.x / 2, ssize.y / 2);
        first_pass_textures[1].setSmooth(true);

        second_pass_texture[0].create(ssize.x / 4, ssize.y / 4);
        second_pass_texture[0].setSmooth(true);
        second_pass_texture[1].create(ssize.x / 4, ssize.y / 4);
        second_pass_texture[1].setSmooth(true);
    }
}

void Bloom_effect::filter_bright(const sf::RenderTexture& iinput, sf::RenderTexture& ooutput)
{
    sf::Shader& brightness_sh {shader_holder.get_resource(Id_shaders::brightness_pass)};
    brightness_sh.setUniform("source", iinput.getTexture());
    apply_shader(brightness_sh, ooutput);
    ooutput.display();
}

void Bloom_effect::downsample(sf::RenderTexture& iinput, sf::RenderTexture& ooutput)
{
    sf::Shader& downsample_sh {shader_holder.get_resource(Id_shaders::down_sample_pass)};
    downsample_sh.setUniform("source", iinput.getTexture());
    downsample_sh.setUniform("sourceSize", sf::Vector2f {iinput.getSize()});
    apply_shader(downsample_sh, ooutput);
    ooutput.display();
}

void Bloom_effect::blur_multipass(std::array<sf::RenderTexture, 2>& rrender_textures)
{
    sf::Vector2u texture_size {rrender_textures[0].getSize()};
    for(int i {0}; i < rrender_textures.size(); ++i) {
        blur(rrender_textures[0], rrender_textures[1], sf::Vector2f {0.0f, 1.0f / texture_size.y});
        blur(rrender_textures[1], rrender_textures[0], sf::Vector2f {1.0f / texture_size.x, 0.0f});
    }
}

void Bloom_effect::blur(const sf::RenderTexture& iinput, sf::RenderTexture& ooutput, sf::Vector2f ooffset_factor)
{
    sf::Shader& gaussian_blur_sh {shader_holder.get_resource(Id_shaders::gaussian_blur_pass)};
    gaussian_blur_sh.setUniform("source", iinput.getTexture());
    gaussian_blur_sh.setUniform("offsetFactor", ooffset_factor);
    apply_shader(gaussian_blur_sh, ooutput);
    ooutput.display();
}

void Bloom_effect::add(const sf::RenderTexture& ssource, const sf::RenderTexture& bbloom, sf::RenderTarget& ttarget)
{
    sf::Shader& add_sh {shader_holder.get_resource(Id_shaders::add_pass)};
    add_sh.setUniform("source", ssource.getTexture());
    add_sh.setUniform("bloom", bbloom.getTexture());
    apply_shader(add_sh, ttarget);
}
