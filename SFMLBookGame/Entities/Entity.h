#pragma once

#include "../SceneryArchitecture/Scene_node.h"

class Sound_player;

class Entity : public Scene_node {
public:
    explicit Entity(int hhealth_points, Sound_player& ssound_player);

    void set_velocity(sf::Vector2f vvelocity);
    void set_velocity(float xx, float yy);
    sf::Vector2f get_velocity() const;
    void accelerate(sf::Vector2f vvelocity);
    void accelerate(float xx, float yy);
    void repair(int hhealth_points);
    void damage(int hhealth_points);
    void destroy();
    int get_health_points();
    Sound_player& get_dj() const;
    
    virtual void remove();

    bool is_destroyed() const override;
protected:
    void update_current(sf::Time ffixed_timestep, Command_queue& ccommand_queue) override;
private:
    sf::Vector2f velocity;
    int health_points;
    Sound_player& entity_sound_player;
};
