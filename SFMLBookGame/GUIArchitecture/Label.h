#pragma once

#include "Component.h"
#include "../IDs/Id_resources.h"

namespace gui {
    class Label : public Component {
    public:
        Label(const std::string& ttext, const Font_holder& ffonts);

        void set_text(const std::string& ttext);

        bool is_selectable() const override;
        void handle_event(const sf::Event& eevent) override;
    private:
        void draw(sf::RenderTarget& ttarget, sf::RenderStates sstates) const override;

        sf::Text text;
    };
}
