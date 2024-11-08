#pragma once
#include <string>
#include "MapModel.h"
#include "DataTypes.h"
#include "CollisionDetector.h"
#include "LootGenerator.h"
#include <optional>
#include <memory>

// class LootGenerator;
class Player;
class Dog;
using PlayersList = vector<shared_ptr<Player>>;

using namespace std;

class GameSession
{
public:
    GameSession(const std::string& map_id, float loot_period, float loot_probability);
    SessionInfo get_session_state();
    void init_loot_generator(float loot_period, float loot_probability);
    std::shared_ptr<Player> add_player(const std::string& player_name, shared_ptr<Map> player_map, const std::string& token, bool spawn_dog_in_random_point, int default_bag_capacity);
    std::string GetMap()
        { return map_id_;}
    bool HasPlayerWithAuthToken(const std::string& auth_token);
    const PlayersList& get_all_players()
	    { return players_; }

    std::shared_ptr<Player> GetPlayerWithAuthToken(const std::string& auth_token);
    void delete_player(std::shared_ptr<Player> player);
    void MoveDogs(int delta_time);

    int GetNumPlayers()
	{ return players_.size(); }

    const LootList& get_loots_info()
    { return loots_info_; }

    void set_loot_list(LootList loot)
    {
		loots_info_ = std::move(loot);
		loot_id_ += loot.size();
    }

    void GenerateLoot(int deltaTime, shared_ptr<Map> map_);
    std::pair<float, float> get_loot_position(Point start, Point end, bool horizontal_road);
    LootInfo generate_loot_info(shared_ptr<Map> map_);
    ItemList get_gathered_items(Gatherer gatherer, const LootList& loots, shared_ptr<Map> map_);
    void add_loot_to_dog(shared_ptr<Dog> dog, LootList& loots, const ItemList& items);

private:
    std::string map_id_;
	std::unique_ptr<LootGenerator> lootGen_;
	PlayersList players_;
	LootList loots_info_;
	int player_id{};
	shared_ptr<Map> map_;
	int loot_id_{};
};

using SessionList = std::vector<std::shared_ptr<GameSession>>;