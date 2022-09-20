#pragma once

namespace gui {
    class Component : public sf::Transformable, public sf::Drawable, private sf::NonCopyable {
    public:
        Component();
        virtual ~Component();

        virtual bool is_selectable() const = 0;
        virtual void handle_event(const sf::Event& eevent) = 0;

        bool is_selected() const;

        virtual void select_true();
        virtual void deselect();
        virtual bool is_active() const;
        virtual void activate();
        virtual void deactivate();
    private:
        bool selected {false};
        bool active {false};
    };
}
