#include "MapModel.h"
#include "Exceptions.h"
#include "DataTypes.h"

Road::Road(RoadType road_type, const Point& start, const Coord& end)
{
    if(road_type == RoadType::HORIZONTAL)
    {
        this->start = start;
        this->end = Point{end, start.y};
    }
    else 
        if(road_type == RoadType::VERTICAL)
        {
            this->start = start;
            this->end = Point{start.x, end};
        }
        else
            throw UnknownRoadOrientation();
}

Map::Map(const std::string& id, const std::string& name)
{
    __id = id;
    __name = name;
    __dog_speed = 0.0;
    __bag_capacity = 1;
}

Point correct_dog_position_inside_road(const Point& position, const RoadBoundRect& bound_rect)
{
    Point pos(position);
    if(pos.x < bound_rect.min_point.x)
        pos.x = bound_rect.min_point.x;

    if(pos.x > bound_rect.max_point.x)
        pos.x = bound_rect.max_point.x;

    if(pos.y < bound_rect.min_point.y)
        pos.y = bound_rect.min_point.y;

    if(pos.y > bound_rect.max_point.y)
        pos.y = bound_rect.max_point.y;

    return pos;
}

bool roads_crossed(const Road& road1, const Road& road2)
{
    if ((road1.is_horizontal() && road2.is_vertical()) || (road1.is_vertical() && road2.is_horizontal()))
    {
        const auto& first_start = road1.get_start();
        const auto& second_start = road2.get_start();
        const auto& second_end = road2.get_end();
    
        if(road1.is_horizontal())
        {
            if (((second_start.y < first_start.y) && (second_end.y < first_start.y)) ||
                ((second_start.y > first_start.y) && (second_end.y > first_start.y)))
                return false;
        }
        else
        {
            if(road1.is_vertical())
                if (((second_start.x < first_start.x) and (second_end.x < first_start.x)) or
                    ((second_start.x > first_start.x) and (second_end.x > first_start.x)))
                    return false;
        }
        return true;
    }
    return false;
}

bool roads_adjacent(const Road& road1, const Road& road2)
{
    if((road1.is_horizontal() && road2.is_horizontal()) || (road1.is_vertical() && road2.is_vertical()))
    {
        const auto& first_start = road1.get_start();
        const auto& first_end = road1.get_end();

        const auto& second_start = road2.get_start();
        const auto& second_end = road2.get_end();

        if ((first_start == second_start) || (first_start == second_end) ||
            (first_end == second_start) || (first_end == second_end))
            return true;
    }
    return false;
}