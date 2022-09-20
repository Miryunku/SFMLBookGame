#include "../pch.h"
#include "Button.h"
#include "../IDs/Id_resources.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"
#include "../Others/Utility.h"
#include "../MusicSoundProcess/Sound_player.h"

namespace gui {
    Button::Button(State::Context ccontext)
        : sprite{ ccontext.textures->get_resource(Id_textures::buttons) },
        text{ "", ccontext.fonts->get_resource(Id_fonts::main), 16 },
        sound_player{ *ccontext.sound_player }
    {
        change_texture(Type::normal);
        sf::FloatRect bounds {sprite.getLocalBounds()};
        text.setPosition(bounds.width / 2.0f, bounds.height / 2.0f);
    }

    void Button::set_callback(std::function<void()> ccallback)
    {
        callback = std::move(ccallback);
    }

    void Button::set_text(const std::string& ttext)
    {
        text.setString(ttext);
        center_origin(text);
    }

    void Button::set_toggle(bool ttoggle)
    {
        toggle = ttoggle;
    }

    void Button::select_true()
    {
        Component::select_true();
        change_texture(Type::selected);
    }

    void Button::deselect()
    {
        Component::deselect();
        change_texture(Type::normal);
    }

    void Button::activate()
    {
        Component::activate();
        //if toggle is off, deactivate the button since we are just momentarily activated
        if(!toggle) {
            deactivate();
        }
        //if toggle is on, show that the button is pressed and thus "toggled"
        if(toggle) {
            change_texture(Type::pressed);
        }
        if(callback) {
            callback();
        }
        sound_player.play(Id_sound::button);
    }

    void Button::deactivate()
    {
        Component::deactivate();
        if(toggle) {
            if(is_selected()) {
                change_texture(Type::selected);
            }
            else {
                change_texture(Type::normal);
            }
        }
    }

    bool Button::is_selectable() const
    {
        return true;
    }

    void Button::change_texture(Type bbutton_type)
    {
        sf::IntRect texture_rect {0, 50 * static_cast<int>(bbutton_type), 200, 50};
        sprite.setTextureRect(texture_rect);
    }

    void Button::handle_event(const sf::Event& eevent)
    {
        //empty by default
    }

    void Button::draw(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
    {
        sstates.transform *= getTransform();
        ttarget.draw(sprite, sstates);
        ttarget.draw(text, sstates);
    }
}
