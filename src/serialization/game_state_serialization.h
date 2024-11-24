#include "../core/DataTypes.h"

void serialize_game_state(const GameState& state, const std::string& path);
GameState deserialize_game_state(const std::string& path);