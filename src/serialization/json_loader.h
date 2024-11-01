#include <map>
#include <string>
#include "../core/DataTypes.h"

namespace json_loader {

std::map<std::string, std::string> ParseJoinGameRequest(const std::string& body);
DogDirection GetMoveDirection(const std::string& body);
int ParseDeltaTimeRequest(const std::string& body);
}  // namespace json_loader
