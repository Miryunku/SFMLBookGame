#include "../pch.h"
#include "Text_node.h"
#include "../ProcessArchitecture/TPT Resource_holder.h"
#include "../Others/Utility.h"

Text_node::Text_node(const Font_holder& ffonts)
{
    text.setFont(ffonts.get_resource(Id_fonts::main));
    text.setCharacterSize(20);
}

void Text_node::set_string(const std::string& ttext)
{
    text.setString(ttext);
    center_origin(text);
}

void Text_node::draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
{
    ttarget.draw(text, sstates);
}
