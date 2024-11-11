#pragma once
#include <string>
#include <vector>
#include <boost/json/detail/value_to.hpp>

namespace test_utils {
    bool isValidToken(const std::string& token);
    bool allValuesAreUnEqual(std::vector<std::string>& tokens);
    boost::json::value convertToJSonValue(std::string content);
    boost::json::object convertToJSonObject(std::string content);
    std::string readFile(const std::string& path);
}