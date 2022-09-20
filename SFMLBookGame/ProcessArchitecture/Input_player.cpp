#include "../pch.h"
#include "Input_player.h"
#include "../IDs/Categories.h"
#include "../Entities/Aircraft.h"

struct Aircraft_mover {
    Aircraft_mover(float xx, float yy);

    void operator()(Scene_node& nnode, sf::Time ffixed_timestep);

    sf::Vector2f velocity;
};

Aircraft_mover::Aircraft_mover(float xx, float yy)
    : velocity {xx, yy}
{
    //empty by default
}

void Aircraft_mover::operator()(Scene_node& nnode, sf::Time ffixed_timestep)
{
    Aircraft& aircraft {static_cast<Aircraft&>(nnode)};
    aircraft.accelerate(velocity);
}

struct Aircraft_bulleter {
    void operator()(Scene_node& nnode, sf::Time ffixed_timestep);
};

void Aircraft_bulleter::operator()(Scene_node& nnode, sf::Time ffixed_timestep)
{
    Aircraft& aircraft {static_cast<Aircraft&>(nnode)};
    aircraft.fire();
}

struct Aircraft_missiler {
    void operator()(Scene_node& nnode, sf::Time ffixed_timestep);
};

void Aircraft_missiler::operator()(Scene_node& nnode, sf::Time ffixed_timestep)
{
    Aircraft& aircraft {static_cast<Aircraft&>(nnode)};
    aircraft.launch_missile();
}

Input_player::Input_player()
{
    //set initial key bindings
    key_bindings[sf::Keyboard::Up] = Action::moveup;
    key_bindings[sf::Keyboard::Down] = Action::movedown;
    key_bindings[sf::Keyboard::Left] = Action::moveleft;
    key_bindings[sf::Keyboard::Right] = Action::moveright;
    key_bindings[sf::Keyboard::Z] = Action::fire_bullet;
    key_bindings[sf::Keyboard::X] = Action::fire_missile;

    //set initial action bindings
    initialize_actions();
    for(std::map<Action, Command>::value_type& pair : action_bindings) {
        pair.second.category = entity_categories::player_aircraft;
    }
}

void Input_player::handle_event(const sf::Event& eevent, Command_queue& ccommand_queue)
{
    if(eevent.type == sf::Event::KeyPressed) {
        std::map<sf::Keyboard::Key, Action>::iterator iter {key_bindings.find(eevent.key.code)};
        if(iter != key_bindings.end() && !is_realtime_action(iter->second)) {
            ccommand_queue.push(action_bindings[iter->second]);
        }
    }
}

void Input_player::handle_realtime_input(Command_queue& ccommand_queue)
{
    for(std::map<sf::Keyboard::Key, Action>::value_type& pair : key_bindings) {
        if(sf::Keyboard::isKeyPressed(pair.first) && is_realtime_action(pair.second)) {
            ccommand_queue.push(action_bindings[pair.second]);
        }
    }
}

void Input_player::assign_key(sf::Keyboard::Key kkey, Action aaction)
{
    //remove any key that already maps to an action
    for(std::map<sf::Keyboard::Key, Action>::iterator iter {key_bindings.begin()}; iter != key_bindings.end(); ++iter) {
        if(iter->second == aaction) {
            key_bindings.erase(iter);
            break;
        }
    }
    key_bindings[kkey] = aaction;
}

sf::Keyboard::Key Input_player::get_assigned_key(Action aaction) const
{
    for(const std::map<sf::Keyboard::Key, Action>::value_type& pair : key_bindings) {
        if(pair.second == aaction) {
            return pair.first;
        }
    }
    return sf::Keyboard::Unknown;
}

void Input_player::set_mission_status(Input_player::Mission_status sstatus)
{
    mission_status = sstatus;
}

Input_player::Mission_status Input_player::get_mission_status() const
{
    return mission_status;
}

void Input_player::initialize_actions()
{
    constexpr float player_speed {200.0f};
    Aircraft_mover move_up {0.0f, -player_speed};
    Aircraft_mover move_down {0.0f, player_speed};
    Aircraft_mover move_left {-player_speed, 0.0f};
    Aircraft_mover move_right {player_speed, 0.0f};
    Aircraft_bulleter bulleter;
    Aircraft_missiler missiler;

    action_bindings[Action::moveup].action = move_up;
    action_bindings[Action::movedown].action = move_down;
    action_bindings[Action::moveleft].action = move_left;
    action_bindings[Action::moveright].action = move_right;
    action_bindings[Action::fire_bullet].action = bulleter;
    action_bindings[Action::fire_missile].action = missiler;
}

bool Input_player::is_realtime_action(Action aaction)
{
    switch(aaction) {
    case Action::moveup:
    case Action::movedown:
    case Action::moveleft:
    case Action::moveright:
    case Action::fire_bullet:
        return  true;
    default:
        return false;
    }
}
