#pragma once
#include "MapModel.h"
#include "DataTypes.h"
#include <vector>

enum class ItemType:int
{
    Loot = 1,
    Office = 2
};

struct CollectionResult
{
    CollectionResult(float sq_distance, float proj_ratio)
    {
        this->sq_distance = sq_distance; 
        this->proj_ratio = proj_ratio;  
    }
    
    bool is_collected(float collect_radius)
    {
        return ((proj_ratio >= 0 && proj_ratio <= 1) && (sq_distance <= collect_radius * collect_radius));
    }

    float sq_distance{}; //квадрат расстояния до точки
    float proj_ratio{}; //доля пройденного отрезка
};

struct Item
{
    Item(int id, Point pos_, float width_, ItemType type_ = ItemType::Loot)
    {

        id_ = id;
        type_ = type_;
        position = pos_;
        width = width_;
    }

    int id_;
    Point position; 
    float width; 
    ItemType type_{ItemType::Loot};
};

using ItemList = vector<Item>;


struct Gatherer
{
    Point start_pos;
    Point end_pos;
    float width;
};

struct GatheringEvent
{
    int item_id{};
    int gatherer_id{};
    float sq_distance{};
    float time{};
};

class ItemGathererProvider
{
public:    
    virtual int items_count() const = 0;
    virtual Item get_item(int idx) const = 0;
    virtual int gatherers_count() const = 0;
    virtual Gatherer get_gatherer(int idx) const = 0;
};

class ItemGatherer: public ItemGathererProvider
{
public:    
    ItemGatherer(std::vector<Item> items, const std::vector<Gatherer>& gatherers)
    {
        items_ = std::move(items);
        gatherers_ = gatherers;
    }

    virtual int items_count() const override
    {
        return items_.size();
    }

    virtual Item get_item(int idx) const override
    {
        return items_[idx];
    }

    virtual int gatherers_count() const override
    {
        return gatherers_.size();
    }

    virtual Gatherer get_gatherer(int idx) const override
    {
        return gatherers_[idx];
    }

private:
    std::vector<Item> items_; 
    std::vector<Gatherer> gatherers_;
};

CollectionResult TryCollectPoint(const Point& a, const Point& b, const Point& c);
std::vector<GatheringEvent> find_gather_events(const ItemGathererProvider& provider);