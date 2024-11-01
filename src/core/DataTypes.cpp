#include "DataTypes.h"

bool operator == (const Point& p1, const Point& p2)
    { return ((std::abs(p1.x - p2.x) <= delta) && (std::abs(p1.y - p2.y) <= delta)); }

std::string string_from_direction(DogDirection direction)
{
    std::map<DogDirection, std::string> conv_map{
        {DogDirection::NORTH, "U"},
        {DogDirection::SOUTH, "D"},
        {DogDirection::WEST, "L"},
        {DogDirection::EAST, "R"},
        {DogDirection::STOP, ""}};

    return conv_map.at(direction);
}

DogDirection direction_from_string(const std::string& direction)
{
     std::map<std::string, DogDirection> conv_map{
        {"U", DogDirection::NORTH},
        {"D", DogDirection::SOUTH},
        {"L", DogDirection::WEST},
        {"R", DogDirection::EAST},
        {"", DogDirection::STOP}};

    return conv_map.at(direction);
}