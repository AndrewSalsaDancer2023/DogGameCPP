#include <boost/asio/io_context.hpp>
#include <thread>
#include <boost/asio/signal_set.hpp>
#include "network/request_handler.h"
#include "core/MapParser.h"
#include "core/Game.h"
#include "core/DataTypes.h"
#include "serialization/game_state_serialization.h"
#include "db/postgres.h"
// #include "event_logger.h"

#include <cstdlib>
#include <memory>
#include "utility/utility_functions.h"
using namespace std::literals;
namespace net = boost::asio;
namespace sys = boost::system;


namespace {

// Запускает функцию fn на num_threads потоках, включая текущий
template <typename Fn>
void RunWorkers(unsigned num_threads, const Fn& fn) {
	num_threads = std::max(1u, num_threads);
    std::vector<std::jthread> workers;
    workers.reserve(num_threads - 1);
    // Запускаем num_threads-1 рабочих потоков, выполняющих функцию fn
    while (--num_threads) {
        workers.emplace_back(fn);
    }
    fn();
}

}  // namespace

int main(int argc, const char* argv[]) {
	// std::optional<Args> args;
	// args = ParseCommandLine(argc, argv);
	// if(!args)
	// 	return EXIT_FAILURE;


    try {
        std::optional<Args> args = Args{};
    	postgres::Database db{pqxx::connection{LEAVE_GAME_DB_URL_ENV_VALUE}};
    	db.CreateTable();
        // 1. Загружаем карту из файла и построить модель игры
        // model::Game game = json_loader::LoadGame(args->config_file, args->www_root);
        std::filesystem::path pth(args->config_file);
        auto json_value = ReadJson(pth);
        auto stor = std::make_shared<MapStorage>();
        stor->parse_maps(json_value);

        auto tokenizer = std::make_shared<utils::PlayerToken>();
        auto game = create_game(stor, tokenizer/*, repository: ConcreteRepository, exception_coro: Coroutine*/);
        // if(args->tick_period > 0)
        // 	game.SetTickPeriod(args->tick_period);

        if(!args->save_file.empty() && (args->save_period > 0))
        {
        	// game.AddSavePath(args->save_file);
        	// game.SetSavePeriod(args->save_period);
            game->set_save_path(args->save_file);
            game->restore_sessions(deserialize_game_state(game->get_save_path()));
        }
        // game.SetSpawnInRandomPoint(args->spawn_random_points);
        // Game game(stor, tokenizer, args->spawn_random_points);
        // game.set_tick_period(args->tick_period);
        // 2. Инициализируем io_context
        const unsigned num_threads = std::thread::hardware_concurrency();
        net::io_context ioc(num_threads);

        // 3. Добавляем асинхронный обработчик сигналов SIGINT и SIGTERM
        // Подписываемся на сигналы и при их получении завершаем работу сервера
        net::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc, game](const sys::error_code& ec, [[maybe_unused]] int signal_number) {
        		if (!ec) {
        			ioc.stop();
                    game->serialize_state();
        			event_logger::LogServerEnd("server exited", EXIT_SUCCESS);
        		}
        });

        // 4. Создаём обработчик HTTP-запросов и связываем его с моделью игры
        auto handler = std::make_shared<http_handler::RequestHandler>(game, ioc);

        // 5. Запустить обработчик HTTP-запросов, делегируя их обработчику запросов
        const auto address = net::ip::make_address("0.0.0.0");
        constexpr net::ip::port_type port = 8080;
        
        http_server::ServeHttp(ioc, {address, port}, [&handler](auto&& req, auto&& send) {
        	handler->operator()(std::forward<decltype(req)>(req), std::forward<decltype(send)>(send));
        });
        
	    event_logger::InitLogger();
        // Эта надпись сообщает тестам о том, что сервер запущен и готов обрабатывать запросы
        event_logger::LogStartServer(address.to_string(), port, "server started");

        // 6. Запускаем обработку асинхронных операций
        RunWorkers(std::max(1u, num_threads), [&ioc] {
            ioc.run();
        });
    } catch (const std::exception& ex) {
        std::string reason{ex.what()};
        event_logger::LogServerEnd("server exited", EXIT_FAILURE, ex.what());
        return EXIT_FAILURE;
    }
}
