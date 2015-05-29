/*
 Title        : regex trait
 Project      : string parser
 File         : regex_trait.h
 Despcription : defines the regex for the basic types
 License      : MIT License (MIT)
 Copyright (c) 2015 Julien HARBULOT
 */

#ifndef string_parser_regex_trait_h
#define string_parser_regex_trait_h

#include <regex>

namespace cpp_sscanf {
    namespace trait{
        template <class T>
        struct regex_trait;
        
        template <>
        struct regex_trait<double>{
            static std::regex regex(double){ return std::regex{R"(-?\d*\.?\d+(?:e[+-]?\d+)?)"}; }
        };
        
        template <>
        struct regex_trait<int>{
            static std::regex regex(int){ return std::regex{R"(-?\d+)"}; }
        };
    }//trait
}//cpp_scanf
#endif
