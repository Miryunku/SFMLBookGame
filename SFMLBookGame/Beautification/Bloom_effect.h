#pragma once

#include "Post_effect.h"
#include "../IDs/Id_resources.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"

class Bloom_effect : public Post_effect {
public:
    Bloom_effect();
    
    void apply(const sf::RenderTexture& iinput, sf::RenderTarget& ttarget) override;
private:
    void prepare_textures(sf::Vector2u ssize);
    void filter_bright(const sf::RenderTexture& iinput, sf::RenderTexture& ooutput);
    void downsample(sf::RenderTexture& iinput, sf::RenderTexture& ooutput);
    void blur_multipass(std::array<sf::RenderTexture, 2>& rrender_textures);
    void blur(const sf::RenderTexture& iinput, sf::RenderTexture& ooutput, sf::Vector2f ooffset_factor);
    void add(const sf::RenderTexture& ssource, const sf::RenderTexture& bbloom, sf::RenderTarget& ttarget);

    Shader_holder shader_holder;
    sf::RenderTexture brightness_texture;
    std::array<sf::RenderTexture, 2> first_pass_textures;
    std::array<sf::RenderTexture, 2> second_pass_texture;
};
