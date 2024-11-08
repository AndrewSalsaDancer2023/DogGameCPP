#pragma once
#include <utility>
#include <map>
#include <string>
#include "MapModel.h"
#include <optional>
#include <memory>
#include "MapParser.h"
#include "Utils.h"
// logger = logging.getLogger(__name__)

class Player;
class GameSession;
class Dog;
using LootParameters = std::pair<float, float>;
using PlayerAuthInfo = std::pair<std::string, int>;
using MapIdToIndex = std::map<std::string, int>;
using PlayersList = std::vector<std::shared_ptr<Player>>;

// std::string config_path = "/data/config.json"
class Game
{
public:    
    Game(std::shared_ptr<MapStorage> storage, std::shared_ptr<utils::ITokenGenerator> generator, 
        int tick_period, bool spawn_in_random_points/*, ConcreteRepository repository, exception_coro: Coroutine*/);
      
    std::shared_ptr<Map> find_map(const std::string& id_)
        { return storage_->get_map(id_); }

    const Maps& get_maps() const
        { return storage_->get_maps(); }

    std::string get_map_as_string(const std::string& id) const
        { return storage_->get_map_as_string(id); }

    const PlayersList& find_all_players_for_auth_info(const std::string& auth_token);
    
    const LootList& get_loots_for_auth_info(const std::string& auth_token);

    std::shared_ptr<Player> GetPlayerWithAuthToken(const std::string& auth_token);

    bool has_session_with_auth_info(const std::string& auth_token);
    std::shared_ptr<GameSession> get_session_with_auth_info(const std::string& auth_token);

    PlayerAuthInfo add_player(const std::string& map_id, const std::string& player_name);
    float get_default_dog_speed()
    { return storage_->get_default_dog_speed(); }

    void MoveDogs(int delta_time);

    void GenerateLoot(int delta_time);
    void set_tick_period(int period)
     { tick_period_ = period; }

    int get_tick_period()
        { return tick_period_; }

    void set_loot_parameters(const LootConfig& loot);

    LootParameters get_loot_parameters()
        { return {loot_period_, loot_probability_}; }


    void timer_handler();

    void set_save_period(int period)
        { save_period = period; }

    GameState create_game_state();

    void serialize_sessions(int interval);
    void init_dog(std::shared_ptr<Dog> dog, const PlayerState& player_state);
    //void restore_sessions(session_content);
    PlayersList find_expired_players();

    void save_expired_players(const PlayersList& players);
    void delete_expired_players(const PlayersList& players);
    void delete_empty_sessions();
    void handle_retired_players();
    std::vector<PlayerRecordItem> get_retired_players(int start, int max_items);
    void set_dog_retirement_time(float period)
        { dog_retirement_time_ = period; }
    void set_default_dog_speed(float speed)
        { default_dog_speed_ = speed; }

    std::shared_ptr<GameSession> __get_session_for_token(const std::string& auth_token);
private:
    std::shared_ptr<GameSession> __find_session(const std::string& map_name);
    // std::shared_ptr<GameSession> __get_session_for_token(const std::string& auth_token);
    
    std::shared_ptr<MapStorage> storage_;
    std::shared_ptr<utils::ITokenGenerator> generator_;
    std::vector<std::shared_ptr<GameSession>> sessions_;
    float default_dog_speed_ = 0.0;
    float dog_retirement_time_ {60*1000.0f};
    int tick_period_{};
    bool spawn_in_random_points_{};
    float loot_period_ = 0.0f;
    float loot_probability_ = 0.0f;
    int save_period = 0;
    int time_without_saving_ = 0;
};

std::shared_ptr<Game> create_game(std::shared_ptr<MapStorage> storage, std::shared_ptr<utils::PlayerToken> tokenizer,
                                  int tick_period = 100, bool spawn_random_point = false
                                 /*, repository: ConcreteRepository, exception_coro: Coroutine*/);