/*
 Title        : string matcher
 Project      : string parser
 File         : string_matcher.h
 Despcription : defines the operaro<<(..., StringPattern) function
 License      : MIT License (MIT)
 Copyright (c) 2015 Julien HARBULOT
 */

#ifndef string_parser_string_matcher_h
#define string_parser_string_matcher_h

#include <istream>
#include <ios>
#include <stdexcept>

namespace string_matcher {
    
    struct StringPattern{
        const std::string str;
        
        StringPattern(const char* pattern)
        :str{pattern}
        {}
        
        StringPattern(const std::string& pattern)
        :str{pattern}
        {}
    };
    std::istream& operator>>(std::istream& stream, StringPattern const& pattern){
        if(not stream.good() ){
            return stream;
        }
        
        std::string buffer(pattern.str.size(), '\0');
        try{
            stream.read(&buffer[0], pattern.str.size());
        }catch(std::exception& e) {
            if(buffer == pattern.str) throw; //unknown case
            //otherwise failbit will be set :
        }
        
        if(buffer != pattern.str){
            stream.clear();
            stream.seekg( - stream.gcount(), std::ios_base::cur);
            stream.clear(std::ios::failbit);
        }
        
        return stream;
    }
}
#endif