#include "game_state_serialization.h"

#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <fstream>
#include <filesystem>

using InputArchive = boost::archive::text_iarchive;
using OutputArchive = boost::archive::text_oarchive;

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, GatheredLootItem& loots, const unsigned int version)
{
    ar & loots.id_;
    ar & loots.type_;
}

template<class Archive>
void serialize(Archive & ar, Point& position, const unsigned int version)
{
    ar & position.x;
    ar & position.y;
}

template<class Archive>
void serialize(Archive & ar, DogState& dog_state, const unsigned int version)
{
    ar & dog_state.pos_;
    ar & dog_state.dir_;
    ar & dog_state.score;

    ar & dog_state.idle_time;
    ar & dog_state.play_time;
    ar & dog_state.cur_road;

    ar & dog_state.bag;
}

template<class Archive>
void serialize(Archive & ar, PlayerState& player_state, const unsigned int version)
{
    ar & player_state.id_;
    ar & player_state.name;
    ar & player_state.token;
    ar & player_state.dog;
}

///////////////////////////////////////////////////////////////////////////////////
template<class Archive>
void serialize(Archive & ar, SessionInfo& session_info, const unsigned int version)
{
    ar & session_info.map_id;
    ar & session_info.players;
    ar & session_info.loots_info_state;
}

template<class Archive>
void serialize(Archive & ar, GameState& state, const unsigned int version)
{
    ar & state;
}

} // namespace serialization
} // namespace boost


void serialize_game_state(const GameState& state, const std::string& path)
{
    std::ofstream ofs(path);
    boost::archive::text_oarchive oa(ofs);
    // write class instance to archive
    oa << state; 
}

GameState deserialize_game_state(const std::string& path)
{
	GameState state;

    if(std::filesystem::exists(path))
    {
        std::ifstream ifs(path);
        InputArchive ia{ifs};
        ia >> state;
    }

	return state;
}

/*
void SerializeGameSession(const model::GameSession& session)
{
	 std::stringstream ss;
	 OutputArchive oa{ss};

	 oa << session.GetState();
	 std::ofstream ofs("filename.arch");
	 ofs << ss.str();
}

model::GameSessionState DeserializeGameSession()
{
	std::stringstream ss;
	InputArchive ia{ss};

	model::GameSessionState state;

	ia >> state;
	return state;
}

*/