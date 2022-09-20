#include "../pch.h"
#include "Scene_node.h"
#include "../ProcessArchitecture/Command.h"
#include "../Others/Utility.h"

Scene_node::Scene_node(entity_categories::Categories ccategory)
    : default_category {ccategory}
{
    //empty by default
}

void Scene_node::attach_child(std::unique_ptr<Scene_node> cchild)
{
    cchild->parent = this;
    children.push_back(std::move(cchild));
}

std::unique_ptr<Scene_node> Scene_node::detach_child(const Scene_node& cchild)
{
    auto iterator {std::find_if(children.begin(), children.end(), [&cchild](std::unique_ptr<Scene_node>& pointer)
        {
            return pointer.get() == &cchild;
        })
    };
    if(iterator == children.end()) {
        throw std::logic_error {"Scene_node::detach_child"};
    }
    std::unique_ptr<Scene_node> value_to_return {std::move(*iterator)};
    value_to_return->parent = nullptr;
    children.erase(iterator);
    return value_to_return;
}

void Scene_node::update(sf::Time ffixed_timestep, Command_queue& ccommand_queue)
{
    update_current(ffixed_timestep, ccommand_queue);
    update_children(ffixed_timestep, ccommand_queue);
}

void Scene_node::update_current(sf::Time ffixed_timestep, Command_queue& ccommand_queue)
{
    //nothing by default, it's meant to be overriden by derived classes
}

void Scene_node::update_children(sf::Time ffixed_timestep, Command_queue& ccommand_queue)
{
    for(std::unique_ptr<Scene_node>& child : children) {
        child->update(ffixed_timestep, ccommand_queue);
    }
}

void Scene_node::draw(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
{
    sstates.transform *= getTransform(); //combine current node transform with the absolute world transform
    draw_current(ttarget, sstates); // the transform, now combined, is passed to this function and the next one
    draw_children(ttarget, sstates);
}

void Scene_node::draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
{
    //nothing by default, it's meant to be overriden by derived classes
}

void Scene_node::draw_children(sf::RenderTarget& ttarget, sf::RenderStates sstates) const
{
    for(const std::unique_ptr<Scene_node>& child : children) {
        child->draw(ttarget, sstates); //this recursion is not dangerous
    }
}

unsigned int Scene_node::get_category() const
{
    return default_category;
}

sf::Transform Scene_node::get_world_transform() const
{
    sf::Transform transform;
    for(const Scene_node* node = this; node != nullptr; node = node->parent) {
        transform *= node->getTransform();
    }
    return transform;
}

sf::Vector2f Scene_node::get_world_position() const
{
    return get_world_transform()* sf::Vector2f {};
}

sf::FloatRect Scene_node::get_bounding_rect() const
{
    return sf::FloatRect {};
}

void Scene_node::on_commmand(const Command& ccommand, sf::Time ffixed_timestep)
{
    if(ccommand.category & get_category()) {
        ccommand.action(*this, ffixed_timestep);
    }
    for(std::unique_ptr<Scene_node>& child : children) {
        child->on_commmand(ccommand, ffixed_timestep);
    }
}

void Scene_node::check_node_collision(Scene_node& nnode, std::set<std::pair<Scene_node*, Scene_node*>>& ccollision_pairs)
{
    if(collision(*this, nnode) && this != &nnode && !nnode.is_destroyed() && !is_destroyed()) {
        ccollision_pairs.insert(std::minmax(this, &nnode));
    }
    for(std::unique_ptr<Scene_node>& child : children) {
        child->check_node_collision(nnode, ccollision_pairs);
    }
}

void Scene_node::check_scene_collision(Scene_node& nnode, std::set<std::pair<Scene_node*, Scene_node*>>& ccollision_pairs)
{
    check_node_collision(nnode, ccollision_pairs);
    for(std::unique_ptr<Scene_node>& child : nnode.children) {
        check_scene_collision(*child, ccollision_pairs);
    }
}

void Scene_node::clean_removals()
{
    children.erase(std::remove_if(children.begin(), children.end(), std::mem_fn(&Scene_node::is_removal)), children.end());
    std::for_each(children.begin(), children.end(), std::mem_fn(&Scene_node::clean_removals));
}

bool collision(const Scene_node& lhs, const Scene_node& rhs)
{
    return lhs.get_bounding_rect().intersects(rhs.get_bounding_rect());
}

float distance(const Scene_node& lhs, const Scene_node& rhs)
{
    return vector_length(lhs.get_world_position() - rhs.get_world_position());
}

bool Scene_node::is_destroyed() const
{
    return false; //scene_graph and scene_layers don't die
}

bool Scene_node::is_removal() const
{
    return false;
}
