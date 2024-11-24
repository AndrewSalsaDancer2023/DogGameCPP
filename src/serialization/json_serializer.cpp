#include "json_serializer.h"
#include <utility>
#include <boost/json.hpp>
#include "../utility/utility_functions.h"

namespace json = boost::json;
using namespace std::literals;

// const int MILLISECONDS_IN_SECOND = 1000;
namespace json_serializer {

   std::string MakeErrorResponce(const std::string& codeMessage, const std::string& errorMessage)
   {
        json::object resp_object;
        resp_object["code"] = codeMessage;
        resp_object["message"] = errorMessage;
      	return json::serialize(resp_object);
   }

   std::string MakeMapNotFoundResponce()
   {
	return	MakeErrorResponce("mapNotFound", "Map not found");
   }
   
   std::string MakeBadRequestResponce()
   {
	return	MakeErrorResponce("badRequest", "Bad request");   
   }

   std::string MakeAuthResponce(const std::string& auth_key, unsigned playerId)
   {
        json::object resp_object;
        
        resp_object["authToken"] = auth_key;
        resp_object["playerId"] = playerId;
                    
      	return json::serialize(resp_object);
   }

   std::string MakeMappedResponce(const std::map<std::string, std::string>& key_values)
   {
       json::object resp_object;

       for(const auto&[key, value] : key_values)
    	   resp_object[key] = value;

       return json::serialize(resp_object);
   }

   std::string GetPlayerInfoResponce(const std::vector<std::shared_ptr<Player>>& players_info)
   {
	   json::object resp_object;

	   for(auto& player : players_info)
	   {
		   json::object name_object;
		   name_object["name"] = player->GetName();

		   resp_object[std::to_string(player->get_id())] = name_object;
	   }

	  return json::serialize(resp_object);
   }

   json::array SerializeDogBag(const std::vector<LootInfo>& loots)
   {
      json::array bag_ar;

      for(const auto& cur_loot: loots)
      {
    	  json::object loot_object;

    	  loot_object["id"] = cur_loot.id;
		  loot_object["type"] = cur_loot.type;

		  bag_ar.emplace_back(loot_object);
      }

	   return bag_ar;
   }

   json::object SerializePlayers(const std::vector<std::shared_ptr<Player>>& players)
   {
	   json::object players_object;

	   for(auto& player : players)
	  	{
		    auto dog =  player->GetDog();

	  		json::object dog_object;
	  		json::array pos_ar, speed_ar;

	  		auto pos = dog->get_position();
	  		pos_ar.emplace_back(pos.x);
	  		pos_ar.emplace_back(pos.y);

	  		dog_object["pos"] = pos_ar;

	  		auto speed = dog->get_speed();
	  		speed_ar.emplace_back(speed.vx);
			speed_ar.emplace_back(speed.vy);

			dog_object["speed"] = speed_ar;

			DogDirection dir = dog->get_direction();
			dog_object["dir"] = string_from_direction(dir);

			dog_object["bag"] = SerializeDogBag(dog->get_gathered_loot());
			dog_object["score"] = dog->get_score();
			players_object[std::to_string(player->get_id())] = dog_object;
	  	}

	   return players_object;
   }

   json::object SerializeLoots(const std::vector<LootInfo>& loots)
      {
   	   	   json::object loots_object;

   	   	   for(size_t i = 0; i < loots.size(); ++i)
   	   	   {
   	   		json::object loot_object;
   	   		json::array pos_ar;

	  		pos_ar.emplace_back(loots[i].x);
	  		pos_ar.emplace_back(loots[i].y);

	  		loot_object["pos"] = pos_ar;
   	   		loot_object["type"] = loots[i].type;
   	   		loots_object[std::to_string(i)] = loot_object;
   	   	   }

   	   	   return loots_object;
      }

   std::string GetPlayersDogInfoResponce(const std::vector<std::shared_ptr<Player>>& players, const std::vector<LootInfo>& loots)
   {
	   json::object resp_object;

	    resp_object["players"] = SerializePlayers(players);
	    resp_object["lostObjects"] = SerializeLoots(loots);
	  	return json::serialize(resp_object);
   }


    std::string GetMapListResponce(std::shared_ptr<Game> game)
    {
        json::array map_ar;
        for(const auto& map : game->get_maps())
        {
            json::object map_obj;
	
            map_obj[ "id" ] = map.second->id();
            map_obj[ "name" ] = map.second->name();
            map_ar.emplace_back(map_obj);
        }
 
        return json::serialize(map_ar);
    }


    std::string MakeRecordsResponce(std::shared_ptr<Game> game, int start, int max_items)
    {
    	json::array map_ar;
      for( const auto& record: game->get_retired_players(start, max_items))
        {
            json::object map_obj;

            map_obj[ "name" ] = record.name;
    	    map_obj[ "score" ] = record.score;
    	    map_obj[ "playTime" ] = convert_int_to_double(record.play_time);
    	    map_ar.emplace_back(map_obj);
        }

        return json::serialize(map_ar);
    }

    std::string GetMapContentResponce(std::shared_ptr<Game> game, const std::string& map_id)
    {
        return game->get_map_as_string(map_id);
    }
}  // namespace json_serializer
