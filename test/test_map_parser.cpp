#include <gtest/gtest.h>
#include <filesystem>
#include <memory>
#include "./utils/test_utilities.h"
#include "../src/core/MapParser.h"


TEST(ReadJsonTestSuite, TryReadFileOnAbsentPath) {

    std::filesystem::path path1("./dir/absent_file1.json");
    std::filesystem::path path2("./dir/absent_file12.json");
    EXPECT_THROW(ReadJson(path1), std::filesystem::filesystem_error);
    EXPECT_THROW(ReadJson(path2), std::filesystem::filesystem_error);
}

TEST(ParseRoadTestSuite, TryParseHorizontalRoad) {

    std::string content("{ \"x0\": 0, \"y0\": 0, \"x1\": 40}");
    auto road_object = test_utils::convertToJSonObject(content);
    Road road = ParseRoad(road_object);
    
    ASSERT_FLOAT_EQ(road.get_start().x, 0.0);
    ASSERT_FLOAT_EQ(road.get_start().y, 0.0);

    ASSERT_FLOAT_EQ(road.get_end().x, 40.0);
    ASSERT_FLOAT_EQ(road.get_end().y, 0.0);

    ASSERT_TRUE(road.is_horizontal());
    ASSERT_FALSE(road.is_vertical());
}

TEST(ParseRoadTestSuite, TryParseHorizontalRoadAnotherCoords) {

    std::string content("{ \"x0\": -1000, \"y0\": -200, \"x1\": 140}");
    auto road_object = test_utils::convertToJSonObject(content);
    Road road = ParseRoad(road_object);
    
    ASSERT_FLOAT_EQ(road.get_start().x, -1000.0);
    ASSERT_FLOAT_EQ(road.get_start().y, -200.0);

    ASSERT_FLOAT_EQ(road.get_end().x, 140.0);
    ASSERT_FLOAT_EQ(road.get_end().y, -200.0);

    ASSERT_TRUE(road.is_horizontal());
    ASSERT_FALSE(road.is_vertical());
}

TEST(ParseRoadTestSuite, TryParseVerticalRoad) {

    std::string content("{ \"x0\": 40, \"y0\": 0, \"y1\": 30}");
    auto road_object = test_utils::convertToJSonObject(content);
    Road road = ParseRoad(road_object);
    
    ASSERT_FLOAT_EQ(road.get_start().x, 40.0);
    ASSERT_FLOAT_EQ(road.get_start().y, 0.0);

    ASSERT_FLOAT_EQ(road.get_end().x, 40.0);
    ASSERT_FLOAT_EQ(road.get_end().y, 30.0);

    ASSERT_FALSE(road.is_horizontal());
    ASSERT_TRUE(road.is_vertical());
}

TEST(ParseRoadTestSuite, TryParseVerticalRoadAnotherCoords) {
    
    std::string content("{ \"x0\": -40, \"y0\": 130, \"y1\": 1345}");
    auto road_object = test_utils::convertToJSonObject(content);
    Road road = ParseRoad(road_object);
    
    ASSERT_FLOAT_EQ(road.get_start().x, -40.0);
    ASSERT_FLOAT_EQ(road.get_start().y, 130.0);

    ASSERT_FLOAT_EQ(road.get_end().x, -40.0);
    ASSERT_FLOAT_EQ(road.get_end().y, 1345.0);

    ASSERT_FALSE(road.is_horizontal());
    ASSERT_TRUE(road.is_vertical());
}

TEST(ParseOfficeTestSuite, ParseOffice) {
    std::string content("{ \"id\": \"office 1\", \"x\": 40, \"y\": 30, \"offsetX\": 5, \"offsetY\": 0 }");
    auto office_object = test_utils::convertToJSonObject(content);
    Office office = ParseOffice(office_object);    

    ASSERT_EQ(office.id, "office 1");
    ASSERT_FLOAT_EQ(office.position.x, 40.0);
    ASSERT_FLOAT_EQ(office.position.y, 30.0);

    ASSERT_FLOAT_EQ(office.offset.dx, 5.0);
    ASSERT_FLOAT_EQ(office.offset.dy, 0.0);
}

TEST(ParseOfficeTestSuite, ParseAnotherOffice) {
    std::string content("{ \"id\": \"office 2\", \"x\": -40, \"y\": -130, \"offsetX\": -5, \"offsetY\": -10 }");
    auto office_object = test_utils::convertToJSonObject(content);
    Office office = ParseOffice(office_object);    

    ASSERT_EQ(office.id, "office 2");
    ASSERT_FLOAT_EQ(office.position.x, -40.0);
    ASSERT_FLOAT_EQ(office.position.y, -130.0);

    ASSERT_FLOAT_EQ(office.offset.dx, -5.0);
    ASSERT_FLOAT_EQ(office.offset.dy, -10.0);
}

TEST(ParseLootTestSuite, ParseLoot) {
    std::string content("{\"name\": \"key\", \"file\": \"assets/key.obj\", \"type\": \"obj\", \"rotation\": 90, \n\
                            \"color\": \"#338844\", \"scale\": 0.03, \"value\": 10 }");
    auto loot_object = test_utils::convertToJSonObject(content);

    Loot loot = ParseLoot(loot_object);
    ASSERT_EQ(loot.name, "key");
    ASSERT_EQ(loot.file, "assets/key.obj");
    ASSERT_EQ(loot.type, "obj");
    ASSERT_EQ(loot.rotation, 90);
    ASSERT_EQ(loot.color, "#338844");
    ASSERT_FLOAT_EQ(loot.scale, 0.03);
    ASSERT_EQ(loot.score, 10);
}

TEST(ParseLootTestSuite, ParseAnotherLoot) {
    std::string content("{\"name\": \"key123\", \"file\": \"assetsdew/keye3r.obj\", \"type\": \"obj98\", \"rotation\": 180, \n\
                            \"color\": \"#AABBCC\", \"scale\": 1.56, \"value\": 120 }");
    auto loot_object = test_utils::convertToJSonObject(content);

    Loot loot = ParseLoot(loot_object);
    ASSERT_EQ(loot.name, "key123");
    ASSERT_EQ(loot.file, "assetsdew/keye3r.obj");
    ASSERT_EQ(loot.type, "obj98");
    ASSERT_EQ(loot.rotation, 180);
    ASSERT_EQ(loot.color, "#AABBCC");
    ASSERT_FLOAT_EQ(loot.scale, 1.56);
    ASSERT_EQ(loot.score, 120);
}

TEST(CreateMapTestSuite, CreateMapDefault) {
    Map map{"map1", "Map 1"};
    ASSERT_EQ(map.id(), "map1");
    ASSERT_EQ(map.name(), "Map 1");
    ASSERT_EQ(map.bag_capacity(), 1);
    ASSERT_FLOAT_EQ(map.dog_speed(), 0.0);
    ASSERT_TRUE(map.roads().empty());
    ASSERT_TRUE(map.offices().empty());    
    ASSERT_TRUE(map.loots().empty());
}

TEST(CreateMapTestSuite, CreateMapOtherNameId) {
    Map map{"New10", "New Map"};
    ASSERT_EQ(map.id(), "New10");
    ASSERT_EQ(map.name(), "New Map");
}

TEST(CreateMapTestSuite, AddRoads) {
    Map map{"map1", "Map 1"};
    Road road{RoadType::HORIZONTAL, {0.0, 0.0}, 10.0};
    map.add_road(road);
    Road road2{RoadType::VERTICAL, {10.0, 10.0}, 20.0};
    map.add_road(road2);
    ASSERT_EQ(map.get_num_roads(), 2);
    Road road3{RoadType::HORIZONTAL, {10.0, 20.0}, 30.0};
    map.add_road(road3);
    ASSERT_EQ(map.get_num_roads(), 3);
}

TEST(CreateMapTestSuite, AddOffices) {
    Map map{"map1", "Map 1"};
    Office offise{"office1", {0.0, 0.0}, {10.0, 10.0}};
    map.add_office(offise);
    ASSERT_EQ(map.offices().size(), 1);

    Office offise2{"office2", {10.0, 15.0}, {20.0, 30.0}};
    map.add_office(offise2);
    ASSERT_EQ(map.offices().size(), 2);

    Office offise3{"office3", {2340.0, 3245.0}, {20.0, 50.0}};
    map.add_office(offise3);
    ASSERT_EQ(map.offices().size(), 3);
}

TEST(CreateMapTestSuite, AddLoots) {
    Map map{"map1", "Map 1"};
    Loot loot1, loot2, loot3;
    map.add_loot(loot1);
    ASSERT_EQ(map.loots().size(), 1);
    map.add_loot(loot2);
    ASSERT_EQ(map.loots().size(), 2);    
    map.add_loot(loot3);
    ASSERT_EQ(map.loots().size(), 3);
}

TEST(CreateMapTestSuite, NameIdSettings) {
    Map map{"map1", "Map 1"};
    map.set_id("id1");
    map.set_name("name1");

    ASSERT_EQ(map.id(), "id1");
    ASSERT_EQ(map.name(), "name1");
}

TEST(CreateMapTestSuite, OtherNameIdSettings) {
    Map map{"map1", "Map 1"};
    map.set_id("new id");
    map.set_name("new name");

    ASSERT_EQ(map.id(), "new id");
    ASSERT_EQ(map.name(), "new name");
}

TEST(ParseMapStorageTestSuite, CreateMapStorage) {
    MapStorage storage;
    ASSERT_TRUE(storage.get_maps().empty());
    ASSERT_FLOAT_EQ(storage.get_dog_retirement_time(), 0.0);
    ASSERT_EQ(storage.get_default_bag_capacity(), 3);
    ASSERT_FLOAT_EQ(storage.get_default_dog_speed(), 1.0);
}


TEST(ParseMapTestSuite, TryParseMap) {

    std::string content("{\"dogSpeed\": 4.0, \"id\": \"map1\", \"name\": \"Map 1\", \n\
    \"lootTypes\": [ \n\
    { \"name\": \"key\", \"file\": \"assets/key.obj\", \"type\": \"obj\", \"rotation\": 90, \n\
      \"color\" : \"#338844\", \"scale\": 0.03, \"value\": 10 }, \n\
    { \"name\": \"wallet\", \"file\": \"assets/wallet.obj\", \"type\": \"obj\", \"rotation\": 0, \n\
      \"color\" : \"#883344\", \"scale\": 0.05, \"value\": 50 }], \n\
      \"roads\": [ { \"x0\": 0, \"y0\": 0, \"x1\": 40}, { \"x0\": 40, \"y0\": 0, \"y1\": 30} ], \n\
      \"offices\": [ { \"id\": \"o0\", \"x\": 140, \"y\": 130, \"offsetX\": 15, \"offsetY\": 10}]}");

    auto map_value = test_utils::convertToJSonObject(content);
    MapStorage storage;
    auto map = storage.parse_map(map_value);

    ASSERT_EQ(map->id(), "map1");
    ASSERT_EQ(map->name(), "Map 1");
    ASSERT_FLOAT_EQ(map->dog_speed(), 4.0);
    ASSERT_EQ(map->loots().size(), 2);

    const auto& loots = map->loots();

    ASSERT_EQ(loots[0].name, "key");
    ASSERT_EQ(loots[0].file, "assets/key.obj");
    ASSERT_EQ(loots[0].type, "obj");
    ASSERT_EQ(loots[0].rotation, 90);
    ASSERT_EQ(loots[0].color, "#338844");
    ASSERT_FLOAT_EQ(loots[0].scale, 0.03);
    ASSERT_EQ(loots[0].score, 10);

    ASSERT_EQ(loots[1].name, "wallet");
    ASSERT_EQ(loots[1].file, "assets/wallet.obj");
    ASSERT_EQ(loots[1].type, "obj");
    ASSERT_EQ(loots[1].rotation, 0);
    ASSERT_EQ(loots[1].color, "#883344");
    ASSERT_FLOAT_EQ(loots[1].scale, 0.05);
    ASSERT_EQ(loots[1].score, 50);

    ASSERT_EQ(map->roads().size(), 2);
    const auto& roads = map->roads();
    ASSERT_FLOAT_EQ(roads[0].get_start().x, 0.0);
    ASSERT_FLOAT_EQ(roads[0].get_start().y, 0.0);

    ASSERT_FLOAT_EQ(roads[0].get_end().x, 40.0);
    ASSERT_FLOAT_EQ(roads[0].get_end().y, 0.0);

    ASSERT_TRUE(roads[0].is_horizontal());
    ASSERT_FALSE(roads[0].is_vertical());

    ASSERT_FLOAT_EQ(roads[1].get_start().x, 40.0);
    ASSERT_FLOAT_EQ(roads[1].get_start().y, 0.0);

    ASSERT_FLOAT_EQ(roads[1].get_end().x, 40.0);
    ASSERT_FLOAT_EQ(roads[1].get_end().y, 30.0);

    ASSERT_FALSE(roads[1].is_horizontal());
    ASSERT_TRUE(roads[1].is_vertical());

    ASSERT_EQ(map->offices().size(), 1);

    const auto& offices = map->offices();
    ASSERT_EQ(offices[0].id, "o0");
    ASSERT_FLOAT_EQ(offices[0].position.x, 140.0);
    ASSERT_FLOAT_EQ(offices[0].position.y, 130.0);

    ASSERT_FLOAT_EQ(offices[0].offset.dx, 15.0);
    ASSERT_FLOAT_EQ(offices[0].offset.dy, 10.0);
}

TEST(ParseConfigTestSuite, TryParseConfig) {

    std::filesystem::path pth("./data/config.json");
    auto json_value = ReadJson(pth);
    auto stor = std::make_shared<MapStorage>();
    stor->parse_maps(json_value);

    LootConfig loot_config{stor->get_loot_config()};
    ASSERT_FLOAT_EQ(loot_config["period"], 5.0);
    ASSERT_FLOAT_EQ(loot_config["probability"], 0.5);
    ASSERT_FLOAT_EQ(stor->get_default_dog_speed(), 3.0);
    ASSERT_FLOAT_EQ(stor->get_dog_retirement_time(), 15.0);
    ASSERT_EQ(stor->get_maps().size(), 3);
    auto map = stor->get_map("map3");
    
    ASSERT_FLOAT_EQ(map->dog_speed(), 3.0);
    ASSERT_EQ(map->loots().size(), 2);
    ASSERT_EQ(map->roads().size(), 9);
    ASSERT_EQ(map->offices().size(), 1);
}

