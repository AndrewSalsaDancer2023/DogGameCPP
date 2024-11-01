#pragma once
#include <vector>
#include <string>
#include <map>
#include <cmath>

using namespace std;
constexpr float delta = 0.00001f;

struct Point
{
    float x{};
    float y{};

    // Point(float x, float y)
    // :x(x), y(y)
    // {}
    // Point() :x{0.0f}, y{0.0f} {}
    // bool operator == (const Point& other)
    //     { return ((std::abs(x - other.x) <= delta) && (std::abs(y - other.y) <= delta)); }

};

bool operator == (const Point& p1, const Point& p2);

struct Offset
{
    float dx{};
    float dy{};

    // Offset(float x, float y)
    // :dx(x), dy(y)
    // {}
};
 
struct LootInfo
 {
    int id{};
    int type{};
    float x{};
    float y{};
 };

using LootList = std::vector<LootInfo>;

using Dimension = float;
using Coord = Dimension;

enum class RoadType: int
{
    HORIZONTAL,
    VERTICAL
};

enum class RoadMutualPositionType: int
{
    Parallel,
    Adjacent,
    Crossed
};

struct RoadInfo
{
    unsigned road_index{};
    RoadMutualPositionType road_type{RoadMutualPositionType::Parallel};
};

using AdjacentRoads = std::vector<std::vector<RoadMutualPositionType>>;

struct Office
{
    std::string id;
    Point position;
    Offset offset;
};

struct Loot
{
    std::string name;
    std::string file;
    std::string type;
    int rotation{};
    std::string color;
    float scale{};
    int score{};
};

struct GatheredLootItem
{
    int id_{};
    int type_{};
};

using GatheredLootList = std::vector<GatheredLootItem>;

struct DogState
{
    Point pos_;
    std::string dir_;
    int score{};

    int idle_time{};
    int play_time{};
    int cur_road{};
    GatheredLootList bag;
};

struct PlayerState
{
    int id_{};
    std::string name;
    std::string token;
    DogState dog;
};

using PlayerStateList = std::vector<PlayerState>;

struct PlayerRecordItem
{
	std::string name;
	int score{};
	int play_time{};
};

struct SessionInfo
{
    std::string map_id;
    PlayerStateList players;
    LootList loots_info_state;
};

using GameState = std::vector<SessionInfo>;

struct DogSpeed
{
    float vx{};
    float vy{};
};

struct RoadBoundRect
{
    Point min_point;
    Point max_point;

    bool point_inside(const Point& new_pos)
    {
        if ((new_pos.x >= min_point.x) && (new_pos.x <= max_point.x) &&
            (new_pos.y >= min_point.y) && (new_pos.y <= max_point.y))
            return true;

        return false;
    }
};

enum class DogDirection
{
    NORTH,
    SOUTH,
    WEST,
    EAST,
    STOP
};

std::string string_from_direction(DogDirection direction);

DogDirection direction_from_string(const std::string& direction);

using LootConfig = std::map<std::string, float>;