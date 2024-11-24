#include "Game.h"
#include "DataTypes.h"
#include <algorithm>
#include "Dog.h"
#include "MapParser.h"
#include "GameSession.h"
#include "Utils.h"
#include "Exceptions.h"
#include "../utility/utility_functions.h"
#include "../serialization/game_state_serialization.h"

Game::Game(std::shared_ptr<MapStorage> storage, std::shared_ptr<utils::ITokenGenerator> generator, 
           int tick_period, bool spawn_in_random_points/*, ConcreteRepository repository, exception_coro: Coroutine*/)
{
    storage_ = storage;
    generator_ = generator;
    spawn_in_random_points_ = spawn_in_random_points;
    tick_period_=tick_period;
    // self.repository_ = repository
    // self.data_saver_ = SaveDataCoroutineRunner()
    // self.exception_handler_ = exception_coro
}

const PlayersList& Game::find_all_players_for_auth_info(const std::string& auth_token)
{
    const auto& session = __get_session_for_token(auth_token);
    if(!session)
        throw InvalidTokenException();

    return session->get_all_players();
}

const LootList& Game::get_loots_for_auth_info(const std::string& auth_token)
{
    auto session = __get_session_for_token(auth_token);
    if(!session)
        throw InvalidTokenException();

    return session->get_loots_info();
}

    std::shared_ptr<Player> Game::GetPlayerWithAuthToken(const std::string& auth_token)
    {
	    auto itFind = std::find_if(sessions_.begin(), sessions_.end(),[&auth_token](std::shared_ptr<GameSession>& session){
			return session->HasPlayerWithAuthToken(auth_token) == true;
		});

	    if(itFind == sessions_.end())
		    throw PlayerAbsentException();

	    return (*itFind)->GetPlayerWithAuthToken(auth_token);
    }

    bool Game::has_session_with_auth_info(const std::string& auth_token)
    {
        auto itFind = std::find_if(sessions_.begin(), sessions_.end(),[&auth_token](std::shared_ptr<GameSession>& session){
			return session->HasPlayerWithAuthToken(auth_token) == true;
		});

	    if(itFind == sessions_.end())
		    return false;

	    return true;
    }

    std::shared_ptr<GameSession> Game::get_session_with_auth_info(const std::string& auth_token)
    {
        auto itFind = std::find_if(sessions_.begin(), sessions_.end(),[&auth_token](std::shared_ptr<GameSession>& session){
					return session->HasPlayerWithAuthToken(auth_token) == true;
				});

	    if(itFind == sessions_.end())
		    throw InvalidSessionException();

	    return *itFind;
    }
        

    PlayerAuthInfo Game::add_player(const std::string& map_id, const std::string& player_name)
    {
        if(player_name.empty()) 
	        throw EmptyNameException();

        // if(!mapToAdd)
	    //     throw MapNotFoundException();
        auto mapToAdd = find_map(map_id);

        std::shared_ptr<GameSession> session = __find_session(map_id);
        if(!session)
        {
    	    auto [loot_period, loot_probability] = get_loot_parameters();
    	    session = std::make_shared<GameSession>(map_id, loot_period, loot_probability);
    	    sessions_.push_back(session);
        }

        auto player = session->add_player(player_name, mapToAdd, generator_->GetToken(), spawn_in_random_points_, storage_->get_default_bag_capacity());
        return {player->GetToken(), player->get_id()};
    }

    void Game::MoveDogs(int delta_time)
    {
       std::for_each(sessions_.begin(), sessions_.end(),[delta_time](std::shared_ptr<GameSession>& session)
       {
		    session->MoveDogs(delta_time);
	    });
    }

    void Game::GenerateLoot(int delta_time)
    {
        std::for_each(sessions_.begin(), sessions_.end(),[this, delta_time](std::shared_ptr<GameSession>& session)
        {
		    const auto& map_name =  session->GetMap();
		    auto map = find_map(map_name);
		    // if(pMap)
			session->GenerateLoot(delta_time, map);
	    });
    }

    void Game::set_loot_parameters(const LootConfig& loot)
    {
        loot_period_ = loot.at("period");
        loot_probability_ = loot.at("probability");
    }
    
    std::shared_ptr<GameSession> Game::__find_session(const std::string& map_name)
    {
        auto itFind = std::find_if(sessions_.begin(), sessions_.end(),[&map_name](std::shared_ptr<GameSession>& session){
		    return session->GetMap() == map_name;
	    });
	
	    if(itFind != sessions_.end())
		    return *itFind;
	
	    return std::shared_ptr<GameSession>();
    }


    std::shared_ptr<GameSession> Game::__get_session_for_token(const std::string& auth_token)
    {
        auto itFind = std::find_if(sessions_.begin(), sessions_.end(),[&auth_token](std::shared_ptr<GameSession>& session){
		    return session->HasPlayerWithAuthToken(auth_token) == true;
	    });

	    if(itFind == sessions_.end())
		    return std::shared_ptr<GameSession>();

	    return (*itFind);
    }

    void Game::timer_handler()
    {
        try {
                auto period = get_tick_period();
                GenerateLoot(period);
                MoveDogs(period);

                // serialize_sessions(period);
                // handle_retired_players();
        }
        catch(...)
        {
            // logger.exception(print_exc())
        }
    }

    GameState Game::create_game_state()
    {
/*        if not self.save_period:
            return []

*/
        GameState game_state;
        for(const auto& session: sessions_)
            game_state.emplace_back(session->get_session_state());

        return game_state;
    }

    void Game::serialize_state()
    {
        const auto& state = create_game_state();
        if(state.empty())
            return;

        serialize_game_state(state, save_path);
    }

    void Game::serialize_sessions(int interval)
    {
        time_without_saving_ += interval;
        if(time_without_saving_ < save_period)
            return;

        serialize_state();

    /*
        state = self.create_game_state()
        if not state:
            return
        serialized_state = game.utils.ModelSerialization.save_sessions_states(state)
        if serialized_state:
            await write_json("/sessions_state.tmp", serialized_state)
*/
        time_without_saving_ = 0;
    }

    void Game::init_dog(shared_ptr<Dog> dog, const PlayerState& player_state)
    {
        dog->set_position(player_state.dog.pos_);
        dog->set_direction(direction_from_string(player_state.dog.dir_));
        dog->set_score(player_state.dog.score);
        dog->set_idle_time(player_state.dog.idle_time);
        dog->set_play_time(player_state.dog.play_time);
        dog->set_road_index(player_state.dog.cur_road);

        for(const auto& item : player_state.dog.bag)
            dog->append_gathered_loot(LootInfo{item.id_, item.type_, 0, 0});
    }

    void Game::restore_sessions(const GameState& state)
    {
        sessions_.clear();
        for(const auto& session: state)
        {
            auto [loot_period, loot_probability] = get_loot_parameters();
            auto new_session = std::make_shared<GameSession>(session.map_id, loot_period, loot_probability);
            new_session->set_loot_list(session.loots_info_state);

            for(const auto& player_state : session.players)
            {
                auto map_to_add = find_map(session.map_id);
                auto player = new_session->add_player(player_state.name, map_to_add, player_state.token,
                                            spawn_in_random_points_, storage_->get_default_bag_capacity());
                player->set_id(player_state.id_);
                //player.set_name(player_state.name);
                // player->set_token(player_state.token);
                auto dog = player->GetDog();
                init_dog(dog, player_state);
            }

            sessions_.emplace_back(new_session);
        }
    }
/*
    def restore_sessions(self, session_content):
        self.sessions_.clear()
        for state in game.utils.ModelSerialization.restore_sessions_states(session_content):
            loot_period, loot_probability = self.get_loot_parameters()
            session = GameSession(state.map_id, loot_period, loot_probability)

            session.set_loot_list(state.loots_info_state)
            for player_state in state.players:
                map_to_add = self.find_map(state.map_id)
                player = session.add_player(player_state.name, map_to_add,
                                            self.spawn_in_random_points_, self.storage_.get_default_bag_capacity())
                player.set_id(player_state.id_)
                # player.set_name(player_state.name)
                player.set_token(player_state.token)
                dog = player.GetDog()
                self.init_dog(dog, player_state)

            self.sessions_.append(session)
*/

    PlayersList Game::find_expired_players()
    {    
        PlayersList result;
        for(const auto& session : sessions_)
            for(const auto& player : session->get_all_players())
            {
                auto idle_time = player->GetDog()->get_idle_time();
                if(idle_time >= dog_retirement_time_)
                    result.push_back(player);
            }
        return result;
    }

    void Game::save_expired_players(const PlayersList& players)
    {  
        std::vector<PlayerRecordItem> retired;

        for(const auto& player : players)
        {
            auto dog = player->GetDog();
            SaveRetiredPlayer(PlayerRecordItem{player->GetName(), dog->get_score(), dog->get_play_time()});
            // retired.push_back(PlayerRecordItem{player->GetName(), dog->get_score(), dog->get_play_time()});
        }
        // repository_.save_retired(convert_to_db_representation(retired));
    }

    void Game::delete_expired_players(const PlayersList& players)
    {
        for(const auto& player : players)
            for(const auto& session : sessions_)
            {    
                auto candidate = session->GetPlayerWithAuthToken(player->GetToken());
                if(candidate)
                    session->delete_player(candidate);
                    break;
            }
    }

    void Game::delete_empty_sessions()
    {
        SessionList session_list;

        for(const auto& session : sessions_)
            if(session->get_all_players().empty())
                session_list.push_back(session);

        for(auto& session : session_list)
            utils::remove_element_from_vector(sessions_, session);
            // sessions_.remove(session);
    }

    void Game::handle_retired_players()
    {
        const auto& expired_players = find_expired_players();
        if(expired_players.empty())
            return;

        save_expired_players(expired_players);
        delete_expired_players(expired_players);
        delete_empty_sessions();
    }

    std::vector<PlayerRecordItem> Game::get_retired_players(int start, int max_items)
    {
       return GetRetiredPlayers(start, max_items);
    }

std::shared_ptr<Game> create_game(std::shared_ptr<MapStorage> storage, std::shared_ptr<utils::PlayerToken> tokenizer,
                                  int tick_period, bool spawn_random_point
                                  /*, repository: ConcreteRepository, exception_coro: Coroutine*/)
{
    // auto storage = std::make_shared<MapStorage>();
    // storage->parse_maps(ReadJson(config_content));
    /*std::unique_ptr<Game>*/auto game = std::make_shared<Game>(storage, tokenizer, tick_period, spawn_random_point/*, repository, exception_coro*/);

    // game.data_saver_.start(create_database, 'postgres', 'records')
    // game.data_saver_.start(game.repository_.create_table)

    game->set_default_dog_speed(storage->get_default_dog_speed());
    game->set_dog_retirement_time(storage->get_dog_retirement_time()*1000);
    game->set_loot_parameters(storage->get_loot_config());
    game->set_save_period(15000);
    game->set_tick_period(tick_period);
    // game->set_tick_period(100);

    // game.SetDefaultBagCapacity(storage.getDefaultBagCapacity())

    return game;
}