#include "../pch.h"
#include "Animation.h"

Animation::Animation()
{
    //empty by default
}

Animation::Animation(const sf::Texture& ttexture)
    : sprite {ttexture}
{
    //empty by default
}

void Animation::set_texture(const sf::Texture& ttexture)
{
    sprite.setTexture(ttexture);
}

const sf::Texture* Animation::get_texture() const
{
    return sprite.getTexture();
}

void Animation::set_frame_size(sf::Vector2i ssize)
{
    frame_size = ssize;
}

sf::Vector2i Animation::get_frame_size() const
{
    return frame_size;
}

void Animation::set_total_frames(int ttotal)
{
    total_frames = ttotal;
}

int Animation::get_total_frames() const
{
    return total_frames;
}

void Animation::set_duration(sf::Time dduration)
{
    duration = dduration;
}

sf::Time Animation::get_duration() const
{
    return duration;
}

void Animation::repeating(bool fflag)
{
    repeat = fflag;
}

bool Animation::repeating() const
{
    return repeat;
}

void Animation::restart()
{
    current_frame = 0;
}

bool Animation::is_finished() const
{
    return current_frame >= total_frames;
}

sf::FloatRect Animation::get_local_bounds() const
{
    return sf::FloatRect {getOrigin(), static_cast<sf::Vector2f>(get_frame_size())};
}

sf::FloatRect Animation::get_global_bounds() const
{
    return getTransform().transformRect(get_local_bounds());
}

void Animation::update(sf::Time ffixed_timestep)
{
    elapsed_time += ffixed_timestep;
    sf::Time time_per_frame {duration / static_cast<float>(total_frames)};
    sf::Vector2i texture_size {sprite.getTexture()->getSize()};
    sf::IntRect texture_rect {sprite.getTextureRect()};
    if(current_frame == 0) {
        texture_rect = sf::IntRect {0, 0, frame_size.x, frame_size.y};
    }
    while(elapsed_time >= time_per_frame && (current_frame <= total_frames || repeat)) {
        elapsed_time -= time_per_frame;
        //move the texture rectangle to the left
        texture_rect.left += texture_rect.width;
        //if we reach the "top-right" of the source texture
        if(texture_rect.left + texture_rect.width > texture_size.x) {
            //move it down one line and to the "top-left"
            texture_rect.left = 0;
            texture_rect.top += texture_rect.height;
        }
        if(repeat) {
            if(current_frame == total_frames + 1) {
                current_frame = 0;
            }
            else {
                ++current_frame;
            }
            if(current_frame == 0) {
                texture_rect = sf::IntRect {0, 0, frame_size.x, frame_size.y};
            }
        }
        else {
            ++current_frame;
        }
    }
    sprite.setTextureRect(texture_rect);
}

void Animation::draw(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
{
    sstates.transform *= getTransform();
    ttarget.draw(sprite, sstates);
}
