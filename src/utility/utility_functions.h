#pragma once
#include <boost/program_options.hpp>
#include "tagged_uuid.h"
#include "../core/DataTypes.h"
#include "../db/postgres.h"
#include "../db/connection_engine.h"
#include <boost/url.hpp>
#include <iostream>

using namespace boost::urls;

struct Args {
    int tick_period{100};
    int save_period{10000};
    std::string config_file{"./data/config.json"};
    // std::string www_root;
    std::string save_file;
    bool spawn_random_points{false};
};

struct AppConfig {
    std::string db_url;
};
const int MAX_DB_RECORDS = 100;
// constexpr const char LEAVE_GAME_DB_URL_ENV_NAME[]{"GAME_DB_URL"};
// constexpr const char LEAVE_GAME_DB_URL_ENV_VALUE[]{"postgres://postgres:qazwsxedc@localhost:5432/leave_game_db"};

namespace {

[[nodiscard]] std::optional<Args> ParseCommandLine(int argc, const char* const argv[]) {
    namespace po = boost::program_options;

    po::options_description desc{"All options"s};
    Args args;
    std::string tick_period;
    std::string save_period;
    desc.add_options()           //
        ("help,h", "produce help message")  //
        ("tick-period,t", po::value(&tick_period)->value_name("milliseconds"s), " set tick period")  //
        ("config-file,c", po::value(&args.config_file)->value_name("file"s), "set config file path") //
        // ("www-root,w", po::value(&args.www_root)->value_name("dir"s), "set static files root") //        
		("randomize-spawn-points", "spawn dogs at random positions") //
		("state-file,f", po::value(&args.save_file)->value_name("file"s), "set file to save server state") //
		("save-state-period,p",  po::value(&save_period)->value_name("milliseconds"s), "time period to save server state in milliseconds");
        
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.contains("help"s)) {
        std::cout << desc;
        return std::nullopt;
    }

    if (!vm.contains("config-file"s)) {
        throw std::runtime_error("Config file path has not been specified"s);
    }

    // if (!vm.contains("www-root"s)) {
    //     throw std::runtime_error("Static files root path is not specified"s);
    // }

    if (vm.contains("tick-period"s))
    {
    	args.tick_period = std::stoi(tick_period);
    }

    if (vm.contains("state-file"s) && vm.contains("save-state-period"s))
    {
    	args.save_period = std::stoi(save_period);
    }
    
    args.spawn_random_points = vm.contains("randomize-spawn-points"s) ? true : false;

    return args;
} 

AppConfig GetConfigFromEnv() {
    AppConfig config;
    if (const auto* url = std::getenv(LEAVE_GAME_DB_URL_ENV_NAME)) {
        config.db_url = url;
    } else {
        throw std::runtime_error(LEAVE_GAME_DB_URL_ENV_NAME + " environment variable not found"s);
    }
    return config;
}

void SaveRetiredPlayer(const PlayerRecordItem& record)
{
	struct PlayerTag {};
	using PlayerId = util::TaggedUUID<PlayerTag>;

	// model::PlayerRecordItem record{PlayerId::New().ToString(), player_name, score, play_time};

	ConnectionPoolSingleton* inst = ConnectionPoolSingleton::getInstance();
	auto* conn_pool = inst->GetPool();
	auto conn = conn_pool->GetConnection();
	postgres::RetiredRepositoryImpl rep{*conn};
	rep.SaveRetired(PlayerId::New().ToString(), record);
}

std::vector<PlayerRecordItem> GetRetiredPlayers(int start, int max_items)
{
	ConnectionPoolSingleton* inst = ConnectionPoolSingleton::getInstance();
	auto* conn_pool = inst->GetPool();
	auto conn = conn_pool->GetConnection();
	postgres::RetiredRepositoryImpl rep{*conn};
	return rep.GetRetired(start, max_items);
}

double convert_int_to_double(int play_time)
{
	const int millisec_In_Second = 1000;
	return static_cast<double>(play_time) / millisec_In_Second;
}

std::string GetRequestStringWithoutParameters(const std::string& request)
{
	std::string np_request = request;
	auto pos = request.rfind('?');
	if(pos != std::string::npos)
		np_request = request.substr(0, pos);

	return np_request;
}

std::string GetAuthToken(std::string_view auth)
{
	std::string_view prefix = "Bearer"sv;

	if(auth.find_first_of(prefix) == std::string_view::npos)
	{
		return "";
	}
	auth.remove_prefix(prefix.size());

	size_t pos = 0;
	do{
		pos = auth.find_first_of(' ');
		if(pos == 0)
			auth.remove_prefix(1);
	}while(pos != std::string_view::npos);

	return std::string(auth.begin(), auth.end());
}

std::pair<int, int> ParseParameters(const std::map<std::string, std::string>& params)
{
	 int start = 0;
	 int max_items = MAX_DB_RECORDS;

	 try
	 {
		 auto it  = params.find("start");
		 if(it != params.end())
			 start = std::stoi(it->second);

		 it  = params.find("maxItems");
		 if(it != params.end())
	 	 	 max_items = std::stoi(it->second);

	 }
	 catch(std::exception& ex)
	 {
	 }
	 return {start, max_items};
}

std::map<std::string, std::string> GetRequestParameters(const std::string& request)
{
	url_view u(request);
	std::map<std::string, std::string> result;

	for (auto param: u.params())
	{
	    result[param.key] = param.value;
	}

	return result;
}
}
