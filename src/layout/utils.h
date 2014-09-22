//
//  utils.h
//  example_config
//
//  Created by Osiloke Emoekpere on 9/21/14.
//
//

#ifndef example_config_utils_h
#define example_config_utils_h
#include <locale>
std::string ofToUpper(const std::string& s)
{
    std::string result;
    
    std::locale loc;
    for (unsigned int i = 0; i < s.length(); ++i)
    {
        result += std::toupper(s.at(i), loc);
    }
    
    return result;
}

std::string ofToLower(const std::string& s)
{
    std::string result;
    
    std::locale loc;
    for (unsigned int i = 0; i < s.length(); ++i)
    {
        result += std::tolower(s.at(i), loc);
    }
    
    return result;
}


#endif
