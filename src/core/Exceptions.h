#pragma once
#include <exception> 
#include <string>

class MapNotFoundException: public std::exception
{
    std::string reason{"map not found"};
public:    
    const char* what() const noexcept override
    {
        return reason.c_str();
    }
};

class UnknownRoadOrientation: public std::exception
{
    std::string reason{"unknown road orientation"};
public:
    const char* what() const noexcept override
    {
        return reason.c_str();
    }
};

class EmptyNameException: public std::exception
{
    std::string reason{"empty name"};
public:
    const char* what() const noexcept override
    {
        return reason.c_str();
    }
};

class ParsingJsonException: public std::exception
{
    std::string reason{"parsing json error"};
public:
    const char* what() const noexcept override
    {
        return reason.c_str();
    }
};

class MetodNotAllowedException: public std::exception
{
    std::string reason{"method not allowed"};
public:
    const char* what() const noexcept override
    {
        return reason.c_str();
    }
};

class InvalidTokenException: public std::exception
{
    std::string reason{"token is invalid"};
public:
    const char* what() const noexcept override
    {
        return reason.c_str();
    }
};

class PlayerAbsentException: public std::exception
{
    std::string reason{"player is absent"};
public:
    const char* what() const noexcept override
    {
        return reason.c_str();
    }
};

class DogSpeedException: public std::exception
{
    std::string reason{"dog speed error"};
public:
    const char* what() const noexcept override
    {
        return reason.c_str();
    }
};

class InvalidSessionException: public std::exception
{
    std::string reason{"invalid session"};
public:
    const char* what() const noexcept override
    {
        return reason.c_str();
    }
};

class BadDeltaTimeException: public std::exception
{
    std::string reason{"bad delta time exception"};
public:
    const char* what() const noexcept override
    {
        return reason.c_str();
    }
};

class LootNotSpecifiedException: public std::exception
{
    std::string reason{"loot not specified"};
public:
    const char* what() const noexcept override
    {
        return reason.c_str();
    }
};

class RoadNotSpecifiedException: public std::exception
{
    std::string reason{"road not specified"};
public:
    const char* what() const noexcept override
    {
        return reason.c_str();
    }
};        