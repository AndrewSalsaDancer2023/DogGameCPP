#pragma once
#include <random>
#include <sstream>

namespace utils
{
int GetRandomIntNumber(int minValue, int maxValue);

float GetRandomFloatNumber(float minValue, float maxValue);

constexpr size_t default_token_size_ = 32;

class ITokenGenerator {
public:
    virtual std::string GetToken() = 0;
};

class PlayerToken : public ITokenGenerator {
public:
	std::string GetToken() override
	{
	auto hexConverter = [](const auto& value) -> std::string
			{
                 std::stringstream sstream;
                 sstream << std::hex << value;
                 return sstream.str();
			};

   std::string token;
   do {
         token = hexConverter(generator1_()) + hexConverter(generator2_());
   }while(token.size() != default_token_size_);

   return token;
}
     	
private:
    std::random_device random_device_;
    std::mt19937_64 generator1_{[this] {
        std::uniform_int_distribution<std::mt19937_64::result_type> dist;
        return dist(random_device_);
    }()};
    std::mt19937_64 generator2_{[this] {
        std::uniform_int_distribution<std::mt19937_64::result_type> dist;
        return dist(random_device_);
    }()};
};

template <typename T>
void remove_element_from_vector(std::vector<T>& container, T& element)
{
	const auto new_end{std::remove(std::begin(container), std::end(container), element)};
	container.erase(new_end, std::end(container));
}
}