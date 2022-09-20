#include "../pch.h"
#include "Component.h"

namespace gui {
    Component::Component()
    {
        //empty
    }

    Component::~Component()
    {
        //empty
    }

    bool Component::is_selected() const
    {
        return selected;
    }

    void Component::select_true()
    {
        selected = true;
    }

    void Component::deselect()
    {
        selected = false;
    }

    bool Component::is_active() const
    {
        return active;
    }

    void Component::activate()
    {
        active = true;
    }

    void Component::deactivate()
    {
        active = false;
    }
}
