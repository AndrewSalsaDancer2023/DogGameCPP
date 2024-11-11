#include "test_utilities.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <boost/json.hpp>

namespace test_utils {
bool isValidToken(const std::string& token)
{
    for(std::string::size_type i = 0; i < token.length(); ++i)
    {
        if(((token[i] >= '0') && (token[i] <= '9')) ||   
          ((token[i] >= 'a') && (token[i] <= 'f')) ||
          ((token[i] >= 'A') && (token[i] <= 'F')))

          continue;
        else
            return false;
    }

    return true;
}

bool allValuesAreUnEqual(std::vector<std::string>& tokens)
{
  std::sort(std::begin(tokens), std::end(tokens));
  for(int i = 0; i < tokens.size()-1; ++i)
  {
    if(tokens[i] == tokens[i+1])
    {
      std::cout << "first token:" << i << ": " << tokens[i]
                << "second token:" << i+1 << ": " << tokens[i+1] << std::endl;
      return false;
    }
  }
  return true;
}

boost::json::value convertToJSonValue(std::string content)
{
    return boost::json::parse(content);
}

boost::json::object convertToJSonObject(std::string content)
{
    return boost::json::parse(content).as_object();
}

struct DeleterFunctor {  
    void operator()(std::ifstream* stream) {
        stream->close(); 
        delete stream;
    }
};

std::string readFile(const std::string& path)
{
  // std::unique_ptr<std::ifstream> fstream(new std::ifstream(path), DeleterFunctor());
  // std::unique_ptr<std::ifstream, DeleterFunctor> fstream(new std::ifstream(path));
  std::string content;
  try{
  std::unique_ptr<std::ifstream, DeleterFunctor> fstream(new std::ifstream);
  fstream->open(path.c_str(), std::ifstream::in);
/*
  if (!fstream->is_open()) {
       std::cout << "Error opening the file:" << path << std::endl;
       return {};
   }
*/
  // String variable to store the read data
  std::string s;
    // Read each line of the file and print it to the
    // standard output stream till the whole file is
  	// completely read
    while (getline(*fstream, s))
      content.append(s);
  }
  catch(std::exception& ex)
  {
    std::cout << "readFile:" << path << "error:" << ex.what() << std::endl;
  }
    // f.close();
    std::cout << content << std::endl;
    return content;
}
//https://docs.conan.io/1/howtos/sanitizers.html#passing-the-information-to-the-compiler-or-build-system
}