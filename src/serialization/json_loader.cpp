#include "json_loader.h"
#include <fstream>
#include <boost/json.hpp>
#include "../core/Exceptions.h"
#include <iostream>

std::string userName_ = "userName";
std::string mapId_ = "mapId";
std::string move_key_ = "move";
std::string period_ = "period";
std::string timeDelta_ = "timeDelta";
boost::json::string directionRight_ = "R";
boost::json::string directionLeft_ = "L";
boost::json::string directionDown_ = "D";
boost::json::string directionUp_ = "U";

namespace json = boost::json;
namespace json_loader {
    std::map<std::string, std::string> ParseJoinGameRequest(const std::string& body)
    {
       std::map<std::string, std::string> result;
    
        auto value = json::parse(body);
   
        result[userName_] = value.at(userName_).as_string();
        result[mapId_] = value.at(mapId_).as_string();
	
        return result;
    }
 
    DogDirection GetMoveDirection(const std::string& body)
    {
      	auto value = json::parse(body);

       	auto direction = value.at(move_key_).as_string();

       	const auto getDirection = [](const boost::json::string& direct) -> DogDirection
        				      {
       								std::map<boost::json::string, DogDirection> dir{ {directionRight_, DogDirection::EAST},
       																						{directionLeft_, DogDirection::WEST,},
																							{directionDown_, DogDirection::SOUTH},
																							{directionUp_, DogDirection::NORTH},
																							{"", DogDirection::STOP}};
       								auto itFind = dir.find(direct);
       								return itFind->second;
        				      };

        return getDirection(direction);
    }

    int ParseDeltaTimeRequest(const std::string& body)
    {
       int result{};
       try
	   {
    	   auto json_object = json::parse(body).as_object();
    	   if(!json_object.contains(timeDelta_))
    		   throw BadDeltaTimeException();

    	   auto delta = json_object.at(timeDelta_).as_int64();
    	   result = static_cast<int>(delta);
    	   return result;
	   }
       catch (const std::exception& ex)
	     {
	            throw BadDeltaTimeException();
	     }

    }

}  // namespace json_loader
