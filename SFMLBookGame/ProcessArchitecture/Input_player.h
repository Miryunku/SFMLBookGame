#pragma once

#include "Command.h"
#include "Command_queue.h"

//struct Command;
//class Command_queue;

class Input_player {
public:
    enum class Action {
        moveup, movedown, moveleft, moveright, fire_bullet, fire_missile, action_count
    };

    enum class Mission_status {
        mission_running, mission_failure, mission_success
    };

    Input_player();

    void handle_event(const sf::Event& eevent, Command_queue& ccommand_queue);
    void handle_realtime_input(Command_queue& ccommand_queue);
    void assign_key(sf::Keyboard::Key kkey, Action aaction);
    sf::Keyboard::Key get_assigned_key(Action aaction) const;
    void set_mission_status(Mission_status sstatus);
    Mission_status get_mission_status() const;
private:
    void initialize_actions();
    static bool is_realtime_action(Action aaction);

    std::map<sf::Keyboard::Key, Action> key_bindings;
    std::map<Action, Command> action_bindings;
    Mission_status mission_status {Mission_status::mission_running};
};
