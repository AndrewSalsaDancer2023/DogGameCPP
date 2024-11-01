#pragma once
#include <map>
#include <string>
#include <memory>
#include "../core/Game.h"
#include "../core/Dog.h"
#include "../core/DataTypes.h"
/*#include "model.h"

namespace model
{
	class Player;
}
*/
namespace json_serializer {
std::string MakeBadRequestResponce();
std::string MakeMapNotFoundResponce();
std::string MakeAuthResponce(const std::string& auth_key, unsigned playerId);
std::string MakeMappedResponce(const std::map<std::string, std::string>& key_values);
std::string GetMapListResponce(const Game& game);
std::string GetMapContentResponce(const Game& game, const std::string& map_id);
std::string GetPlayerInfoResponce(const std::vector<std::shared_ptr<Player>>& players_info);
std::string GetPlayersDogInfoResponce(const std::vector<std::shared_ptr<Player>>& players, const std::vector<LootInfo>& loots);
std::string MakeRecordsResponce(const Game& game, int start, int max_items);
}  // namespace json_serializer
