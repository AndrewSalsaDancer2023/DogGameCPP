#include "Dog.h"
#include "DataTypes.h"
#include <map>
#include <set>
#include <cstdlib>
#include <vector>
#include "Exceptions.h"
#include "Utils.h"

constexpr float dS = 0.4;
constexpr int millisecondsInSecond = 1000;
constexpr float gathererWidth = 0.6;
constexpr float epsilon = 0.0001;


Dog::Dog(shared_ptr<Map> map, int default_bag_capacity)
{
    if (!map_->bag_capacity())
        bag_capacity_ = map_->bag_capacity();
    else 
        bag_capacity_ = default_bag_capacity;

    map_ = map;
    roads_ = map_->roads();
    direction_ = DogDirection::NORTH;

    adjacent_roads_ = std::vector<std::set<int>>(roads_.size());
    find_adjacent_roads();
}

void Dog::set_speed(DogDirection direction, float speed)
{
    std::map<DogDirection, DogSpeed> dir{ {DogDirection::EAST, {speed, 0}},
    			   	   	   	   	   	   	   	     {DogDirection::WEST, {-speed, 0}},
    											 {DogDirection::SOUTH, {0, speed}},
    											 {DogDirection::NORTH, {0, -speed}},
                                                  { DogDirection::STOP, {}} };

    auto find_vel = dir.find(direction);
    if(find_vel == dir.end())
        throw DogSpeedException();

    if(direction != DogDirection::STOP)
        direction_ = direction;

    speed_ = find_vel->second;
}

void Dog::spawn_dog_in_map(bool spawn_in_random_point)
{
    if(spawn_in_random_point)
        set_start_position_random_road();
    else
        set_start_position_first_road();
}

void Dog::find_adjacent_roads()
{
    for(int i = 0; i < roads_.size(); ++i)
        for(int j = i+1; j < roads_.size(); ++j)
        {
            auto road_type = RoadMutualPositionType::Parallel;

            if(roads_adjacent(roads_[i], roads_[j]))
                road_type = RoadMutualPositionType::Adjacent;
            else
                if(roads_crossed(roads_[i], roads_[j]))
                    road_type = RoadMutualPositionType::Crossed;

            if(road_type != RoadMutualPositionType::Parallel)
            {
                adjacent_roads_[i].insert(j);
                adjacent_roads_[j].insert(i);
            }
        }
}

void Dog::set_start_position_first_road()
{
    current_road_index_ = 0;
    auto start = roads_[current_road_index_].get_start();
    position_ = start;
}

void Dog::set_start_position_random_road()
{
    current_road_index_ = utils::GetRandomIntNumber(0, roads_.size()-1);
    auto start = roads_[current_road_index_].get_start();
    auto end = roads_[current_road_index_].get_end();

    if(roads_[current_road_index_].is_horizontal())
    {
        if(start.x > end.x)
            std::swap(start, end);
            position_ = Point{utils::GetRandomFloatNumber(start.x, end.x), start.y};
    }
    else
    {
        if(start.y > end.y)
            std::swap(start, end);
        position_ = Point{start.x, utils::GetRandomFloatNumber(start.y, end.y)};
    }
}

RoadBoundRect Dog::get_current_road_bound_rect(int road_index)
{
    auto road = roads_[road_index];
    auto x_min = std::min(road.get_start().x, road.get_end().x);
    auto x_max = std::max(road.get_start().x, road.get_end().x);

    auto y_min = min(road.get_start().y, road.get_end().y);
    auto y_max = max(road.get_start().y, road.get_end().y);

    auto bound_rect = RoadBoundRect();
    bound_rect.min_point = Point{x_min - dS, y_min - dS};
    bound_rect.max_point = Point{x_max + dS, y_max + dS};
    return bound_rect;
}

std::optional<int> Dog::search_position_inside_adjacent_roads(Point pos)
{
    for(const auto& road_index : adjacent_roads_[current_road_index_])
    {
        auto bound_rect = get_current_road_bound_rect(road_index);
        if(bound_rect.point_inside(pos))
            return road_index;
    }

    return std::nullopt;
}

Point Dog::evaluate_new_position(int delta_time)
{
    auto dt = float(delta_time) / millisecondsInSecond;

    return Point{position_.x + dt * speed_.vx, 
                 position_.y + dt * speed_.vy};
}

void Dog::change_position(int delta_time)
{
    auto bound_rect = get_current_road_bound_rect(current_road_index_);
    auto new_pos = evaluate_new_position(delta_time);
    if(bound_rect.point_inside(new_pos))
    {
        position_ = new_pos;
        return;
    }
    else
    {
        auto adj_road_index = search_position_inside_adjacent_roads(new_pos);
        if(adj_road_index)
        {
            current_road_index_ = *adj_road_index;
            position_ = new_pos;
        }
        else
        {
            position_ = correct_dog_position_inside_road(new_pos, bound_rect);
            speed_.vx = 0.0;
            speed_.vy = 0.0;
        }
    }
}

std::optional<Gatherer> Dog::move(int delta_time)
{
    if(direction_ == DogDirection::STOP)
        return std::nullopt;

    const auto& start = get_position();
        
    change_position(delta_time);
    const auto& end = get_position();

    if ((std::abs(start.x-end.x) > epsilon) || (std::abs(start.y - end.y) > epsilon))
    {
        return Gatherer{{start.x, start.y}, {end.x, end.y}, gathererWidth};
    }

    return std::nullopt;
}

    
bool Dog::add_loot(LootInfo loot)
{
    if(gathered_loots_.size() >= bag_capacity_)
        return false;

    gathered_loots_.push_back(loot);
    return true;
}

void Dog::pass_loot_to_office()
{
    // const auto& loots = map_->loots;

    for(const auto& loot : gathered_loots_)
        score_ += map_->loots()[loot.type].score;
        // score_ += loots[loot.type].score;

    gathered_loots_.clear();
}

DogState Dog::get_state()
{
    std::vector<GatheredLootItem>  bag;

    for(const auto& loot : gathered_loots_)
        bag.push_back({loot.id, loot.type});

    return DogState{position_, string_from_direction(direction_), score_, idle_time_,
                    play_time_, current_road_index_, bag};
}

