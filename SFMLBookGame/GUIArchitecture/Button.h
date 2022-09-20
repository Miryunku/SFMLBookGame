#pragma once

#include "Component.h"
#include "../IDs/Id_resources.h"
#include "../StateArchitecture/State.h"

class Sound_player;

namespace gui {
    class Button : public Component {
    public:
        enum class Type {
            normal, selected, pressed, button_count
        };

        Button(State::Context ccontext);

        void set_callback(std::function<void()> ccallback);
        void set_text(const std::string& ttext);
        void set_toggle(bool ttoggle);

        void select_true() override;
        void deselect() override;
        void activate() override;
        void deactivate() override;
        bool is_selectable() const override;
        void handle_event(const sf::Event& eevent) override;
    private:
        void change_texture(Type bbutton_type);

        void draw(sf::RenderTarget& ttarget, sf::RenderStates sstates) const override;

        std::function<void()> callback;
        sf::Sprite sprite;
        sf::Text text;
        bool toggle {false};
        Sound_player& sound_player;
    };
}
