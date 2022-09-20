#pragma once

class Post_effect {
public:
    virtual ~Post_effect();

    virtual void apply(const sf::RenderTexture& iinput, sf::RenderTarget& ttarget) = 0;
protected:
    static void apply_shader(const sf::Shader& sshader, sf::RenderTarget& ttarget);
};
