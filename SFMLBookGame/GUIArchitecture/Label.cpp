#include "../pch.h"
#include "Label.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"

namespace gui {
    Label::Label(const std::string& ttext, const Font_holder& ffonts)
        : text {ttext, ffonts.get_resource(Id_fonts::main), 16}
    {
        //empty by default
    }

    void Label::set_text(const std::string& ttext)
    {
        text.setString(ttext);
    }

    bool Label::is_selectable() const
    {
        return false;
    }

    void Label::handle_event(const sf::Event& eevent)
    {
        //empty by default
    }

    void Label::draw(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
    {
        sstates.transform *= getTransform();
        ttarget.draw(text, sstates);
    }
}
