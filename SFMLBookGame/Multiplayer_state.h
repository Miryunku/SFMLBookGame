#pragma once

#include "StateArchitecture/State.h"
#include "ProcessArchitecture/World.h"
#include "Game_server.h"

class Multiplayer_state : State {
public:
    using Input_player_ptr = std::unique_ptr<Input_player>;

    Multiplayer_state(State_stack& state_stack, Context context, bool is_host);

    void on_activate();
    void on_destroy();
    void disable_all_realtime_actions();

    bool handle_event(const sf::Event& eevent) override;
    bool update(sf::Time ffixed_timestep) override;
    void draw() override;
private:
    void update_broadcast_message(sf::Time elapsed_time);
    void handle_packet(sf::Int32 packet_type, sf::Packet& packet);

    sf::RenderWindow& window;
    World world;
    Texture_holder& texture_holder;
    //= = = =
    std::map<int, Input_player_ptr> input_players;
    std::vector<sf::Int32> local_players_IDs;
    sf::TcpSocket socket;
    bool connected;
    //= = = =
    std::unique_ptr<Game_server> game_server;
    sf::Clock tick_clock;
    std::vector<std::string> broadcasts;
    //= = = =
    sf::Text broadcast_text;
    sf::Time broadcast_elapsed_time;
    //= = = =
    sf::Text player_invitation_text;
    sf::Time player_invitation_time;
    //= = = =
    sf::Text failed_connection_text;
    sf::Clock failed_connection_clock;
    //= = = =
    bool active_state;
    bool has_focus;
    bool host;
    bool game_started;
    sf::Time client_timeout;
    sf::Time time_since_last_packet;
};
