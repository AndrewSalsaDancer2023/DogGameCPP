#include "GameSession.h"
#include "Exceptions.h"
#include "Dog.h"
#include <string>
#include <algorithm>
#include <optional>
#include "Utils.h"

constexpr float baseWidth = 0.5f;
constexpr float lootWidth = 0.0f;

GameSession::GameSession(const std::string& map_id, float loot_period, float loot_probability)
{
    map_id_ = map_id;
	init_loot_generator(loot_period, loot_probability);
}

SessionInfo GameSession::get_session_state()
{
	auto info = SessionInfo();
	info.map_id = map_id_;
	info.loots_info_state = loots_info_;
	for(const auto& player : players_)
		info.players.push_back(player->get_state());

	return info;
}

void GameSession::init_loot_generator(float loot_period, float loot_probability)
{
	int duration = loot_period * 1000;
	lootGen_ = std::make_unique<LootGenerator>(LootGenerator::TimeInterval{duration}, loot_probability);
}

std::shared_ptr<Player> GameSession::add_player(std::string player_name, shared_ptr<Map> player_map, bool spawn_dog_in_random_point, int default_bag_capacity)
{
	map_ = player_map;
	auto itFind = std::find_if(std::begin(players_), std::end(players_), [&player_name](shared_ptr<Player>& plr) {
		return plr->GetName() == player_name;
	});

	if(itFind != players_.end())
		return *itFind;
	

	utils::PlayerToken token_generator;
	auto dog = std::make_shared<Dog>(map_, default_bag_capacity);
	auto player = std::make_shared<Player>(player_id, player_name, token_generator.GetToken(), dog);
	players_.push_back(player);
	player_id ++;

	return player;
}

bool GameSession::HasPlayerWithAuthToken(const std::string& auth_token)
{
	auto itFind = std::find_if(std::begin(players_), std::end(players_), [&auth_token](shared_ptr<Player>& player) {
		return player->GetToken() == auth_token;
	});
	
	if(itFind != players_.end())
		return true;

	return false;
}

std::shared_ptr<Player> GameSession::GetPlayerWithAuthToken(const std::string& auth_token)
{
	auto itFind = std::find_if(std::begin(players_), std::end(players_), [&auth_token](shared_ptr<Player>& player) {
		return player->GetToken() == auth_token;
	});
	
	if(itFind != players_.end())
		return *itFind;

	throw PlayerAbsentException();
}

void GameSession::MoveDogs(int delta_time)
{
	for(const auto& player : players_)
	{
		auto dog = player->GetDog();
		dog->add_play_time(delta_time);

		auto gatherer = dog->move(delta_time);
		if(!gatherer)
			dog->add_idle_time(delta_time);
			return;

		dog->set_idle_time(0);

		auto items = get_gathered_items(*gatherer, loots_info_, map_);
		add_loot_to_dog(dog, loots_info_, items);
	}
}

void GameSession::GenerateLoot(int deltaTime, shared_ptr<Map> map_)
{
		auto num_loot_to_generate = lootGen_->Generate(LootGenerator::TimeInterval{deltaTime}, loots_info_.size(), players_.size());

		while(num_loot_to_generate > 0)
		{
			loots_info_.push_back(generate_loot_info(map_));
			num_loot_to_generate--;
		}
}

std::pair<float, float> GameSession::get_loot_position(Point start, Point end, bool horizontal_road)
{
	if(horizontal_road)
	{
		if(start.x > end.x)
			std::swap(start, end);

		float x = utils::GetRandomFloatNumber(start.x, end.x);
		float y = start.y;
		return {x, y};
	}

	if(start.y > end.y)
		std::swap(start, end);

	float x = start.x;
	float y = utils::GetRandomFloatNumber(start.y, end.y);
	return {x, y};
}

LootInfo GameSession::generate_loot_info(shared_ptr<Map> map_)
{
	auto roads = map_->roads();
	auto num_loots = map_->get_num_loots();
	if(num_loots == 0)
		throw LootNotSpecifiedException();

	auto loot_type = utils::GetRandomIntNumber(0, num_loots-1);

	int num_roads = map_->get_num_roads();
	if(num_roads == 0)
		throw RoadNotSpecifiedException();

	int road_index = utils::GetRandomIntNumber(0, num_roads-1);

	const auto& start = roads[road_index].get_start();
	const auto& end = roads[road_index].get_end();

	auto [x, y] = get_loot_position(start, end, roads[road_index].is_horizontal());

	LootInfo loot_info{loot_id_, loot_type, x, y};
	loot_id_ += 1;
	return loot_info;
}

ItemList GameSession::get_gathered_items(Gatherer gatherer, const LootList& loots, shared_ptr<Map> map_)
{
	ItemList items;

	for(const auto& cur_loot : loots)
	{
		auto item = Item(cur_loot.id, Point{cur_loot.x, cur_loot.y}, lootWidth);
		items.push_back(item);
	}

	for(const auto& office : map_->offices())
	{
		auto item = Item(0, office.position, baseWidth, ItemType::Office);
		items.push_back(item);
	}

	auto item_gath = ItemGatherer(items, {gatherer});
	auto ev = find_gather_events(item_gath);

	ItemList result;

	for(auto i = 0; i < ev.size(); ++i)
		result.push_back(item_gath.get_item(ev[i].item_id));

	return result;
}

void GameSession::add_loot_to_dog(shared_ptr<Dog> dog, LootList& loots, const ItemList& items)
{
	for(const auto& item : items)
	{
		if(item.type_ == ItemType::Office)
			dog->pass_loot_to_office();
		else
		{
			auto itFind = std::find_if(loots.begin(), loots.end(), [&item](LootInfo& elem) {
				return elem.id == item.id_;
			});
	
			if(itFind == loots.end())
				continue;

			if(dog->add_loot(*itFind))
				loots.erase(itFind);
		}
	}
}

void GameSession::delete_player(std::shared_ptr<Player> player)
{ 	
	auto findIt = std::find(std::begin(players_), std::end(players_), player);
	if(findIt == std::end(players_))
		return;

	const auto new_end{std::remove(std::begin(players_), std::end(players_), *findIt)};
	players_.erase(new_end, std::end(players_));
}