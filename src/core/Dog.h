#pragma once
#include <vector>
#include <optional>
#include <string>
#include <set>
#include <memory>
#include "DataTypes.h"
#include "MapModel.h"
#include "CollisionDetector.h"

// class Map;
// enum class DogDirection;
// class Gatherer;
// struct Point;
// struct LootList;
// struct LootInfo;

using namespace std;

class Dog {
public:    
    Dog(shared_ptr<Map> map, int default_bag_capacity);
    void set_speed(DogDirection dir_, float speed);
    void spawn_dog_in_map(bool spawn_in_random_point);
    std::optional<Gatherer> move(int delta_time);
    DogState get_state();
    DogDirection get_direction()
        { return direction_; }

    void set_position(const Point& position)
        { position_ = position;}

    Point get_position()
        { return position_;}

    DogSpeed get_speed()
        { return speed_; }
    
    LootList get_gathered_loot()
        { return gathered_loots_;}

    void append_gathered_loot(LootInfo loot)
        { gathered_loots_.push_back(loot); }

    int get_idle_time()
        { return idle_time_; }

    void add_idle_time(int delta)
        { idle_time_ += delta; }

    void set_idle_time(int time)
        { idle_time_ = time; }

    int get_road_index()
        { return current_road_index_; }

    void set_road_index(int road_index)
        { current_road_index_ = road_index; }

    bool add_loot(LootInfo loot);

    void pass_loot_to_office();

    void set_score(int score)
        {  score_ = score; }

    int get_score()
        { return score_; }

    int get_play_time()
        { return play_time_;}

    void add_play_time(int delta)
        { play_time_ += delta; }

    void set_play_time(int play_time)
        { play_time_ = play_time; }

    int get_bag_capacity()
        { return bag_capacity_; }

    void set_direction(DogDirection dir)
        { direction_ = dir;}

private:    
    void find_adjacent_roads();
    void set_start_position_first_road();
    void set_start_position_random_road();
    RoadBoundRect get_current_road_bound_rect(int road_index);
    std::optional<int> search_position_inside_adjacent_roads(Point pos);
    Point evaluate_new_position(int delta_time);
    void change_position(int delta_time);

    vector< set<int> > adjacent_roads_;
    shared_ptr<Map> map_;
    Roads roads_;
    DogDirection direction_;
    Point position_;
    DogSpeed speed_;
    LootList gathered_loots_;
    int idle_time_{};
    int current_road_index_{};
    int score_{};
    int play_time_{};
    int bag_capacity_ = 3;
};

class Player {
public:    
    Player(int id, const string& name, const string& token, shared_ptr<Dog> dog)
    {
        this->id = id;
        this->name = name;
        this->token = token;
        this->dog = dog;
    }

    string GetToken()
        { return token;}

    const string& GetName() const
        { return name;}

    int get_id()
        { return id;}

    shared_ptr<Dog> GetDog()
    {return dog;}

    void set_token(string token)
    { this->token = token; }

    
    void set_name(string name)
    { this->name = name; }

    void set_id(int id)
     { this->id = id; }


    PlayerState get_state() const
    {
        return PlayerState{id, name, token, dog->get_state()};
        // return PlayerState(id, name, token, dog->get_state());
    }


private:
    shared_ptr<Dog> dog;
    string token;
    string name;
    int id;

};

using PlayersList = vector<shared_ptr<Player>>;