cmake_minimum_required(VERSION 3.11)

project(game_server CXX)
set(CMAKE_CXX_STANDARD 20)
set(CONAN_DISABLE_CHECK_COMPILER TRUE CACHE BOOL "" FORCE )
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
conan_basic_setup(TARGETS)

find_package(Boost 1.81.0 REQUIRED)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Добавляем библиотеку, указывая, что она статическая.
add_library(GameLib STATIC 
	src/core/MapModel.h
	src/core/MapModel.cpp
	src/core/DataTypes.h 
	src/core/DataTypes.cpp
	src/core/Utils.h
	src/core/Utils.cpp	
	src/core/CollisionDetector.h
	src/core/CollisionDetector.cpp
	src/core/Exceptions.h
	src/core/MapParser.h
	src/core/MapParser.cpp
	src/core/Dog.h
	src/core/Dog.cpp
	src/core/GameSession.h
	src/core/GameSession.cpp
	src/core/Game.h
	src/core/Game.cpp
	src/core/LootGenerator.h
	src/core/LootGenerator.cpp
	src/logging/event_logger.h
	src/logging/event_logger.cpp
	src/network/ticker.h
	src/network/request_handler.h	
	src/network/api_handler.h
	src/network/api_handler.cpp
	src/serialization/json_loader.h
	src/serialization/json_loader.cpp	
	src/serialization/json_serializer.h
	src/serialization/json_serializer.cpp
	src/network/http_server.h
	src/network/http_server.cpp
	src/utility/utility_functions.h
	src/db/postgres.h
	src/db/postgres.cpp	
)

# они должны быть видны и в библиотеке GameLib и в зависимостях.
target_link_libraries(GameLib PUBLIC Threads::Threads CONAN_PKG::boost CONAN_PKG::libpq CONAN_PKG::libpqxx)
target_include_directories(GameLib PUBLIC CONAN_PKG::boost)

add_executable(game_server
		src/main.cpp
)

target_link_libraries(game_server PRIVATE GameLib)

add_executable(server_tests
	test/test_data_types.cpp

)

target_link_libraries(server_tests PRIVATE CONAN_PKG::gtest)
target_link_libraries(server_tests PRIVATE GameLib)
