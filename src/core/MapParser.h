#pragma once
#include <filesystem>
#include <map>
#include <boost/json.hpp>
#include "DataTypes.h"
#include "MapModel.h"
#include <memory>

using Maps = std::map<std::string, std::shared_ptr<Map>>;
using MapJSON = std::map<std::string, std::string>;
 
class MapStorage
{
public:    
    void parse_maps(const boost::json::value& config_content);
    std::shared_ptr<Map> get_map(const std::string& map_name)
        { return maps.at(map_name); }
    const Maps& get_maps() const
        { return maps; }
    std::string get_map_as_string(const std::string& map_id)
        { return maps_json.at(map_id); }
    std::shared_ptr<Map> parse_map(const boost::json::object& map_representation);

    float get_dog_retirement_time()
    { return defaultDogRetirementTime; }

    int get_default_bag_capacity()
    { return defaultBagCapacity; }

    float get_default_dog_speed()
    { return defaultDogSpeed; }

    const LootConfig& get_loot_config()
    { return loot_config; }

private:
    Maps maps;
    MapJSON maps_json;
    LootConfig loot_config;
    float defaultDogRetirementTime = 0.0f;
    float defaultDogSpeed = 1.0f;
    int defaultBagCapacity = 3;    
};

boost::json::value ReadJson(const std::filesystem::path& json_path);
 Road ParseRoad(const boost::json::object& road_object);
 Office ParseOffice(const boost::json::object& office_object);
 Loot ParseLoot(const boost::json::object& loot_object);