#pragma once
#include "DataTypes.h"
#include <vector>
#include <string>

using namespace std;

class Road
{
public:
    Road(RoadType road_type, const Point& start, const Coord& end);

    bool is_horizontal() const
    { return start.y == end.y; }

    bool is_vertical() const
    { return start.x == end.x; } 

    const Point& get_start() const
    { return start; }

    const Point& get_end() const
    { return end; }

private:
    Point start;
    Point end;
};

using Roads = vector<Road>;

class Map
{
public:    
    Map(const std::string& id_, const std::string& name);
    // Map() = default;

    void set_id(const std::string& id_)
    {__id = id_;}

    void set_name(const std::string& name)
    {__name = name;}

    const std::string& id()
        { return __id; }

    std::string name()
        { return __name; }

    const std::vector<Road>& roads()
        { return __roads; }

    int get_num_roads()
        { return __roads.size(); }

    const std::vector<Office>& offices()
        { return __offices; }

    const std::vector<Loot>& loots()
        { return __loots; }

    void add_road(const Road& road)
        { __roads.push_back(road); }

    void add_office(const Office& office)
        { __offices.push_back(office); }

    void add_loot(Loot loot)
        { __loots.push_back(loot); }

    int get_num_loots()
        { return __loots.size(); }

    void set_dog_speed(float speed)
     { __dog_speed = speed; }

    float dog_speed()
    { return __dog_speed; }

    void set_bag_capacity(int capacity)
     { __bag_capacity = capacity; }

    int bag_capacity()
    { return __bag_capacity; }

private:
    std::string __id;
    std::string __name;
    std::vector<Road>__roads;
    std::vector<Office> __offices;
    std::vector<Loot> __loots;
    float __dog_speed{};
    int __bag_capacity{};
};

Point correct_dog_position_inside_road(const Point& pos, const RoadBoundRect& bound_rect);
bool roads_crossed(const Road& road1, const Road& road2);
bool roads_adjacent(const Road& road1, const Road& road2);