#pragma once

#include "../IDs/Categories.h"

struct Command;
class Command_queue;

class Scene_node : public sf::Transformable, public sf::Drawable, private sf::NonCopyable {
public:
    explicit Scene_node(entity_categories::Categories ccategory = entity_categories::none);

    void attach_child(std::unique_ptr<Scene_node> cchild);
    std::unique_ptr<Scene_node> detach_child(const Scene_node& cchild);
    void update(sf::Time ffixed_timestep, Command_queue& ccommand_queue);
    sf::Transform get_world_transform() const;
    sf::Vector2f get_world_position() const;
    void on_commmand(const Command& ccommand, sf::Time ffixed_timestep);
    void check_node_collision(Scene_node& nnode, std::set<std::pair<Scene_node*, Scene_node*>>& ccollision_pairs);
    void check_scene_collision(Scene_node& nnode, std::set<std::pair<Scene_node*, Scene_node*>>& ccollision_pairs);
    void clean_removals();

    virtual unsigned int get_category() const;
    virtual sf::FloatRect get_bounding_rect() const;
    virtual bool is_destroyed() const;
    virtual bool is_removal() const;
private:
    void update_children(sf::Time ffixed_timestep, Command_queue& ccommand_queue);
    void draw_children(sf::RenderTarget& ttarget, sf::RenderStates sstates) const;

    virtual void update_current(sf::Time ffixed_timestep, Command_queue& ccomand_queue);
    virtual void draw_current(sf::RenderTarget& ttarget, sf::RenderStates sstates) const;

    void draw(sf::RenderTarget& ttarget, sf::RenderStates sstates) const final;

    std::vector<std::unique_ptr<Scene_node>> children;
    Scene_node* parent {nullptr};
    entity_categories::Categories default_category;
};

bool collision(const Scene_node& lhs, const Scene_node& rhs);
float distance(const Scene_node& lhs, const Scene_node& rhs);
