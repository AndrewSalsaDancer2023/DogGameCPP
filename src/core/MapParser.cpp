#include "MapParser.h"
#include <fstream>
#include <iostream>

std::string speedKey = "dogSpeed";
std::string defaultDogSpeedKey = "defaultDogSpeed";
std::string lootConfigKey = "lootGeneratorConfig";
std::string default_Dog_Retirement = "dogRetirementTime";
std::string default_bag_capacity = "defaultBagCapacity";

std::string x = "x";
std::string y = "y";
std::string w = "w";
std::string h = "h";
std::string id = "id";
std::string offsetX = "offsetX";
std::string offsetY = "offsetY";

std::string x0 = "x0";
std::string y_0 = "y0";
std::string x1 = "x1";
std::string y_1 = "y1";
std::string timeDelta = "timeDelta";
std::string userName = "userName";
std::string mapId = "mapId";

std::string name = "name";
std::string file = "file";
std::string type = "type";
std::string rotation = "rotation";
std::string color = "color";
std::string scale = "scale";
std::string value = "value";
std::string dog_Speed = "dogSpeed";
std::string bag_Capacity = "bagCapacity";
std::string roads_key = "roads";
std::string buildings_key = "buildings";
std::string offices_key = "offices";
std::string lootTypes = "lootTypes";
std::string default_Dog_Speed = "defaultDogSpeed";
std::string lootGeneratorConfig = "lootGeneratorConfig";
std::string probability = "probability";
std::string bagCapacityDefault = "defaultBagCapacity";
std::string maps_key = "maps";
std::string move_key = "move";
std::string period = "period";

std::string periodConfigKey = "period";
std::string probabilityConfigKey = "probability";

boost::json::value ReadJson(const std::filesystem::path& json_path)
{
    if(!std::filesystem::exists(json_path))
        throw	std::filesystem::filesystem_error(std::string("File not exists:") + json_path.c_str(), std::error_code());

    std::ifstream input(json_path);
    std::stringstream sstr;

    while(input >> sstr.rdbuf());
    return boost::json::parse(sstr.str());
}

    Road ParseRoad(const boost::json::object& road_object)
    {
    	if(road_object.contains("x1"))
    	{
    		Road road(RoadType::HORIZONTAL, Point{static_cast<float>(road_object.at(x0).as_int64()),
                                                 static_cast<float>(road_object.at(y_0).as_int64())},
                                            static_cast<float>(road_object.at(x1).as_int64()));
            return road;
        }

        Road road(RoadType::VERTICAL, Point{static_cast<float>(road_object.at(x0).as_int64()),
                                            static_cast<float>(road_object.at(y_0).as_int64())},
                                      static_cast<float>(road_object.at(y_1).as_int64()));
        return road;
    }

    Office ParseOffice(const boost::json::object& office_object)
    {
        Office office{std::string(office_object.at(id).as_string().data()),
        		      Point{static_cast<float>(office_object.at(x).as_int64()),
        		     	 	static_cast<float>(office_object.at(y).as_int64())},
					  Offset{static_cast<float>(office_object.at(offsetX).as_int64()),
							 static_cast<float>(office_object.at(offsetY).as_int64())}};
        return office;
    }

    Loot ParseLoot(const boost::json::object& loot_object)
    {
        Loot loot{std::string(loot_object.at(name).as_string().data()),
        		  std::string(loot_object.at(file).as_string().data()),
				  std::string(loot_object.at(type).as_string().data()),
				  loot_object.contains(rotation) ? static_cast<int>(loot_object.at(rotation).as_int64()) : -1,
				  loot_object.contains(color) ? std::string(loot_object.at(color).as_string().data()) : "",
				  static_cast<float>(loot_object.at(scale).as_double()),
				  loot_object.contains(value) ? static_cast<int>(loot_object.at(value).as_int64()) : 0};

        return loot;
    }

template <typename T>
std::vector<T> ParseObjects(const boost::json::object& object, const std::string& object_tag, 
        	       	         std::function<T(const boost::json::object& office_object)> parseFunc)
{
	std::vector<T> objects;
	
	auto objectsArray = object.at(object_tag).as_array();
	for(auto index = 0; index < objectsArray.size(); ++index)
	{
		auto curObject = objectsArray.at(index).as_object();
		objects.emplace_back(parseFunc(curObject));
	}
	
		return objects;
}

void MapStorage::parse_maps(const boost::json::value& config_content)
{
    boost::json::object object_to_read = config_content.as_object();

    if(object_to_read.contains(defaultDogSpeedKey))
        defaultDogSpeed  = static_cast<float>(object_to_read.at(defaultDogSpeedKey).as_double());

    if(object_to_read.contains(lootConfigKey))
    {
        const auto& config = object_to_read.at(lootConfigKey).as_object();
        loot_config[periodConfigKey] = static_cast<float>(config.at(periodConfigKey).as_double());
        loot_config[probabilityConfigKey] = static_cast<float>(config.at(probabilityConfigKey).as_double());
    }

    if(object_to_read.contains(default_Dog_Retirement))
        defaultDogRetirementTime = static_cast<float>(object_to_read.at(default_Dog_Retirement).as_double());
        
    if(object_to_read.contains(default_bag_capacity))
        defaultBagCapacity = static_cast<int>(object_to_read.at(default_bag_capacity).as_int64());

    auto map_array = object_to_read.at(maps_key).as_array();
    for(auto index = 0; index < map_array.size(); ++index)
        {
            const auto& curMap = map_array.at(index).as_object();
            std::string map_name(curMap.at(id).as_string().data());
            maps[map_name] = parse_map(curMap);
        }
}

std::shared_ptr<Map> MapStorage::parse_map(const boost::json::object& map_representation)
{   
    std::string map_id(map_representation.at(id).as_string().data());
    std::string map_name(map_representation.at(name).as_string().data());

    auto map_ = std::make_shared<Map>(map_id, map_name);

    if(map_representation.contains(dog_Speed))    
        map_->set_dog_speed(static_cast<float>(map_representation.at(dog_Speed).as_double()));

    if(map_representation.contains(bag_Capacity))
        map_->set_bag_capacity(static_cast<int>(map_representation.at(bag_Capacity).as_int64()));

    auto roads = ParseObjects<Road>(map_representation, roads_key, ParseRoad);
	for(const auto& road: roads)
		map_->add_road(road);
	
	auto offices = ParseObjects<Office>(map_representation, offices_key, ParseOffice);
	for(const auto& office: offices)
		map_->add_office(office);
	
	auto loots = ParseObjects<Loot>(map_representation, lootTypes, ParseLoot);
	for(const auto& loot: loots)
		map_->add_loot(loot);

    return map_;
}
