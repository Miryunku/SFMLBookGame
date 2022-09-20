#include "../pch.h"
#include "Container.h"

namespace gui {
    Container::Container()
    {
        //empty
    }

    void Container::pack(std::shared_ptr<Component> ccomponent)
    {
        children.push_back(ccomponent);
        if(!has_selection() && ccomponent->is_selectable()) {
            select(children.size() - 1);
        }
    }

    bool Container::is_selectable() const
    {
        return false;
    }

    void Container::handle_event(const sf::Event& eevent)
    {
        //if we have selected a child and it is active then give it events
        if(has_selection() && children[selected_child]->is_active()) {
            children[selected_child]->handle_event(eevent);
        }
        else if(eevent.type == sf::Event::KeyReleased) {
            if(eevent.key.code == sf::Keyboard::W || eevent.key.code == sf::Keyboard::Up) {
                select_previous();
            }
            else if(eevent.key.code == sf::Keyboard::S || eevent.key.code == sf::Keyboard::Down) {
                select_next();
            }
            else if(eevent.key.code == sf::Keyboard::Enter || eevent.key.code == sf::Keyboard::Space) {
                if(has_selection()) {
                    children[selected_child]->activate();
                }
            }
        }
    }

    bool Container::has_selection() const
    {
        return selected_child >= 0;
    }

    void Container::select(int iindex)
    {
        if(children[iindex]->is_selectable()) {
            if(has_selection()) {
                children[selected_child]->deselect();
            }
            children[iindex]->select_true();
            selected_child = iindex;
        }
    }

    void Container::select_next()
    {
        if(!has_selection()) {
            return;
        }
        //search next selectable component, wrap around if necessary
        int next = (selected_child == children.size() - 1) ? 0 : selected_child + 1;
        while(!children[next]->is_selectable()) {
            next = (next == children.size() - 1) ? 0 : next + 1;
        }
        select(next);
    }

    void Container::select_previous()
    {
        if(!has_selection()) {
            return;
        }
        //search previous selectable component, wrap around if necessary
        int previous = (selected_child <= 0) ? children.size() - 1 : selected_child - 1;
        while(!children[previous]->is_selectable()) {
            previous = (previous <= 0) ? children.size() - 1 : previous - 1;
        }
        select(previous);
    }

    void Container::draw(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
    {
        sstates.transform *= getTransform();
        for(const std::vector<std::shared_ptr<Component>>::value_type& child : children) {
            ttarget.draw(*child, sstates);
        }
    }
}
