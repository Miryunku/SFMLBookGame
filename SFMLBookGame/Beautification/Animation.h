#pragma once

class Animation : public sf::Drawable, public sf::Transformable {
public:
    Animation();
    Animation(const sf::Texture& ttexture);

    void update(sf::Time ffixed_timestep);

    void set_texture(const sf::Texture& ttexture);
    const sf::Texture* get_texture() const;
    void set_frame_size(sf::Vector2i ssize);
    sf::Vector2i get_frame_size() const;
    void set_total_frames(int ttotal);
    int get_total_frames() const;
    void set_duration(sf::Time dduration);
    sf::Time get_duration() const;
    void repeating(bool fflag);
    bool repeating() const;
    void restart();
    bool is_finished() const;
    sf::FloatRect get_local_bounds() const;
    sf::FloatRect get_global_bounds() const;
private:
    void draw(sf::RenderTarget& ttarget, sf::RenderStates sstates) const override;

    sf::Sprite sprite;
    sf::Vector2i frame_size;
    int total_frames {0};
    int current_frame {0};
    sf::Time duration;
    sf::Time elapsed_time;
    bool repeat {false};
};
