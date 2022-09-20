#pragma once

#include "Component.h"

namespace gui {
    class Container : public Component {
    public:
        Container();

        void pack(std::shared_ptr<Component> ccomponent);

        bool is_selectable() const override;
        void handle_event(const sf::Event& eevent) override;
    private:
        bool has_selection() const;
        void select(int iindex);
        void select_next();
        void select_previous();

        void draw(sf::RenderTarget& ttarget, sf::RenderStates sstates) const override;

        std::vector<std::shared_ptr<Component>> children;
        int selected_child {-1};
    };
}
