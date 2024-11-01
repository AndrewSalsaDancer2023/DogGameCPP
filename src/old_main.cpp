#include <iostream>
// #include "DataTypes.h"
// #include "MapModel.h"
// #include "Utils.h"
// #include "CollisionDetector.h"
// #include "Exceptions.h"
#include "MapParser.h"
#include "Game.h"
#include <boost/json.hpp>
#include <filesystem>
#include <string>

int main()
{
 try {
        std::filesystem::path pth("./config.json");
        auto json_value = ReadJson(pth);
        auto stor = std::make_shared<MapStorage>();
        stor->parse_maps(json_value);
        auto game = std::make_unique<Game>(stor, false);
 }

    catch(std::exception& ex)
    {
    	std::cout << ex.what() << std::endl;
    }
}
